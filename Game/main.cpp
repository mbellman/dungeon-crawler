#include <Scenes/GameScene.h>
#include <Engine.h>
#include <System/Controller.h>

int main(int argc, char* argv[]) {
	Engine* engine = new Engine(1200, 720, "Dungeon Crawler", NULL, NULL, 0);
	Controller* controller = new Controller(engine);

	controller->start(new GameScene());

	delete controller;
	return 0;
}
