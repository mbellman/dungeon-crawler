#include <UI/UI.h>
#include <SDL.h>
#include <algorithm>
#include <string>

namespace Soft {

/**
 * UI
 * --
 */
UI::UI(SDL_Renderer* renderer) {
	this->renderer = renderer;
}

UI::~UI() {
	for (auto* uiObject : uiObjects) {
		delete uiObject;
	}

	uiObjectMap.clear();
	uiObjects.clear();
}

void UI::add(std::string key, UIObject* uiObject) {
	uiObject->setRenderer(renderer);

	uiObjects.push_back(uiObject);
	uiObjectMap.emplace(key, uiObject);
}

UIObject* UI::get(std::string key) {
	const auto& uiObject = uiObjectMap.find(key);

	if (uiObject != uiObjectMap.end()) {
		return uiObject->second;
	}

	return nullptr;
}

void UI::remove(std::string key) {
	const auto& entry = uiObjectMap.find(key);

	if (entry != uiObjectMap.end()) {
		UIObject* uiObject = entry->second;

		uiObjectMap.erase(key);
		uiObjects.erase(std::remove(uiObjects.begin(), uiObjects.end(), uiObject), uiObjects.end());

		delete uiObject;
	}
}

void UI::update(int dt) {
	for(auto* uiObject : uiObjects) {
		uiObject->update(dt);
	}
}

} // namespace Soft
