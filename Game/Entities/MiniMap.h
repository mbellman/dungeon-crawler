#pragma once

#include <SoftEngine.h>
#include <Level/LevelLayout.h>
#include <Entities/Player.h>
#include <vector>

class MiniMap : public Soft::Entity {
public:
	MiniMap(const LevelLayout* levelLayout, const Player* player);
	~MiniMap();

	void initialize() override;
	void onUpdate(int dt) override;

private:
	constexpr static int TILE_SIZE = 11;
	static Soft::Color WALKABLE_TILE_COLOR;
	static Soft::Color SOLID_TILE_COLOR;
	static Soft::Color GRID_LINE_COLOR;

	const LevelLayout* levelLayout = nullptr;
	const Player* player = nullptr;
	Soft::Area mapSize;
	Soft::Region clippingRegion = { 0, 0, 99, 79 };
	std::vector<Soft::UIGraphic*> layerMaps;
	std::vector<Soft::UIGraphic*> directionalPlayerIcons;

	void addDirectionalPlayerIcons();
	void addLayerMap(int layerIndex);
	int getCurrentLayer();
	int getCurrentPlayerIconIndex();
	void setColorBufferTileColor(Soft::ColorBuffer* colorBuffer, int x, int z, const Soft::Color& color);
};
