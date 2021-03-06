#include <System/Math.h>
#include <memory>
#include <cmath>
#include <math.h>
#include <algorithm>
#include <Helpers.h>

namespace Soft {

/**
 * Ease
 * ----
 */
float Ease::linear(float t) {
	return t;
}

float Ease::quadIn(float t) {
	return t * t;
}

float Ease::quadOut(float t) {
	return t * (2 - t);
}

float Ease::quadInOut(float t) {
	t *= 2.0f;

	return (t < 1.0f ? quadIn(t) : 1.0f + quadOut(t - 1.0f)) / 2.0f;
}

float Ease::sineWave(float t) {
	return sinf(t * M_PI);
}

float Ease::inOutWave(float t) {
	return t < 0.5f ? quadInOut(t * 2.0f) : 1.0f - quadInOut((t - 0.5f) * 2.0f);
}

/**
 * Coordinate
 * ----------
 */
Coordinate Coordinate::lerp(const Coordinate& c1, const Coordinate& c2, float r) {
	return {
		Lerp::lerp(c1.x, c2.x, r),
		Lerp::lerp(c1.y, c2.y, r)
	};
}

/**
 * RotationMatrix
 * --------------
 */
RotationMatrix RotationMatrix::operator *(const RotationMatrix& rm) const {
	return {
		m11 * rm.m11 + m12 * rm.m21 + m13 * rm.m31, m11 * rm.m12 + m12 * rm.m22 + m13 * rm.m32, m11 * rm.m13 + m12 * rm.m23 + m13 * rm.m33,
		m21 * rm.m11 + m22 * rm.m21 + m23 * rm.m31, m21 * rm.m12 + m22 * rm.m22 + m23 * rm.m32, m21 * rm.m13 + m22 * rm.m23 + m23 * rm.m33,
		m31 * rm.m11 + m32 * rm.m21 + m33 * rm.m31, m31 * rm.m12 + m32 * rm.m22 + m33 * rm.m32, m31 * rm.m13 + m32 * rm.m23 + m33 * rm.m33
	};
}

RotationMatrix RotationMatrix::fromVec3(const Vec3& rotation) {
	float sx = sinf(rotation.x);
	float sy = sinf(rotation.y);
	float sz = sinf(rotation.z);
	float cx = cosf(rotation.x);
	float cy = cosf(rotation.y);
	float cz = cosf(rotation.z);

	RotationMatrix rX = { 1, 0, 0, 0, cx, -sx, 0, sx, cx };
	RotationMatrix rY = { cy, 0, sy, 0, 1, 0, -sy, 0, cy };
	RotationMatrix rZ = { cz, -sz, 0, sz, cz, 0, 0, 0, 1 };

	return rZ * rY * rX;
}

Vec3 RotationMatrix::operator *(const Vec3& v) const {
	return {
		m11 * v.x + m12 * v.y + m13 * v.z,
		m21 * v.x + m22 * v.y + m23 * v.z,
		m31 * v.x + m32 * v.y + m33 * v.z
	};
}

/**
 * Vec2
 * ----
 */
Vec2 Vec2::lerp(const Vec2& v1, const Vec2& v2, float r) {
	return {
		Lerp::lerp(v1.x, v2.x, r),
		Lerp::lerp(v1.y, v2.y, r)
	};
}

Vec2 Vec2::operator *(float scalar) const {
	return {
		x * scalar,
		y * scalar
	};
}

Vec2 Vec2::operator /(float divisor) const {
	return {
		x / divisor,
		y / divisor
	};
}

/**
 * Vec3
 * ----
 */
Vec3::Vec3() {}

Vec3::Vec3(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

void Vec3::clamp(float low, float high) {
	x = FAST_CLAMP(x, low, high);
	y = FAST_CLAMP(y, low, high);
	z = FAST_CLAMP(z, low, high);
}

Vec3 Vec3::crossProduct(const Vec3& v1, const Vec3& v2) {
	return {
		v1.y * v2.z - v1.z * v2.y,
		v1.z * v2.x - v1.x * v2.z,
		v1.x * v2.y - v1.y * v2.x
	};
}

float Vec3::distance(const Vec3& v1, const Vec3& v2) {
	return (v1 - v2).magnitude();
}

float Vec3::dotProduct(const Vec3& v1, const Vec3& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

Vec3 Vec3::lerp(const Vec3& v1, const Vec3& v2, float r) {
	return {
		Lerp::lerp(v1.x, v2.x, r),
		Lerp::lerp(v1.y, v2.y, r),
		Lerp::lerp(v1.z, v2.z, r)
	};
}

float Vec3::magnitude() const {
	return sqrt(x * x + y * y + z * z);
}

void Vec3::normalize() {
	Vec3 unitVector = unit();

	x = unitVector.x;
	y = unitVector.y;
	z = unitVector.z;
}

void Vec3::rotate(const RotationMatrix& rotationMatrix) {
	Vec3 rotatedVector = rotationMatrix * (*this);

	x = rotatedVector.x;
	y = rotatedVector.y;
	z = rotatedVector.z;
}

Vec3 Vec3::unit() const {
	float m = magnitude();

	if (m == 0.0f) {
		return *this;
	}

	return {
		x / m,
		y / m,
		z / m
	};
}

Vec3 Vec3::operator +(const Vec3& vector) const {
	return {
		x + vector.x,
		y + vector.y,
		z + vector.z
	};
}

Vec3 Vec3::operator +=(const Vec3& vector) {
	x += vector.x;
	y += vector.y;
	z += vector.z;

	return *this;
}

Vec3 Vec3::operator -(const Vec3& vector) const {
	return {
		x - vector.x,
		y - vector.y,
		z - vector.z
	};
}

Vec3 Vec3::operator -=(const Vec3& vector) {
	x -= vector.x;
	y -= vector.y;
	z -= vector.z;

	return *this;
}

Vec3 Vec3::operator *(float scalar) const {
	return {
		x * scalar,
		y * scalar,
		z * scalar
	};
}

Vec3 Vec3::operator *=(float scalar) {
	x *= scalar;
	y *= scalar;
	z *= scalar;

	return *this;
}

Vec3 Vec3::operator *=(const Vec3& vector) {
	x *= vector.x;
	y *= vector.y;
	z *= vector.z;

	return *this;
}

Vec3 Vec3::operator /(float scalar) const {
	return {
		x / scalar,
		y / scalar,
		z / scalar
	};
}

Vec3 Vec3::operator /=(float scalar) {
	x /= scalar;
	y /= scalar;
	z /= scalar;

	return *this;
}

/**
 * TweenableFloat
 * --------------
 */
TweenableFloat::TweenableFloat(float value) {
	tween.value.start = value;
	currentValue = value;
}

TweenableFloat TweenableFloat::operator =(float value) {
	currentValue = value;

	return *this;
}

bool TweenableFloat::isTweening() const {
	return tween.isActive;
}

void TweenableFloat::tweenTo(float value, int duration, Ease::EaseFunction easing) {
	tween.value.start = currentValue;
	tween.value.end = value;
	tween.time = 0;
	tween.duration = duration;
	tween.easing = easing;
	tween.isActive = true;
}

void TweenableFloat::update(int dt) {
	if (tween.isActive) {
		tween.time += dt;

		if (tween.time >= tween.duration) {
			tween.time = tween.duration;
			tween.isActive = false;
		}

		currentValue = Lerp::lerp(tween.value.start, tween.value.end, tween.alpha());
	}
}

float TweenableFloat::value() const {
	return currentValue;
}

} // namespace Soft
