#include <Entities/Staff.h>
#include <GameUtils.h>
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

	staffLight = new Soft::Light();

	staffLight->setColor({ 0, 150, 0 });
	staffLight->range = 250.0f;

	staffLight->follow(model, [=](const Soft::Vec3& staffPosition, Soft::Vec3& lightPosition) {
		lightPosition = staffPosition;
		lightPosition.y += Staff::SCALE * 1.5f;
	});

	add(model);
	add(staffLight);
}

void Staff::onUpdate(int dt) {
	updatePosition();

	if (swingTween.isActive) {
		updateSwing(dt);
	} else {
		updateRotation();
	}
}

void Staff::dispelLight() {
	staffLight->power = 0.0f;
	staffLight->power.tweenTo(1.0f, GameUtils::CAST_LIGHT_COOLDOWN_TIME, Soft::Ease::quadInOut);
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
	swingTween.duration = Staff::SWING_DURATION;
	swingTween.easing = Soft::Ease::sineWave;
	swingTween.isActive = true;

	model->transformOrigin = Staff::SWING_TRANSFORM_ORIGIN;
	swingPitchAxis = getPitchAxis();
}

void Staff::updatePosition() {
	float sy = sinf(camera->yaw);
	float cy = cosf(camera->yaw);

	float bounceOffset = camera->isTweening()
		? sinf(getAge() / 50.0f) * 0.25f
		: 0.0f;

	Soft::Vec3 offset = {
		cy * Staff::SIDE_DISTANCE - sy * Staff::FORWARD_DISTANCE,
		Staff::VERTICAL_OFFSET + sinf(camera->pitch) * Staff::CAMERA_PITCH_VERTICAL_DRIFT + bounceOffset,
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
	float lastPitch = swingTween.alpha() * Staff::SWING_PITCH_DEGREES;

	swingTween.time += dt;

	float currentPitch = swingTween.alpha() * Staff::SWING_PITCH_DEGREES;
	float pitchDelta = currentPitch - lastPitch;

	model->rotateOnAxis(pitchDelta, swingPitchAxis);

	if (swingTween.progress() >= 1.0f) {
		model->transformOrigin = Staff::DEFAULT_TRANSFORM_ORIGIN;
		swingTween.isActive = false;
	}
}

Soft::Vec3 Staff::DEFAULT_TRANSFORM_ORIGIN = { 0.0f, 30.0f, 0.0f };
Soft::Vec3 Staff::SWING_TRANSFORM_ORIGIN = { 0.0f, -30.0f, 0.0f };
