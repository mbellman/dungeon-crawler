#include <Graphics/Color.h>
#include <System/Math.h>
#include <Helpers.h>

namespace Soft {

/**
 * Color
 * -----
 */
void Color::clamp() {
	R = normalize(R);
	G = normalize(G);
	B = normalize(B);
}

Color Color::lerp(const Color& c1, const Color& c2, float r) {
	return {
		Lerp::lerp(c1.R, c2.R, r),
		Lerp::lerp(c1.G, c2.G, r),
		Lerp::lerp(c1.B, c2.B, r)
	};
}

int Color::normalize(int component) {
	return FAST_CLAMP(component, 0, 255);
}

Vec3 Color::ratios() const {
	return {
		(float)R / 255.0f,
		(float)G / 255.0f,
		(float)B / 255.0f
	};
}

Color Color::operator +(int illumination) const {
	return {
		normalize(R + illumination),
		normalize(G + illumination),
		normalize(B + illumination)
	};
}

Color Color::operator +(const Color& color) const {
	return {
		normalize(R + color.R),
		normalize(G + color.G),
		normalize(B + color.B)
	};
}

Color Color::operator +=(const Color& color) {
	R = normalize(R + color.R);
	G = normalize(G + color.G);
	B = normalize(B + color.B);

	return *this;
}

Color Color::operator -(int attenuation) const {
	return {
		normalize(R - attenuation),
		normalize(G - attenuation),
		normalize(B - attenuation)
	};
}

Color Color::operator -(const Color& color) const {
	return {
		normalize(R - color.R),
		normalize(G - color.G),
		normalize(B - color.B)
	};
}

Color Color::operator -=(const Color& color) {
	R = normalize(R + color.R);
	G = normalize(G + color.G);
	B = normalize(B + color.B);

	return *this;
}

Color Color::operator *(float multiplier) const {
	return {
		normalize((int)(R * multiplier)),
		normalize((int)(G * multiplier)),
		normalize((int)(B * multiplier))
	};
}

Color Color::operator *=(float multiplier) {
	R = normalize((int)(R * multiplier));
	G = normalize((int)(G * multiplier));
	B = normalize((int)(B * multiplier));

	return *this;
}

} // namespace Soft
