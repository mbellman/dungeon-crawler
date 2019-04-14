#pragma once

#include <SDL.h>
#include <Graphics/Color.h>

namespace Soft {

/**
 * ColorBuffer
 * -----------
 *
 * A buffer of Color objects representing texture data.
 */
class ColorBuffer {
public:
	const int width;
	const int height;

	ColorBuffer(int width, int height);
	~ColorBuffer();

	ColorBuffer* createDownsizedBuffer();
	int getHeight() const;
	int getWidth() const;
	const Color& read(int index) const;
	const Color& read(int x, int y) const;
	const Color& sample(float u, float v) const;
	void write(int x, int y, int R, int G, int B);

private:
	inline int getIndex(int x, int y) const;

	Color* buffer = NULL;
	int bufferSize;
};

} // namespace Soft
