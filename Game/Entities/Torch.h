#pragma once

#include <SoftEngine.h>
#include <Level/LevelLoader.h>

class Torch : public Soft::Entity {
public:
	Torch(const TorchData& torchData, Soft::TextureBuffer* fireTexture);

	void initialize() override;
	void onUpdate(int dt) override;

private:
	TorchData torchData;
	Soft::TextureBuffer* fireTexture = nullptr;
	Soft::Billboard* fire = nullptr;

	void addEmbers();
	void addFire();
	void addTorchBase();
	void addTorchLight();
	Soft::Vec3 getFirePosition();
	Soft::Vec3 getTorchBasePosition();
	Soft::Vec3 getTorchDirectionVector();
	Soft::Vec3 getTorchLightPosition();
	float getRotationAngle();
};
