#pragma once

#include <SoftEngine.h>

class BlockBuilder {
public:
	static Soft::Object* createBlockObject(int blockType);

private:
	static Soft::Mesh* createSingleBlockFace();
};
