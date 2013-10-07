#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <vector>

#include "mathlib.h"

struct ChunkMemoryFootprint
{
	size_t densityValues;
	size_t densityValueSize;
	size_t triangles;
	size_t triangleSize;

	const size_t densityBytes() const
	{
		return densityValues * densityValueSize;
	}

	const size_t triangleBytes() const
	{
		return triangles * triangleSize;
	}

	const size_t totalBytes() const
	{
		return densityBytes() + triangleBytes();
	}
};

class Chunk
{
public:
	typedef float DensityType;

	/**
	* The size of the chunk
	*/
	static const float SIZE;

	/**
	* The number of sub cubes along one axis.
	* RESOLUTION + 1 is the edge length of the density cube.
	*/
	static const unsigned int RESOLUTION;

	/**
	* ctor
	* Generates a new chunk for the given chunk position
	*/
	Chunk(Vector3I position);

	/**
	* dtor
	*/
	~Chunk();

	/**
	* Converts a density value coordinate to a world coordinate.
	*/
	template<typename T>
	Vector3F toWorld(T x, T y, T z) const
	{
		Vector3F v;
		v.x = position.x * SIZE - SIZE / 2.0f + SIZE / RESOLUTION * (x - 1);
		v.y = position.y * SIZE - SIZE / 2.0f + SIZE / RESOLUTION * (y - 1);
		v.z = position.z * SIZE - SIZE / 2.0f + SIZE / RESOLUTION * (z - 1);
		return v;
	}

	/**
	* Converts a density value coordinate to a world coordinate.
	*/
	template<typename T>
	Vector3<T> toWorld(Vector3<T> v) const
	{
		return toWorld<T>(v.x, v.y, v.z);
	}

	const Vector3I getPosition() const;

	void render() const;

	const ChunkMemoryFootprint getMemoryFootprint() const;

private:
	Vector3I position;

	DensityType* densities;

	std::vector<Vector3UI> triangles;

	std::vector<Vertex> vertices;

	GLuint vertexBuffer;
	GLuint indexBuffer;

	void createBuffers();

	friend void marchChunk(Chunk& c, float* block);
};
