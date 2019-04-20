#include <UI/TextBox.h>
#include <UI/Frame.h>
#include <SoftEngine.h>
#include <SDL_ttf.h>
#include <algorithm>

/**
 * TextBox
 * -------
 */
TextBox::TextBox(TTF_Font* font) {
	this->font = font;
}

void TextBox::initialize() {
	frame = new Frame({
		200, TextBox::HIDDEN_TOP_OFFSET,
		800, 100
	});

	text = new Soft::UIText();
	text->setFont(font);
	text->setColor({ 255, 255, 255 });
	text->position = { 235, TextBox::HIDDEN_TOP_OFFSET + 25 };

	add(frame);
	add(text);
}

void TextBox::onUpdate(int dt) {
	if (isWriting()) {
		currentWritingTime = std::min(currentWritingTime + dt, totalWritingTime);

		int substringLength = fullValue.size() * ((float)currentWritingTime / totalWritingTime);
		std::string partialValue = fullValue.substr(0, substringLength);

		text->setValue(partialValue.c_str());
	}
}

int TextBox::getTotalWritingTime(TextSpeed speed) {
	switch (speed) {
		case TextSpeed::SLOW:
			return fullValue.size() * 100;
		case TextSpeed::NORMAL:
			return fullValue.size() * 60;
		case TextSpeed::FAST:
			return fullValue.size() * 30;
		default:
			return 1000;
			break;
	}
}

bool TextBox::isShown() const {
	return frame->getPosition().y < 750;
}

bool TextBox::isWriting() const {
	return currentWritingTime < totalWritingTime;
}

void TextBox::skipWritingAnimation() {
	currentWritingTime = totalWritingTime - 1;
}

void TextBox::write(const char* value, TextSpeed speed) {
	fullValue = value;
	currentWritingTime = 0;
	totalWritingTime = getTotalWritingTime(speed);
}

void TextBox::show() {
	frame->tweenTo({ 200, TextBox::SHOWN_TOP_OFFSET }, 400, Soft::Ease::linear);
	text->tweenTo({ 235, TextBox::SHOWN_TOP_OFFSET + 25 }, 400, Soft::Ease::linear);
}

void TextBox::hide() {
	frame->tweenTo({ 200, TextBox::HIDDEN_TOP_OFFSET }, 400, Soft::Ease::linear);
	text->tweenTo({ 235, TextBox::HIDDEN_TOP_OFFSET + 25 }, 400, Soft::Ease::linear);
}
