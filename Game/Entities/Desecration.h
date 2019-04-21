#pragma once

#include <Entities/Interactible.h>
#include <Level/LevelLoader.h>

class Desecration : public Interactible<DesecrationData> {
public:
	Desecration(const DesecrationData& desecrationData);

	void initialize() override;

private:
	Soft::Range<Soft::Vec3> getSpawnRange();
};
