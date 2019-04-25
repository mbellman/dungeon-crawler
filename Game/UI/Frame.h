#pragma once

#include <SoftEngine.h>

class Frame : public Soft::Entity {
public:
	Frame(const Soft::Region& region, int borderSize = Frame::DEFAULT_BORDER_SIZE);

	void initialize() override;
	const Soft::Coordinate& getPosition() const;
	void tweenTo(const Soft::Coordinate& target, int duration, Soft::Ease::EaseFunction easing);

private:
	constexpr static int DEFAULT_BORDER_SIZE = 10;

	int borderSize;
	Soft::Region region;
	Soft::UIRect* outerFrame = nullptr;
	Soft::UIRect* innerFrame = nullptr;

	void updateSize();
};
