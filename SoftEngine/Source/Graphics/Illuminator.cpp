#include <Graphics/Illuminator.h>
#include <System/Math.h>
#include <System/Geometry.h>
#include <Helpers.h>
#include <Constants.h>
#include <SDL.h>

namespace Soft {

/**
 * Illuminator
 * -----------
 */
void Illuminator::computeAmbientLightColorIntensity(const Vec3& normal, float fresnelFactor, Vec3& colorIntensity) {
	const Settings& settings = activeScene->settings;

	if (settings.ambientLightFactor > 0) {
		float dot = Vec3::dotProduct(normal, settings.ambientLightVector.unit());

		if (dot < 0) {
			float incidence = cosf((1 + dot) * M_PI / 2);
			float intensity = incidence * settings.ambientLightFactor * (1.0f + fresnelFactor);
			const Vec3& colorRatios = settings.ambientLightColor.ratios();

			colorIntensity.x += intensity * colorRatios.x;
			colorIntensity.y += intensity * colorRatios.y;
			colorIntensity.z += intensity * colorRatios.z;
		}
	}
}

void Illuminator::computeLightColorIntensity(Light* light, const Vec3& vertexPosition, const Vec3& normal, float fresnelFactor, Vec3& colorIntensity) {
	// Static lights are assumed to remain in place indefinitely,
	// but non-static lights may move. In multi-threaded mode, the
	// rendering/illumination pipeline runs one frame behind to
	// facilitate parallelization, meaning the effects of non-static
	// lights must be computed based on their last-frame position.
	const Vec3& lightPosition = light->isStatic
		? light->position
		: light->getPreviousPosition();

	if (
		light->isDisabled ||
		light->power.value() <= 0.001f ||
		abs(lightPosition.x - vertexPosition.x) > light->range ||
		abs(lightPosition.y - vertexPosition.y) > light->range ||
		abs(lightPosition.z - vertexPosition.z) > light->range
	) {
		// Color intensity remains unaffected when lights
		// are disabled, at 0 power, or out of axial range.
		return;
	}

	const Settings& settings = activeScene->settings;
	Vec3 lightSourceVector = vertexPosition - lightPosition;
	float lightDistance = lightSourceVector.magnitude();

	if (lightDistance > light->range) {
		// Out-of-range lights don't affect color intensity.
		return;
	}

	// Normalize for dot product checks
	lightSourceVector /= lightDistance;

	float normalDot = Vec3::dotProduct(normal, lightSourceVector);

	if (normalDot >= 0) {
		// Ignore vertices facing away from the light
		return;
	}

	bool isDirectional = light->isOfType<DirectionalLight>();

	// Directional lights use the angle between the light direction
	// and the light-to-vertex vector to compute incidence. In this
	// case we flip the light source vector sign to point it toward
	// the light, rather than toward the vertex, ensuring consistency
	// with the standard vertex normal dot product comparison.
	float directionalDot = isDirectional
		? Vec3::dotProduct(((DirectionalLight*)light)->getDirection(), lightSourceVector * -1.0f)
		: 0.0f;

	if (isDirectional && directionalDot >= 0) {
		// For directional lights, ignore vertices
		// behind the light direction vector
		return;
	}

	float incidence = cosf((1 + normalDot) * PI_HALF) * (isDirectional ? powf(directionalDot, 4) : 1.0f);
	float illuminance = pow(1.0f - lightDistance / light->range, 2);
	float intensity = light->power.value() * incidence * illuminance * (1.0f + fresnelFactor);
	const Vec3& colorRatios = light->getColorRatios();

	colorIntensity.x += (intensity * colorRatios.x);
	colorIntensity.y += (intensity * colorRatios.y);
	colorIntensity.z += (intensity * colorRatios.z);
}

Vec3 Illuminator::getTriangleVertexColorIntensity(Triangle* triangle, int vertexIndex) {
	const Vertex2d& vertex = triangle->vertices[vertexIndex];
	const Vec3& normal = triangle->sourcePolygon->sourceObject->isFlatShaded ? triangle->sourcePolygon->normal : vertex.normal;
	const Settings& settings = activeScene->settings;
	bool isStaticTriangle = !triangle->isSynthetic && triangle->sourcePolygon->sourceObject->isStatic;
	bool shouldRecomputeAmbientLightColorIntensity = settings.ambientLightFactor > 0 && (!isStaticTriangle || !settings.hasStaticAmbientLight);
	Vec3 colorIntensity;

	if (isStaticTriangle) {
		colorIntensity = triangle->sourcePolygon->cachedVertexColorIntensities[vertexIndex];
	} else {
		colorIntensity = { settings.brightness, settings.brightness, settings.brightness };
	}

	if (shouldRecomputeAmbientLightColorIntensity) {
		computeAmbientLightColorIntensity(normal, triangle->fresnelFactor, colorIntensity);
	}

	for (auto* light : activeScene->getLights()) {
		bool shouldRecomputeLightColorIntensity = !isStaticTriangle || !light->isStatic;

		if (shouldRecomputeLightColorIntensity) {
			computeLightColorIntensity(light, vertex.worldVector, normal, triangle->fresnelFactor, colorIntensity);
		}
	}

	return colorIntensity;
}

void Illuminator::illuminateColorTriangle(Triangle* triangle) {
	const Settings& settings = activeScene->settings;
	bool hasLighting = triangle->sourcePolygon->sourceObject->hasLighting;

	// Lighting step
	if (hasLighting) {
		for (int i = 0; i < 3; i++) {
			Vertex2d* vertex = &triangle->vertices[i];
			const Vec3 colorIntensity = getTriangleVertexColorIntensity(triangle, i);

			vertex->color.R *= colorIntensity.x;
			vertex->color.G *= colorIntensity.y;
			vertex->color.B *= colorIntensity.z;
			vertex->color.clamp();
		}
	}

	// Background color blending step
	for (int i = 0; i < 3; i++) {
		Vertex2d* vertex = &triangle->vertices[i];
		float visibilityRatio = FAST_MIN(vertex->z / settings.visibility, 1.0f);

		vertex->color = Color::lerp(vertex->color, settings.backgroundColor, visibilityRatio);
	}
}

/**
 * Performs a one-time illumination step on Polygons belonging to
 * static Objects, storing the color intensity results in the
 * Polygon's vertex color intensity cache. Only static ambient
 * light (if applicable) and static light sources should factor
 * into the cached value; non-static light sources must be
 * recalculated during runtime.
 */
void Illuminator::illuminateStaticPolygon(Polygon* polygon) {
	const Settings& settings = activeScene->settings;
	float fresnelFactor = 0.0f;

	for (int i = 0; i < 3; i++) {
		Vec3 vertexPosition = polygon->sourceObject->position + polygon->vertices[i]->vector;
		Vec3& normal = polygon->sourceObject->isFlatShaded ? polygon->normal : polygon->vertices[i]->normal;
		Vec3 colorIntensity = { settings.brightness, settings.brightness, settings.brightness };
		Vec3& cachedColorIntensity = polygon->cachedVertexColorIntensities[i];

		if (settings.hasStaticAmbientLight && settings.ambientLightFactor > 0) {
			computeAmbientLightColorIntensity(normal, fresnelFactor, colorIntensity);
		}

		for (auto* light : activeScene->getLights()) {
			if (light->isStatic) {
				computeLightColorIntensity(light, vertexPosition, normal, fresnelFactor, colorIntensity);
			}
		}

		cachedColorIntensity.x = colorIntensity.x;
		cachedColorIntensity.y = colorIntensity.y;
		cachedColorIntensity.z = colorIntensity.z;
	}
}

void Illuminator::illuminateTextureTriangle(Triangle* triangle) {
	for (int i = 0; i < 3; i++) {
		Vertex2d* vertex = &triangle->vertices[i];
		vertex->textureIntensity = getTriangleVertexColorIntensity(triangle, i);
	}
}

void Illuminator::illuminateTriangle(Triangle* triangle) {
	bool hasTexture = triangle->sourcePolygon->sourceObject->texture != NULL;
	bool hasLighting = triangle->sourcePolygon->sourceObject->hasLighting;

	if (hasTexture) {
		if (hasLighting) {
			illuminateTextureTriangle(triangle);
		} else if (hasTexture) {
			// Clear any previous lighting values, since
			// Triangles are recycled from the buffer pool
			resetTextureTriangleLighting(triangle);
		}
	} else {
		illuminateColorTriangle(triangle);
	}
}

void Illuminator::resetTextureTriangleLighting(Triangle* triangle) {
	for (int i = 0; i < 3; i++) {
		Vertex2d& vertex = triangle->vertices[i];

		vertex.textureIntensity.x = 1.0f;
		vertex.textureIntensity.y = 1.0f;
		vertex.textureIntensity.z = 1.0f;
	}
}

void Illuminator::setActiveScene(Scene* scene) {
	activeScene = scene;
}

} // namespace Soft
