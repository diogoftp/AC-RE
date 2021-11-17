#include "pch.h"
#include "structs.hpp"
#include <iostream>
#include <sstream>

Vec3::Vec3() {
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
}

Vec3::Vec3(float _x, float _y, float _z) {
	x = _x;
	y = _y;
	z = _z;
}

Vec3::Vec3(float* ptr) {
	x = ptr[0];
	y = ptr[1];
	z = ptr[2];
}

float Vec3::dist(Vec3 target) {
	return sqrt(pow(x - target.x, 2) + pow(y - target.y, 2) + pow(z - target.z, 2));
}

std::string Vec3::toString() const {
	std::ostringstream output;
	output << "(" << x << ", " << y << ", " << z << ")";
	return output.str();
}

Vec3 Vec3::operator+(const Vec3& vec) {
	return Vec3(x + vec.x, y + vec.y, z + vec.z);
}

Vec3 Vec3::operator-(const Vec3& vec) {
	return Vec3(x - vec.x, y - vec.y, z - vec.z);
}

Vec3 Vec3::operator*(const Vec3& vec) {
	return Vec3(x * vec.x, y * vec.y, z * vec.z);
}

Vec3 Vec3::operator/(const Vec3& vec) {
	return Vec3(x / vec.x, y / vec.y, z / vec.z);
}

Vec3 Vec3::operator=(const Vec3& vec) {
	x = vec.x;
	y = vec.y;
	z = vec.z;
	return *this;
}
