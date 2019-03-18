#include <Entities/TextBox.h>
#include <SoftEngine.h>
#include <SDL_ttf.h>

/**
 * TextBox
 * -------
 */
TextBox::TextBox(TTF_Font* font) {
	this->font = font;
}

void TextBox::initialize() {
	background = new Soft::UIRect();
	background->setColor({ 0, 20, 40 });
	background->setSize(800, 100);
	background->position = { 200, 750 };

	text = new Soft::UIText();
	text->setFont(font);
	text->setColor({ 255, 255, 255 });
	text->position = { 250, 765 };

	add("textbox-bg", background);
	add("textbox-text", text);
}

bool TextBox::isShowing() const {
	return background->position.y < 750;
}

void TextBox::skipTypingAnimation() {

}

void TextBox::setTextValue(const char* value) {
	text->setValue(value);
}

void TextBox::show() {
	background->tweenTo({ 200, 450 }, 400, Soft::Ease::linear);
	text->tweenTo({ 250, 465 }, 400, Soft::Ease::linear);
}

void TextBox::hide() {
	background->tweenTo({ 200, 750 }, 400, Soft::Ease::linear);
	text->tweenTo({ 250, 765 }, 400, Soft::Ease::linear);
}
