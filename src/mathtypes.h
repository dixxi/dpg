#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
};

struct Triangle {
	Vertex vertices[3];
};

struct BoundingBox {
	glm::vec3 lower;
	glm::vec3 upper;
};

namespace std {
	template<>
	struct hash<glm::uvec3> {
		size_t operator()(const glm::uvec3& v) const { return v.x ^ (v.y << 11) ^ (v.z << 22); }
	};

	template<>
	struct hash<glm::ivec3> {
		size_t operator()(const glm::ivec3& v) const { return v.x ^ (v.y << 11) ^ (v.z << 22); }
	};

	template<>
	struct hash<glm::vec3> {
		size_t operator()(const glm::vec3& v) const { return (int)v.x ^ ((int)v.y << 11) ^ ((int)v.z << 22); }
	};
}
