#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <System/Positionable.h>
#include <Graphics/Color.h>
#include <Graphics/ColorBuffer.h>
#include <string>

namespace Soft {

/**
 * UIObject
 * --------
 */
class UIObject : public Positionable2d {
public:
	UIObject();
	virtual ~UIObject();

	void clip(int w, int h);
	void clip(Soft::Region region);
	int getHeight() const;
	const std::string& getKey() const;
	int getWidth() const;
	void setAlpha(float alpha);
	void setRenderer(SDL_Renderer* renderer);
	void setSize(int width, int height);
	void unclip();
	void update(int dt);

protected:
	SDL_Renderer* m_renderer = 0;
	SDL_Texture* m_texture = 0;
	SDL_Rect sourceRect = { 0, 0, 0, 0 };
	SDL_Rect destRect = { 0, 0, 0, 0 };
	int width = 0;
	int height = 0;

	virtual void refresh() = 0;
	void refreshAlpha();
	void setTextureFromSurface(SDL_Surface* surface);

private:
	float alpha = 1.0f;
	std::string key;

	Uint8 getAlphaMod();
};

/**
 * UIRect
 * ------
 */
class UIRect : public UIObject {
public:
	void setColor(const Color& color);
	void setSize(int w, int h);

protected:
	void refresh();

private:
	Color color;
};

/**
 * UIText
 * ------
 */
class UIText : public UIObject {
public:
	UIText();
	UIText(const char* value);

	void setFont(TTF_Font* font);
	void setColor(const SDL_Color &color);
	void setValue(const char* value);

protected:
	void refresh();

private:
	std::string m_value = "";
	TTF_Font* m_font = 0;
	SDL_Color m_color = { 255, 255, 255 };
};

/**
 * UIGraphic
 * ---------
 */
class UIGraphic : public UIObject {
public:
	UIGraphic(const char* filename);
	UIGraphic(const ColorBuffer* colorBuffer);

protected:
	void refresh();

private:
	SDL_Surface* surface = NULL;

	void initializeSurfaceFromColorBuffer(const ColorBuffer* colorBuffer);
	void normalizeSurfaceFormat();
	void setTransparentPixels();
};

} // namespace Soft
