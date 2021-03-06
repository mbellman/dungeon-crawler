#pragma once

#include <functional>
#include <vector>
#include <algorithm>
#include <System/Math.h>
#include <System/Geometry.h>
#include <System/Positionable.h>
#include <Graphics/Color.h>
#include <Loaders/ObjLoader.h>
#include <Graphics/TextureBuffer.h>
#include <Constants.h>

namespace Soft {

/**
 * UpdateHandler
 * -------------
 */
typedef std::function<void(int)> UpdateHandler;

/**
 * Object
 * ------
 */
struct Object : public Positionable3d {
	bool isStatic = false;
	bool isFlatShaded = false;
	bool hasLighting = true;
	bool canOccludeSurfaces = true;
	float fresnelFactor = 0.0f;
	TextureBuffer* texture = NULL;
	UpdateHandler onUpdate = nullptr;
	int sectorId = GLOBAL_SECTOR_ID;
	float lodDistanceThreshold = LOD_DISTANCE_THRESHOLD;
	Vec3 transformOrigin;

	/**
	 * Controls the running lifetime of the Object in milliseconds.
	 * Objects with lifetimes > 0 are automatically removed from the
	 * Scene when they expire. The default value allows Objects to
	 * persist indefinitely.
	 */
	int lifetime = -1;

	/**
	 * Allows the default near plane clipping distance to be
	 * overridden on a per-object basis, remedying problems
	 * with depth range over the near -> far span of extremely
	 * large triangles.
	 */
	float nearClippingDistance = NEAR_PLANE_DISTANCE;

	virtual ~Object();

	void addLOD(Object* lod);
	void addMorphTarget(Object* morphTarget);
	void alwaysFaceToward(const Positionable3d* faceTarget);
	void faceToward(const Positionable3d* target);
	const Object* getLOD(float distance) const;
	const std::vector<Object*>& getLODs() const;
	int getPolygonCount() const;
	const std::vector<Polygon*>& getPolygons() const;
	const Vec3& getPreviousPosition() const;
	int getVertexCount() const;
	const std::vector<Vertex3d>& getVertices() const;
	bool hasLODs() const;
	bool isMorphing() const;

	template<class T>
	bool isOfType() {
		return dynamic_cast<T*>(this) != NULL;
	}

	void recomputeSurfaceNormals();
	void rotate(const Vec3& rotation);
	void rotateDeg(const Vec3& rotation);
	void rotateOnAxis(float angle, const Vec3& axis);
	void scale(float scalar);
	void scale(const Vec3& vector);
	void setColor(int R, int G, int B);
	void setColor(const Color& color);
	void setMorphTarget(int index);
	void setTexture(TextureBuffer* textureBuffer);
	void startMorph(int duration, bool shouldLoop);
	void stopMorph();
	void syncLODs();
	virtual void update(int dt);
	void updateMorph(int dt);

protected:
	std::vector<Vertex3d> vertices;

	void addPolygon(int v1_index, int v2_index, int v3_index);
	void addVertex(const Vec3& vector);
	void addVertex(const Vec3& vector, const Color& color);
	void addVertex(const Vec3& vector, const Vec2& uv);

private:
	struct Morph {
		int duration = 0;
		int time = 0;
		bool shouldLoop = false;
		bool isActive = false;
		bool isReversed = false;
	};

	Vec3 previousPosition;
	std::vector<Polygon*> polygons;
	std::vector<Object*> lods;
	Soft::Vec3 orientation;
	Morph morph;
	int totalMorphTargets = 0;
	const Positionable3d* faceTarget = nullptr;

	static Vec3 computePolygonNormal(const Polygon& polygon);
	static Vec3 computeVertexNormal(const Vertex3d& vertex);
	void applyRotationMatrix(const RotationMatrix& matrix);
	void updateOrientation(const Vec3& rotation);
};

/**
 * Model
 * -----
 */
struct Model : Object {
	Model(const ObjLoader& obj);
};

/**
 * Mesh
 * ----
 */
struct Mesh : Object {
	Mesh(int rows, int columns, float tileSize);

	void setTextureInterval(int rowInterval, int columnInterval);
	void setVertexOffsets(std::function<void(int, int, Vec3&)> offsetHandler);

private:
	int rows = 0;
	int columns = 0;
};

/**
 * Cube
 * ----
 */
struct Cube : Object {
	Cube(float radius);

	void setFaceUVCoordinates(float x1, float y1, float x2, float y2);

private:
	static Vec3 vertexPositions[24];
	static int polygonVertices[12][3];
};

/**
 * Particle
 * --------
 */
struct Particle : Object {
	bool shouldReset = true;

	Particle();

private:
	static Vec3 vertexPositions[4];
	static Vec2 uvs[4];
};

/**
 * Billboard
 * ---------
 */
struct Billboard : Object {
	Billboard(float width, float height);

private:
	static Vec3 vertexPositions[8];
	static Vec2 uvs[8];
};

/**
 * Skybox
 * ------
 */
struct Skybox : Object {
	Skybox(float size);

	void setTexture(TextureBuffer* texture);

private:
	static Vec3 vertexPositions[14];
	static Vec2 uvs[14];
};

/**
 * Light
 * -----
 */
struct Light : Object {
	TweenableFloat power = 1.0f;
	float range = 500.0f;
	bool isDisabled = false;

	const Color& getColor() const;
	const Vec3& getColorRatios() const;
	void setColor(int R, int G, int B);
	void setColor(const Color& color);
	void update(int dt) override;

private:
	Color color = { 255, 255, 255 };
	Vec3 cachedColorRatios = { 1.0f, 1.0f, 1.0f };
};

/**
 * DirectionalLight
 * ----------------
 */
struct DirectionalLight : Light {
	const Vec3& getDirection() const;
	void setDirection(const Vec3& direction);

private:
	Vec3 direction = { 0, -1, 0 };
};

} // namespace Soft
