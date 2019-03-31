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
	model->transformOrigin = { 0.0f, -30.0f, 0.0f };

	add(model);
}

void Staff::onUpdate(int dt) {
	updatePosition();
	updateRotation();
}

void Staff::swing() {

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

		Soft::Vec3 pitchAxis = {
			cosf(camera->yaw),
			0.0f,
			sinf(camera->yaw)
		};

		model->rotateOnAxis(pitchAngle, pitchAxis);

		lastCameraPitch = camera->pitch;
	}
}
