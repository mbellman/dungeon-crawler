#pragma once

#include <map>
#include <vector>
#include <string>
#include <SDL.h>
#include <UI/UIObjects.h>

namespace Soft {

/**
 * UI
 * --
 */
class UI {
public:
	UI(SDL_Renderer* renderer);
	~UI();

	void add(UIObject* uiObject);
	void add(std::string key, UIObject* uiObject);
	UIObject* get(std::string key);
	void remove(UIObject* uiObject);
	void remove(std::string key);
	void update(int dt);

protected:
	SDL_Renderer* renderer = NULL;
	std::vector<UIObject*> uiObjects;
	std::map<std::string, UIObject*> uiObjectMap;
};

} // namespace Soft
