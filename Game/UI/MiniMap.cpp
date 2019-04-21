#include <UI/MiniMap.h>
#include <GameUtils.h>
#include <MathUtils.h>
#include <SoftEngine.h>
#include <algorithm>

/**
 * MiniMap
 * -------
 */
MiniMap::MiniMap(const LevelLayout* levelLayout, const Player* player) {
	this->levelLayout = levelLayout;
	this->player = player;
}

MiniMap::~MiniMap() {
	layerMaps.clear();
	directionalPlayerIcons.clear();
}

void MiniMap::initialize() {
	const Soft::Area& levelSize = levelLayout->getSize();

	mapSize.width = levelSize.width * MiniMap::TILE_SIZE;
	mapSize.height = levelSize.height * MiniMap::TILE_SIZE;

	for (int i = 0; i < levelLayout->getTotalLayers(); i++) {
		addLayerMap(i);
	}

	addDirectionalPlayerIcons();
}

void MiniMap::onUpdate(int dt) {
	// Recalculate the map clipping region x/y coordinates
	const Soft::Vec3& playerPosition = player->getCurrentPosition();
	const Soft::Area& levelSize = levelLayout->getSize();
	float xPercent = playerPosition.x / (levelSize.width * GameUtils::TILE_SIZE);
	float zPercent = -playerPosition.z / (levelSize.height * GameUtils::TILE_SIZE);
	int half_w = clippingRegion.width >> 1;
	int half_h = clippingRegion.height >> 1;
	int halfTileSize = MiniMap::TILE_SIZE >> 1;
	int clipX = xPercent * mapSize.width - half_w + halfTileSize;
	int clipY = zPercent * mapSize.height - half_h + halfTileSize;
	int maxClipX = mapSize.width - clippingRegion.width;
	int maxClipY = mapSize.height - clippingRegion.height;

	clippingRegion.x = std::clamp(clipX, 0, maxClipX);
	clippingRegion.y = std::clamp(clipY, 0, maxClipY);

	// Show only the current layer map
	int currentLayer = player->getCurrentGridPosition().layer;

	for (int i = 0; i < layerMaps.size(); i++) {
		if (i == currentLayer) {
			layerMaps[i]->setAlpha(0.75f);
			layerMaps[i]->clip(clippingRegion);
		} else {
			layerMaps[i]->setAlpha(0.0f);
		}
	}

	// Update the directional player icon and its position
	int currentIndex = getCurrentPlayerIconIndex();

	Soft::Coordinate centerOffset = {
		clipX < 0 ? clipX : clipX > maxClipX ? clipX - maxClipX : 0,
		clipY < 0 ? clipY : clipY > maxClipY ? clipY - maxClipY : 0
	};

	Soft::Coordinate playerIconPosition = {
		44 + half_w - halfTileSize + 1 + centerOffset.x,
		628 + half_h - halfTileSize + 1 + centerOffset.y
	};

	for (int i = 0; i < directionalPlayerIcons.size(); i++) {
		Soft::UIGraphic* icon = directionalPlayerIcons[i];

		if (i == currentIndex) {
			icon->setAlpha(0.5f);
			icon->setSize(MiniMap::TILE_SIZE - 1, MiniMap::TILE_SIZE - 1);
			icon->position = playerIconPosition;
		} else {
			icon->setAlpha(0.0f);
		}
	}
}

void MiniMap::addDirectionalPlayerIcons() {
	Soft::UIGraphic* icon1 = new Soft::UIGraphic("./Assets/UI/MiniMap/player-forward.png");
	Soft::UIGraphic* icon2 = new Soft::UIGraphic("./Assets/UI/MiniMap/player-left.png");
	Soft::UIGraphic* icon3 = new Soft::UIGraphic("./Assets/UI/MiniMap/player-backward.png");
	Soft::UIGraphic* icon4 = new Soft::UIGraphic("./Assets/UI/MiniMap/player-right.png");

	directionalPlayerIcons.push_back(icon1);
	directionalPlayerIcons.push_back(icon2);
	directionalPlayerIcons.push_back(icon3);
	directionalPlayerIcons.push_back(icon4);

	add(icon1);
	add(icon2);
	add(icon3);
	add(icon4);
}

void MiniMap::addLayerMap(int layerIndex) {
	const Soft::Area& levelSize = levelLayout->getSize();
	Soft::ColorBuffer* layerMapBuffer = new Soft::ColorBuffer(mapSize.width, mapSize.height);

	for (int z = 0; z < levelSize.height; z++) {
		for (int x = 0; x < levelSize.width; x++) {
			GridPosition position = { layerIndex, x, z };
			Soft::Color tileColor = getTileColor(position);

			setColorBufferTileColor(layerMapBuffer, x, z, tileColor);
		}
	}

	Soft::UIGraphic* layerMap = new Soft::UIGraphic(layerMapBuffer);
	layerMap->position = { 44, 628 };

	layerMaps.push_back(layerMap);
	add(layerMap);

	delete layerMapBuffer;
}

int MiniMap::getCurrentLayer() {
	return player->getCurrentGridPosition().layer;
}

int MiniMap::getCurrentPlayerIconIndex() {
	switch (player->getDirection()) {
		case MathUtils::Direction::FORWARD:
			return 0;
		case MathUtils::Direction::LEFT:
			return 1;
		case MathUtils::Direction::BACKWARD:
			return 2;
		case MathUtils::Direction::RIGHT:
			return 3;
		default:
			return 0;
	}
}

Soft::Color MiniMap::getTileColor(const GridPosition& position) {
	if (levelLayout->isDesecrated(position)) {
		return MiniMap::DESECRATED_TILE_COLOR;
	} else if (levelLayout->isWalkablePosition(position) || levelLayout->isStaircaseBlock(position)) {
		return MiniMap::WALKABLE_TILE_COLOR;
	} else {
		return MiniMap::SOLID_TILE_COLOR;
	}
}

void MiniMap::setColorBufferTileColor(Soft::ColorBuffer* colorBuffer, int x, int z, const Soft::Color& color) {
	int px = x * MiniMap::TILE_SIZE;
	int py = z * MiniMap::TILE_SIZE;

	for (int i = 0; i < MiniMap::TILE_SIZE; i++) {
		for (int j = 0; j < MiniMap::TILE_SIZE; j++) {
			const Soft::Color& pixelColor = i == 0 || j == 0
				? MiniMap::GRID_LINE_COLOR
				: color;

			colorBuffer->write(px + j, py + i, pixelColor.R, pixelColor.G, pixelColor.B);
		}
	}
}

Soft::Color MiniMap::WALKABLE_TILE_COLOR = { 100, 100, 100 };
Soft::Color MiniMap::DESECRATED_TILE_COLOR = { 150, 30, 30 };
Soft::Color MiniMap::SOLID_TILE_COLOR = { 30, 30, 30 };
Soft::Color MiniMap::GRID_LINE_COLOR = { 0, 0, 0 };
