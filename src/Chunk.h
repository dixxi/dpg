#pragma once

#include "gl.h"
#include <array>
#include <stdint.h>
#include <vector>

#include "mathlib.h"

struct ChunkMemoryFootprint final {
	size_t densityValues;
	size_t densityValueSize;
	size_t triangles;
	size_t triangleSize;

	const size_t densityBytes() const {
		return densityValues * densityValueSize;
	}

	const size_t triangleBytes() const {
		return triangles * triangleSize;
	}

	const size_t totalBytes() const {
		return densityBytes() + triangleBytes();
	}
};

class Chunk final {
public:
	typedef float DensityType;
	typedef uint64_t IdType;

	friend class ChunkSerializer;
	friend class ChunkManager;
	friend class ChunkCreator;

	enum class VoxelType {
		SOLID,
		SURFACE,
		AIR
	};

	/**
	* The size of the chunk in world units.
	*/
	static const float SIZE;

	/**
	* The number of voxels along one axis.
	* RESOLUTION + 1 is the edge length of the density cube (a cube of voxels).
	*/
	static const unsigned int RESOLUTION;

	static IdType ChunkGridCoordinateToId(glm::ivec3 chunkGridCoord);
	static glm::ivec3 IdToChunkGridCoordinate(IdType id);

#pragma region coordinate_transformation_methods
	/**
	* Converts a voxel coordinate to a world coordinate.
	*/
	template<typename T>
	glm::vec3 toWorld(T x, T y, T z) const {
		glm::vec3 v;
		float blockLength = SIZE / RESOLUTION;
		v.x = blockLength * ((float)x - 1.0f);
		v.y = blockLength * ((float)y - 1.0f);
		v.z = blockLength * ((float)z - 1.0f);
		return getWorldPosition() + v;
	}

	/**
	* Converts a voxel coordinate to a world coordinate.
	*/
	glm::vec3 toWorld(glm::vec3 v) const {
		return toWorld(v.x, v.y, v.z);
	}

	glm::uvec3 toVoxelCoord(const glm::vec3& v) const;
#pragma endregion

	const IdType getId() const;

	/**
	* Gets the position of the chunk's center in the chunk grid.
	*/
	const glm::ivec3 getChunkGridPositon() const;

	/**
	* Gets the position of the chunk's center in the world.
	*/
	const glm::vec3 getWorldPosition() const;

	/**
	* Categorizes the given position in world coordinates.
	*/
	VoxelType categorizeWorldPosition(const glm::vec3& pos) const;

	/**
	* Renders the chunk.
	*/
	void render() const;

	/**
	* Called by ChunkManager
	*/
	void createBuffers();

	/**
	* Get the memory footprint of this chunk.
	*/
	const ChunkMemoryFootprint getMemoryFootprint() const;

	/**
	* dtor
	*/
	~Chunk();

private:
	IdType id{};
	glm::ivec3 position;

	DensityType* densities;
	std::vector<glm::uvec3> triangles;
	std::vector<Vertex> vertices;

	bool buffersInitialized;
	GLuint vertexBuffer{};
	GLuint indexBuffer{};

	/**
	* ctor is private, only allow ChunkCreator and ChunkSerializer to construct chunks via friends
	*/
	Chunk(IdType id);
	Chunk(glm::ivec3 chunkGridCoord);

	/*
	* Helper functions
	*/
	DensityType voxelAt(unsigned int x, unsigned int y, unsigned int z) const;
	std::array<DensityType, 8> voxelCubeAt(unsigned int x, unsigned int y, unsigned int z) const;
	unsigned int caseIndexFromVoxel(std::array<DensityType, 8> values) const;
};
