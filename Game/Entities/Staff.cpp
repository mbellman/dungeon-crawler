#include <Entities/Staff.h>
#include <SoftEngine.h>
#include <cmath>

/**
 * Staff
 * -----
 */
Staff::Staff(const Soft::Camera* camera) {
	this->camera = camera;
}

void Staff::initialize() {
	Soft::ObjLoader loader("./Assets/Models/staff.obj");
	model = new Soft::Model(loader);

	model->setColor(180, 100, 10);
	model->scale(Staff::SCALE);
	model->transformOrigin = Staff::DEFAULT_TRANSFORM_ORIGIN;

	add(model);
}

void Staff::onUpdate(int dt) {
	updatePosition();

	if (swingTween.isActive) {
		updateSwing(dt);
	} else {
		updateRotation();
	}
}

Soft::Vec3 Staff::getPitchAxis() const {
	return {
		cosf(camera->yaw),
		0.0f,
		sinf(camera->yaw)
	};
}

void Staff::swing() {
	if (swingTween.isActive) {
		return;
	}

	swingTween.value.start = 0.0f;
	swingTween.value.end = Staff::SWING_PITCH_DEGREES;
	swingTween.time = 0;
	swingTween.duration = 350;
	swingTween.easing = Soft::Ease::sineWave;
	swingTween.isActive = true;

	model->transformOrigin = Staff::SWING_TRANSFORM_ORIGIN;
	swingPitchAxis = getPitchAxis();
}

void Staff::updatePosition() {
	float sy = sinf(camera->yaw);
	float cy = cosf(camera->yaw);

	Soft::Vec3 offset = {
		cy * Staff::SIDE_DISTANCE - sy * Staff::FORWARD_DISTANCE,
		Staff::VERTICAL_OFFSET + sinf(camera->pitch) * Staff::PITCH_VERTICAL_SHIFT,
		sy * Staff::SIDE_DISTANCE + cy * Staff::FORWARD_DISTANCE
	};

	model->position = camera->position + offset;
}

void Staff::updateRotation() {
	if (camera->yaw != lastCameraYaw) {
		float yawDelta = camera->yaw - lastCameraYaw;

		model->rotate({ 0.0f, -yawDelta, 0.0f });

		lastCameraYaw = camera->yaw;
	}

	if (camera->pitch != lastCameraPitch) {
		float pitchDelta = camera->pitch - lastCameraPitch;
		float pitchAngle = -pitchDelta * 180.0f / M_PI;

		model->rotateOnAxis(pitchAngle, getPitchAxis());

		lastCameraPitch = camera->pitch;
	}
}

void Staff::updateSwing(int dt) {
	float lastPitch = swingTween.alpha();

	swingTween.time += dt;

	float alpha = swingTween.alpha();
	float pitchDelta = alpha - lastPitch;
	float pitchAngle = pitchDelta * 180.0f / M_PI;

	model->rotateOnAxis(pitchAngle, swingPitchAxis);

	if (swingTween.progress() >= 1.0f) {
		model->transformOrigin = Staff::DEFAULT_TRANSFORM_ORIGIN;
		swingTween.isActive = false;
	}
}

Soft::Vec3 Staff::DEFAULT_TRANSFORM_ORIGIN = { 0.0f, 30.0f, 0.0f };
Soft::Vec3 Staff::SWING_TRANSFORM_ORIGIN = { 0.0f, -30.0f, 0.0f };
