#include "Collider.h"

Collider::Collider(ChunkManager& mgr)
	: mgr(mgr) {}

Collider::~Collider() = default;

glm::vec3 Collider::traceOnVoxels(glm::vec3 startPos, glm::vec3 endPos) {
	return endPos;
}