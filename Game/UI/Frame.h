#pragma once

#include <SoftEngine.h>

class Frame : public Soft::Entity {
public:
	Frame(const Soft::Region& region);

	void initialize() override;
	const Soft::Coordinate& getPosition() const;
	void tweenTo(const Soft::Coordinate& target, int duration, Soft::Ease::EaseFunction easing);

private:
	Soft::Region region;
	Soft::UIRect* outerFrame = nullptr;
	Soft::UIRect* innerFrame = nullptr;
};
