#pragma once

#include <SoftEngine.h>

class CastLight : public Soft::Entity {
public:
	CastLight(const Soft::Vec3& spawnPosition, const Soft::Vec3& direction);

	void initialize() override;

private:
	Soft::Vec3 spawnPosition;
	Soft::Vec3 direction;
};
