#include <UI/Frame.h>
#include <SoftEngine.h>

/**
 * Frame
 * -----
 */
Frame::Frame(const Soft::Region& region, int borderSize) {
	this->region = region;
	this->borderSize = borderSize;
}

void Frame::initialize() {
	outerFrame = new Soft::UIRect();
	innerFrame = new Soft::UIRect();

	outerFrame->setColor({ 150, 80, 10 });
	outerFrame->setSize(region.width, region.height);
	outerFrame->position = { region.x, region.y };

	innerFrame->setColor({ 0, 20, 40 });
	innerFrame->setSize(region.width - borderSize * 2, region.height - borderSize * 2);
	innerFrame->position = { region.x + borderSize, region.y + borderSize };

	add(outerFrame);
	add(innerFrame);
}

const Soft::Coordinate& Frame::getPosition() const {
	return outerFrame->position;
}

void Frame::tweenTo(const Soft::Coordinate& target, int duration, Soft::Ease::EaseFunction easing) {
	outerFrame->tweenTo(target, duration, easing);
	innerFrame->tweenTo({ target.x + 10, target.y + 10 }, duration, easing);
}
