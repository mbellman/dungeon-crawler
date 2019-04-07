#pragma once

#include <Entities/Frame.h>
#include <SoftEngine.h>
#include <SDL_ttf.h>
#include <string>

/**
 * TextSpeed
 * ---------
 */
enum class TextSpeed {
	SLOW,
	NORMAL,
	FAST
};

/**
 * TextBox
 * -------
 */
class TextBox : public Soft::Entity {
public:
	TextBox(TTF_Font* font);

	void initialize() override;
	void onUpdate(int dt) override;
	bool isShown() const;
	bool isWriting() const;
	void skipWritingAnimation();
	void write(const char* value, TextSpeed speed);
	void show();
	void hide();

private:
	constexpr static int SHOWN_TOP_OFFSET = 500;
	constexpr static int HIDDEN_TOP_OFFSET = 750;

	Frame* frame = nullptr;
	Soft::UIText* text = nullptr;
	TTF_Font* font = nullptr;
	std::string fullValue = "";
	int currentWritingTime = 0;
	int totalWritingTime = 0;

	int getTotalWritingTime(TextSpeed speed);
};
