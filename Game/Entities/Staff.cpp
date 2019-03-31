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

	add(model);
}

void Staff::onUpdate(int dt) {
	updatePosition();
}

void Staff::swing() {

}

void Staff::updatePosition() {
	float sy = sinf(camera->yaw);
	float cy = cosf(camera->yaw);

	Soft::Vec3 offset = {
		cy * Staff::SIDE_DISTANCE - sy * Staff::FORWARD_DISTANCE,
		-50.0f,
		sy * Staff::SIDE_DISTANCE + cy * Staff::FORWARD_DISTANCE
	};

	model->position = camera->position + offset;
}
