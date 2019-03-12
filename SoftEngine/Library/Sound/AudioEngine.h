#pragma once

#include <SDL_mixer.h>
#include <al.h>
#include <alc.h>

namespace Soft {

class AudioEngine {
public:
	AudioEngine();
	~AudioEngine();

	void mute();
	void unmute();

private:
	ALCdevice* device = nullptr;
	ALCcontext* context = nullptr;
};

} // namespace Soft
