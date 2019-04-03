#include <Graphics/ColorBuffer.h>
#include <algorithm>
#include <Helpers.h>
#include <Constants.h>

namespace Soft {

/**
 * ColorBuffer
 * -----------
 */
ColorBuffer::ColorBuffer(int width, int height): width(width), height(height) {
	buffer = new Color[width * height];
	bufferSize = width * height;
}

ColorBuffer::~ColorBuffer() {
	delete[] buffer;
}

ColorBuffer* ColorBuffer::createDownsizedBuffer() {
	if (width <= 2 || height <= 2) {
		return this;
	}

	ColorBuffer* colorBuffer = new ColorBuffer(width >> 1, height >> 1);

	for (int y = 0; y < height - 1; y +=2) {
		for (int x = 0; x < width - 1; x += 2) {
			const Color& tL = read(x, y);
			const Color& tR = read(x + 1, y);
			const Color& bL = read(x, y + 1);
			const Color& bR = read(x + 1, y + 1);

			int averageR = (int)((tL.R + tR.R + bL.R + bR.R) / 4.0f);
			int averageG = (int)((tL.G + tR.G + bL.G + bR.G) / 4.0f);
			int averageB = (int)((tL.B + tR.B + bL.B + bR.B) / 4.0f);

			colorBuffer->write(x >> 1, y >> 1, averageR, averageG, averageB);
		}
	}

	return colorBuffer;
}

inline int ColorBuffer::getIndex(int x, int y) const {
	if (x >= width || y >= height) {
		return 0;
	}

	return width * y + x;
}

const Color& ColorBuffer::read(int index) const {
	return (index < 0 || index >= bufferSize) ? COLOR_BLACK : buffer[index];
}

const Color& ColorBuffer::read(int x, int y) const {
	return read(getIndex(x, y));
}

const Color& ColorBuffer::sample(float u, float v) const {
	// Modulo-free out-of-bounds UV wrapping
	if (u >= 1.0f) u -= (int)u;
	else if (u < 0.0f) u += (int)(-1.0f * (u - 1.0f));

	if (v >= 1.0f) v -= (int)v;
	else if (v < 0.0f) v += (int)(-1.0f * (v - 1.0f));

	int pixelIndex = (int)(v * height) * width + (int)(u * width);

	return read(pixelIndex);
}

void ColorBuffer::write(int x, int y, int R, int G, int B) {
	Color color = { R, G, B };

	buffer[getIndex(x, y)] = color;
}

} // namespace Soft
