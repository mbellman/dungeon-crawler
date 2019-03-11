#include <Scenes/GameScene.h>
#include <System/Objects.h>

void GameScene::load() {
	Cube* cube = new Cube(50);

	cube->setColor({ 255, 0, 0 });
	cube->position = { 0, 100, 500 };

	cube->onUpdate = [=](int dt) {
		cube->rotateDeg({ 0.6f, -0.75f, 0.5f });
	};

	add(cube);

	settings.ambientLightColor = { 255, 255, 255 };
	settings.brightness = 0.2f;
}
