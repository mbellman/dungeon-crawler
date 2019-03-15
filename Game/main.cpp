#include <Scenes/GameScene.h>
#include <GameConstants.h>
#include <SoftEngine.h>

int main(int argc, char* argv[]) {
	Soft::Engine* engine = new Soft::Engine(
		1200, 720,
		"Dungeon Crawler",
		NULL,
		"./Assets/fonts/FreeMono.ttf",
		Soft::Flags::DEBUG_STATS | Soft::Flags::DEBUG_COMMAND_LINE
	);

	engine->lockProportionalRasterRegion(
		GameConstants::RASTER_REGION.x,
		GameConstants::RASTER_REGION.y,
		GameConstants::RASTER_REGION.width,
		GameConstants::RASTER_REGION.height
	);

	Soft::Controller* controller = new Soft::Controller(engine);

	controller->start(new GameScene());

	delete controller;
	return 0;
}
