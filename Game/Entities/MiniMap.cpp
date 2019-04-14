#include <Entities/MiniMap.h>
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
}

void MiniMap::initialize() {
	const Soft::Area& levelSize = levelLayout->getSize();

	mapSize.width = levelSize.width * MiniMap::TILE_SIZE;
	mapSize.height = levelSize.height * MiniMap::TILE_SIZE;

	for (int i = 0; i < levelLayout->getTotalLayers(); i++) {
		createLayerMap(i);
	}
}

void MiniMap::onUpdate(int dt) {
	updateClippingRegion();

	int currentLayer = player->getCurrentGridPosition().layer;

	for (int i = 0; i < layerMaps.size(); i++) {
		if (i == currentLayer) {
			layerMaps[i]->setAlpha(0.75f);
			layerMaps[i]->clip(clippingRegion);
		} else {
			layerMaps[i]->setAlpha(0.0f);
		}
	}
}

void MiniMap::setColorBufferTileColor(Soft::ColorBuffer* colorBuffer, int x, int z, const Soft::Color& color) {
	int px = x * MiniMap::TILE_SIZE;
	int py = z * MiniMap::TILE_SIZE;

	for (int i = 0; i < MiniMap::TILE_SIZE; i++) {
		for (int j = 0; j < MiniMap::TILE_SIZE; j++) {
			if (i == 0 || j == 0) {
				colorBuffer->write(px + j, py + i, MiniMap::GRID_LINE_COLOR.R, MiniMap::GRID_LINE_COLOR.G, MiniMap::GRID_LINE_COLOR.B);
			} else {
				colorBuffer->write(px + j, py + i, color.R, color.G, color.B);
			}
		}
	}
}

void MiniMap::createLayerMap(int layerIndex) {
	int width = levelLayout->getSize().width * MiniMap::TILE_SIZE;
	int height = levelLayout->getSize().height * MiniMap::TILE_SIZE;
	Soft::ColorBuffer* layerMapBuffer = new Soft::ColorBuffer(width, height);

	for (int z = 0; z < height; z++) {
		for (int x = 0; x < width; x++) {
			GridPosition position = { layerIndex, x, z };

			if (levelLayout->isWalkablePosition(position) || levelLayout->isStaircaseBlock(position)) {
				setColorBufferTileColor(layerMapBuffer, x, z, MiniMap::WALKABLE_TILE_COLOR);
			} else {
				setColorBufferTileColor(layerMapBuffer, x, z, MiniMap::SOLID_TILE_COLOR);
			}
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

void MiniMap::updateClippingRegion() {
	const GridPosition& position = player->getCurrentGridPosition();
	const Soft::Area& levelSize = levelLayout->getSize();
	int half_w = clippingRegion.width >> 1;
	int half_h = clippingRegion.height >> 1;
	int halfTileSize = MiniMap::TILE_SIZE >> 1;
	int clipX = position.x * MiniMap::TILE_SIZE - half_w + halfTileSize;
	int clipY = position.z * MiniMap::TILE_SIZE - half_h + halfTileSize;

	clippingRegion.x = std::clamp(clipX, 0, mapSize.width - clippingRegion.width);
	clippingRegion.y = std::clamp(clipY, 0, mapSize.height - clippingRegion.height);
}

Soft::Color MiniMap::WALKABLE_TILE_COLOR = { 100, 100, 100 };
Soft::Color MiniMap::SOLID_TILE_COLOR = { 30, 30, 30 };
Soft::Color MiniMap::GRID_LINE_COLOR = { 0, 0, 0 };
