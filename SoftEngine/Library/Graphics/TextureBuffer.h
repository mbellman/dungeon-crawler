#pragma once

#include <SDL.h>
#include <Graphics/Color.h>
#include <Graphics/ColorBuffer.h>
#include <vector>

namespace Soft {

/**
 * TextureBuffer
 * -------------
 */
struct TextureBuffer {
	bool shouldUseMipmaps = true;

	TextureBuffer(const char* file);
	~TextureBuffer();

	static Uint32 readPixel(SDL_Surface* surface, int index);

	void addFrame(const char* file);
	void confirmTexture();
	const ColorBuffer* getMipmap(int level) const;
	int getTotalFrames() const;
	void setCurrentFrame(int currentFrame);

private:
	bool isConfirmed = false;
	const char* file;
	int currentFrame = 0;
	std::vector<const ColorBuffer*> mipmaps;
	std::vector<TextureBuffer*> frames;

	void savePixel(SDL_Surface* surface, int index);
};

} // namespace Soft
