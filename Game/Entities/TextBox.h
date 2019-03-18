#pragma once

#include <SoftEngine.h>
#include <SDL_ttf.h>

class TextBox : public Soft::Entity {
public:
	TextBox(TTF_Font* font);

	void initialize() override;
	bool isShowing() const;
	void skipTypingAnimation();
	void setTextValue(const char* value);
	void show();
	void hide();

private:
	Soft::UIRect* background = nullptr;
	Soft::UIText* text = nullptr;
	TTF_Font* font = nullptr;
};
