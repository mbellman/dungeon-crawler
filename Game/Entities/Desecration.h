#pragma once

#include <SoftEngine.h>
#include <Level/LevelLoader.h>

class Desecration : public Soft::Entity {
public:
	Desecration(const DesecrationData& desecrationData);

	void initialize() override;
	const DesecrationData& getDesecrationData() const;

private:
	DesecrationData desecrationData;

	Soft::Range<Soft::Vec3> getSpawnRange();
};
