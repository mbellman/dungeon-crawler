#include <Scenes/GameScene.h>
#include <GameUtils.h>
#include <SoftEngine.h>

int main(int argc, char* argv[]) {
	using namespace Soft;
	using namespace GameUtils;

	Engine* engine = new Engine(
		WINDOW_SIZE.width, WINDOW_SIZE.height,
		"Dungeon Crawler",
		NULL,
		"./Assets/fonts/FreeMono.ttf",
	 	Flags::DISABLE_WINDOW_RESIZE | Flags::DEBUG_STATS | Flags::DEBUG_COMMAND_LINE | Flags::DISABLE_TEXTURE_SAMPLING_INTERVAL | Flags::PIXEL_FILTER
	);

	engine->lockProportionalRasterRegion(
		RASTER_REGION.x,
		RASTER_REGION.y,
		RASTER_REGION.width,
		RASTER_REGION.height
	);

	Controller* controller = new Controller(engine);

	controller->start(new GameScene());

	delete controller;
	return 0;
}
