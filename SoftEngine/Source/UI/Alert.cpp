#include <UI/Alert.h>
#include <SDL.h>

namespace Soft {

void Alert::error(const char* title, const char* message) {
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, title, message, NULL);
}

void Alert::warning(const char* title, const char* message) {
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, title, message, NULL);
}

} // namespace Soft
