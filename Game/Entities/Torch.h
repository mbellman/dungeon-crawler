#pragma once

#include <SoftEngine.h>
#include <Level/LevelLoader.h>

class Torch : public Soft::Entity {
public:
	Torch(const TorchData& torchData);

	void initialize() override;
	void onUpdate(int dt) override;

private:
	TorchData torchData;
	Soft::Billboard* fire = nullptr;

	void addEmbers();
	Soft::Vec3 getTorchBasePosition();
	Soft::Vec3 getTorchLightPosition();
	float getRotationAngle();
};
