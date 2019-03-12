#include <Scenes/GameScene.h>
#include <SoftEngine.h>

int main(int argc, char* argv[]) {
	Soft::Engine* engine = new Soft::Engine(1200, 720, "Dungeon Crawler", NULL, NULL, 0);
	Soft::Controller* controller = new Soft::Controller(engine);

	controller->start(new GameScene());

	delete controller;
	return 0;
}
