#include <Graphics/TextureBuffer.h>
#include <SDL.h>
#include <SDL_image.h>
#include <Graphics/Color.h>
#include <Graphics/ColorBuffer.h>
#include <Helpers.h>
#include <Constants.h>
#include <UI/Alert.h>
#include <stdio.h>
#include <algorithm>
#include <vector>

static int getTotalMipmaps(int size) {
	return size <=2 ? 0 : 1 + getTotalMipmaps(size >> 1);
}

namespace Soft {

/**
 * TextureBuffer
 * -------------
 */
TextureBuffer::TextureBuffer(const char* file) {
	this->file = file;
}

TextureBuffer::~TextureBuffer() {
	for (const auto* mipmap : mipmaps) {
		delete mipmap;
	}

	for (auto* frame : frames) {
		delete frame;
	}

	frames.clear();
	mipmaps.clear();
}

void TextureBuffer::addFrame(const char* file) {
	TextureBuffer* frame = new TextureBuffer(file);

	frames.push_back(frame);
	frame->shouldUseMipmaps = shouldUseMipmaps;
}

void TextureBuffer::confirmTexture() {
	if (!isConfirmed) {
		isConfirmed = true;

		SDL_Surface* image = IMG_Load(file);

		if (image == NULL) {
			char errorMessage[80];

			sprintf(errorMessage, "Unable to load texture: %s", file);
			Alert::error(ALERT_ASSET_ERROR, errorMessage);
			exit(0);
		}

		int width = image->w;
		int height = image->h;
		int totalPixels = image->w * image->h;
		SDL_PixelFormat* format = image->format;
		ColorBuffer* colorBuffer = new ColorBuffer(width, height);

		for (int i = 0; i < totalPixels; i++) {
			Uint32 color = TextureBuffer::readPixel(image, i);
			int x = i % width;
			int y = (int)(i / width);

			colorBuffer->write(x, y, (color & 0x00FF0000) >> 16, (color & 0x0000FF00) >> 8, color & 0x000000FF);
		}

		mipmaps.push_back(colorBuffer);

		if (shouldUseMipmaps) {
			int totalMipMaps = getTotalMipmaps(std::min(width, height));
			ColorBuffer* mipmap = colorBuffer;

			for (int i = 0; i < totalMipMaps; i++) {
				mipmap = mipmap->createDownsizedBuffer();

				mipmaps.push_back(mipmap);
			}
		}

		SDL_FreeSurface(image);

		for (auto* frame : frames) {
			frame->confirmTexture();
		}
	}
}

const ColorBuffer* TextureBuffer::getMipmap(int level) const {
	if (currentFrame == 0) {
		return level >= mipmaps.size() ? mipmaps.back() : mipmaps.at(level);
	} else {
		// Since frame 0 is this TextureBuffer instance, the access
		// index for higher frames needs to be decremented by 1.
		return frames.at(currentFrame - 1)->getMipmap(level);
	}
}

/**
 * Returns the total number of animation frames in the texture,
 * including the base frame, this TextureBuffer instance.
 */
int TextureBuffer::getTotalFrames() const {
	return frames.size() + 1;
}

/**
 * A static method for returning a normalized ARGB 32-bit
 * integer representing to the color of a given pixel, by
 * index, in an SDL_Surface.
 */
Uint32 TextureBuffer::readPixel(SDL_Surface* surface, int index) {
	int x = index % surface->w;
	int y = (int)(index / surface->w);
	SDL_PixelFormat* format = surface->format;
	Uint8* pixel = (Uint8*)surface->pixels + surface->pitch * y + format->BytesPerPixel * x;

#if SDL_BYTEORDER == SDL_LIL_ENDIAN
	int R = pixel[0];
	int G = pixel[1];
	int B = pixel[2];
#else
	int R = pixel[format->BytesPerPixel - 1];
	int G = pixel[format->BytesPerPixel - 2];
	int B = pixel[format->BytesPerPixel - 3];
#endif

	return ARGB(R, G, B);
}

void TextureBuffer::setCurrentFrame(int currentFrame) {
	this->currentFrame = currentFrame;

	if (this->currentFrame > getTotalFrames()) {
		this->currentFrame = 0;
	}
}

} // namespace Soft
