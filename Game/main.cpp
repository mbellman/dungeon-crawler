#include <Scenes/GameScene.h>
#include <GameUtils.h>
#include <SoftEngine.h>

int main(int argc, char* argv[]) {
	Soft::Engine* engine = new Soft::Engine(
		GameUtils::WINDOW_SIZE.width, GameUtils::WINDOW_SIZE.height,
		"Dungeon Crawler",
		NULL,
		"./Assets/fonts/FreeMono.ttf",
	 	Soft::Flags::DEBUG_STATS | Soft::Flags::DEBUG_COMMAND_LINE | Soft::Flags::DISABLE_TEXTURE_SAMPLING_INTERVAL | Soft::Flags::PIXEL_FILTER
	);

	engine->lockProportionalRasterRegion(
		GameUtils::RASTER_REGION.x,
		GameUtils::RASTER_REGION.y,
		GameUtils::RASTER_REGION.width,
		GameUtils::RASTER_REGION.height
	);

	Soft::Controller* controller = new Soft::Controller(engine);

	controller->start(new GameScene());

	delete controller;
	return 0;
}
