#include <map>
#include <string>
#include <iostream>
#include <Loaders/ObjLoader.h>

static std::string VERTEX_LABEL = "v";
static std::string TEXTURE_COORDINATE_LABEL = "vt";
static std::string FACE_LABEL = "f";

namespace Soft {

/**
 * ObjLoader
 * ---------
 */
ObjLoader::ObjLoader(const char* path) {
	load(path);

	while (isLoading) {
		setChunkDelimiter(" ");

		std::string label = readNextChunk();

		if (label == VERTEX_LABEL) {
			handleVertex();
		} else if (label == TEXTURE_COORDINATE_LABEL) {
			handleTextureCoordinate();
		} else if (label == FACE_LABEL) {
			handleFace();
		}

		nextLine();
	}
}

ObjLoader::~ObjLoader() {
	vertices.clear();
	faces.clear();
}

void ObjLoader::handleFace() {
	Face face;

	face.v1 = parseVertexData(readNextChunk());
	face.v2 = parseVertexData(readNextChunk());
	face.v3 = parseVertexData(readNextChunk());

	faces.push_back(face);
}

void ObjLoader::handleVertex() {
	float x = std::stof(readNextChunk());
	float y = std::stof(readNextChunk());
	float z = std::stof(readNextChunk());

	vertices.push_back({ x, y, z });
}

void ObjLoader::handleTextureCoordinate() {
	float u = std::stof(readNextChunk());
	float v = std::stof(readNextChunk());

	textureCoordinates.push_back({ u, v });
}

/**
 * Attempts to parse the primary vertex index, texture coordinate
 * index, and normal index of a polygonal face. A data chunk can
 * be structured in any of the following ways:
 *
 *   v
 *   v/vt
 *   v/vt/vn
 *   v//vn
 *
 * Where v is the primary index, vt the texture coordinate index,
 * and vn the normal index, with respect to previously listed
 * vertex/texture coordinate/normal values.
 */
VertexData ObjLoader::parseVertexData(std::string chunk) {
	VertexData vertexData;
	int offset = 0;

	for (int i = 0; i < 3; i++) {
		int next = chunk.find("/", offset);
		bool hasNext = next > -1;

		if (next - offset == 0 || offset >= chunk.length()) {
			// If the next '/' is immediately after the last,
			// or we've reached the end of the chunk with
			// cycles to spare, this type of vertex index isn't
			// defined.
			vertexData.indexes[i] = -1;
		} else {
			// As long as characters are found in between the
			// previous '/' and the next, or we still have extra
			// characters in the chunk, attempt to parse the index.
			int len = hasNext ? next : std::string::npos;

			vertexData.indexes[i] = std::stoi(chunk.substr(offset, len));
		}

		offset = hasNext ? next + 1 : chunk.length();
	}

	return vertexData;
}

} // namespace Soft
