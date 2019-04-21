#pragma once

#include <SoftEngine.h>
#include <Level/LevelLoader.h>

template<typename T = EntityData>
class Interactible : public Soft::Entity {
public:
	Interactible(const T& data) {
		this->data = data;
	}

	const T& getData() const {
		return data;
	}

protected:
	T data;
};
