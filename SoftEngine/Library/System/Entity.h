#pragma once

#include <System/Objects.h>
#include <UI/UIObjects.h>

namespace Soft {

class Entity {
public:
	~Entity();

	virtual void load() = 0;
	virtual void onUpdate(int dt);

protected:
	void add(Object* object);
	void add(UIObject* uiObject);
};

} // namespace Soft
