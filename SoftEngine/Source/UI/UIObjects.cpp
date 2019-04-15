#include <UI/UIObjects.h>
#include <Graphics/TextureBuffer.h>
#include <UI/Alert.h>
#include <Constants.h>
#include <Helpers.h>
#include <SDL.h>
#include <SDL_image.h>
#include <algorithm>

namespace Soft {

/**
 * UIObject
 * --------
 */
UIObject::UIObject() {
	key = RNG::uuid();
}

UIObject::~UIObject() {
	if (m_texture != NULL) {
		SDL_DestroyTexture(m_texture);
	}
}

void UIObject::clip(int w, int h) {
	sourceRect.w = w;
	sourceRect.h = h;

	destRect.w = w;
	destRect.h = h;
}

void UIObject::clip(Region region) {
	sourceRect.x = region.x;
	sourceRect.y = region.y;
	sourceRect.w = region.width;
	sourceRect.h = region.height;

	destRect.w = region.width;
	destRect.h = region.height;
}

Uint8 UIObject::getAlphaMod() {
	return alpha * 255;
}

int UIObject::getHeight() const {
	return height;
}

const std::string& UIObject::getKey() const {
	return key;
}

int UIObject::getWidth() const {
	return width;
}

void UIObject::refreshAlpha() {
	SDL_SetTextureBlendMode(m_texture, SDL_BLENDMODE_BLEND);
	SDL_SetTextureAlphaMod(m_texture, getAlphaMod());
}

void UIObject::setAlpha(float alpha) {
	this->alpha = alpha;

	if (m_texture != NULL) {
		refreshAlpha();
	}
}

void UIObject::setRenderer(SDL_Renderer* renderer) {
	m_renderer = renderer;

	refresh();
}

void UIObject::setSize(int width, int height) {
	destRect.w = width;
	destRect.h = height;
}

void UIObject::setTextureFromSurface(SDL_Surface* surface) {
	m_texture = SDL_CreateTextureFromSurface(m_renderer, surface);

	SDL_FreeSurface(surface);
}

void UIObject::unclip() {
	sourceRect.x = 0;
	sourceRect.y = 0;
	sourceRect.w = width;
	sourceRect.h = height;

	destRect.w = width;
	destRect.h = height;
}

void UIObject::update(int dt) {
	updatePosition(dt);

	if (m_texture != NULL && alpha > 0.001f) {
		destRect.x = position.x;
		destRect.y = position.y;

		SDL_RenderCopy(m_renderer, m_texture, &sourceRect, &destRect);
	}
}

/**
 * UIRect
 * ------
 */
void UIRect::refresh() {
	if (m_renderer != NULL && width > 0 && height > 0) {
		if (m_texture != NULL) {
			SDL_DestroyTexture(m_texture);
		}

		Uint32* pixels = new Uint32[width * height];
		Uint32 fillColor = ARGB(color.R, color.G, color.B);
		m_texture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, width, height);

		std::fill(pixels, pixels + width * height, fillColor);
		SDL_UpdateTexture(m_texture, NULL, pixels, width * sizeof(Uint32));
		refreshAlpha();

		delete[] pixels;
	}
}

void UIRect::setColor(const Color& color) {
	this->color = color;

	refresh();
}

void UIRect::setSize(int w, int h) {
	width = w;
	height = h;

	unclip();
	refresh();
}

/**
 * UIText
 * ------
 */
UIText::UIText() {}

UIText::UIText(const char* value) {
	m_value = value;
}

void UIText::refresh() {
	if (m_renderer != NULL && m_font != NULL && m_value.size() > 0) {
		if (m_texture != NULL) {
			SDL_DestroyTexture(m_texture);
		}

		const char* value = m_value.c_str();
		SDL_Surface* m_surface = TTF_RenderText_Solid(m_font, value, m_color);

		TTF_SizeText(m_font, value, &width, &height);
		setTextureFromSurface(m_surface);
		refreshAlpha();
		unclip();
	}
}

void UIText::setColor(const SDL_Color &color) {
	m_color = color;
	refresh();
}

void UIText::setFont(TTF_Font* font) {
	m_font = font;
	refresh();
}

void UIText::setValue(const char* value) {
	m_value = value;
	refresh();
}

/**
 * UIGraphic
 * ---------
 */
UIGraphic::UIGraphic(const char* filename) {
	surface = IMG_Load(filename);

	if (!surface) {
		char errorMessage[60];

		sprintf(errorMessage, "Unable to load image: %s", filename);
		Alert::error(ALERT_ASSET_ERROR, errorMessage);
		exit(0);
	}

	width = surface->w;
	height = surface->h;

	unclip();
	setTransparentPixels();
}


UIGraphic::UIGraphic(const ColorBuffer* colorBuffer) {
	initializeSurfaceFromColorBuffer(colorBuffer);

	width = colorBuffer->getWidth();
	height = colorBuffer->getHeight();

	unclip();
	setTransparentPixels();
}

void UIGraphic::initializeSurfaceFromColorBuffer(const ColorBuffer* colorBuffer) {
	int width = colorBuffer->getWidth();
	int height = colorBuffer->getHeight();

	surface = SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, SDL_PIXELFORMAT_RGBA32);

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			const Color& color = colorBuffer->read(x, y);
			Uint8* pixel = (Uint8*)surface->pixels + surface->pitch * y + surface->format->BytesPerPixel * x;

		#if SDL_BYTEORDER == SDL_LIL_ENDIAN
			pixel[0] = color.R;
			pixel[1] = color.G;
			pixel[2] = color.B;
			pixel[3] = 255;
		#else
			pixel[3] = color.R;
			pixel[2] = color.G;
			pixel[1] = color.B;
			pixel[0] = 255;
		#endif
		}
	}
}

void UIGraphic::normalizeSurfaceFormat() {
	if (surface == NULL) {
		return;
	}

	SDL_Surface* rgbaSurface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA32, 0);

	SDL_FreeSurface(surface);

	surface = rgbaSurface;
}

void UIGraphic::refresh() {
	if (surface != NULL) {
		setTextureFromSurface(surface);
		refreshAlpha();

		// We can safely reset the pointer since
		// the surface data is now freed
		surface = NULL;
	}
}

void UIGraphic::setTransparentPixels() {
	if (surface == NULL) {
		return;
	}

	if (surface->format->BytesPerPixel < 4) {
		normalizeSurfaceFormat();
	}

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			int pixelIndex = y * width + x;
			Uint32 color = TextureBuffer::readPixel(surface, pixelIndex);

			int R = (color & 0x00FF0000) >> 16;
			int G = (color & 0x0000FF00) >> 8;
			int B = (color & 0x000000FF);

			if (R == COLOR_TRANSPARENT.R && G == COLOR_TRANSPARENT.G && B == COLOR_TRANSPARENT.B) {
				Uint8* pixel = (Uint8*)surface->pixels + surface->pitch * y + surface->format->BytesPerPixel * x;

			#if SDL_BYTEORDER == SDL_LIL_ENDIAN
				pixel[3] = 0;
			#else
				pixel[0] = 0;
			#endif
			}
		}
	}
}

} // namespace Soft
