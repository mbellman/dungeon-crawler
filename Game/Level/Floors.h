#pragma once

#include <Level/BlockBuilder.h>
#include <Level/LevelLayout.h>
#include <MathUtils.h>

namespace Floors {
	namespace {
		static int UP = MathUtils::Direction::UP;
		static int DOWN = MathUtils::Direction::DOWN;
		static int LEFT = MathUtils::Direction::LEFT;
		static int RIGHT = MathUtils::Direction::RIGHT;
	}

	Block Floor1[3][5][5] = {
		// Layer 1
		{
			// Row 1
			{
				{ BlockTypes::GROUND, 0 },
				{ BlockTypes::GROUND, 0 },
				{ BlockTypes::GROUND, 0 },
				{ BlockTypes::GROUND, 0 },
				{ BlockTypes::GROUND, 0 }
			},
			// Row 2
			{
				{ BlockTypes::GROUND, 0 },
				{ BlockTypes::GROUND, 0 },
				{ BlockTypes::GROUND, 0 },
				{ BlockTypes::GROUND, 0 },
				{ BlockTypes::GROUND, 0 }
			},
			// Row 3
			{
				{ BlockTypes::GROUND, 0 },
				{ BlockTypes::GROUND, 0 },
				{ BlockTypes::GROUND, 0 },
				{ BlockTypes::GROUND, 0 },
				{ BlockTypes::GROUND, 0 }
			},
			// Row 4
			{
				{ BlockTypes::GROUND, 0 },
				{ BlockTypes::GROUND, 0 },
				{ BlockTypes::GROUND, 0 },
				{ BlockTypes::GROUND, 0 },
				{ BlockTypes::GROUND, 0 }
			},
			// Row 5
			{
				{ BlockTypes::GROUND, 0 },
				{ BlockTypes::GROUND, 0 },
				{ BlockTypes::GROUND, 0 },
				{ BlockTypes::GROUND, 0 },
				{ BlockTypes::GROUND, 0 }
			}
		},
		// Layer 2
		{
			// Row 1
			{
				{ BlockTypes::WALL_LEFT, UP | DOWN | LEFT | RIGHT },
				{ BlockTypes::WALL_FRONT, UP | DOWN | LEFT | RIGHT },
				{ BlockTypes::WALL_FRONT, UP | DOWN | LEFT | RIGHT },
				{ BlockTypes::WALL_FRONT, UP | DOWN | LEFT | RIGHT },
				{ BlockTypes::WALL_RIGHT, UP | DOWN | LEFT | RIGHT }
			},
			// Row 2
			{
				{ BlockTypes::WALL_LEFT, UP | DOWN | LEFT | RIGHT },
				{ BlockTypes::EMPTY, UP | DOWN | LEFT | RIGHT },
				{ BlockTypes::EMPTY, UP | DOWN | LEFT | RIGHT },
				{ BlockTypes::EMPTY, UP | DOWN | LEFT | RIGHT },
				{ BlockTypes::WALL_RIGHT, UP | DOWN | LEFT | RIGHT }
			},
			// Row 3
			{
				{ BlockTypes::WALL_LEFT, UP | DOWN | LEFT | RIGHT },
				{ BlockTypes::EMPTY, UP | DOWN | LEFT | RIGHT },
				{ BlockTypes::EMPTY, UP | DOWN | LEFT | RIGHT },
				{ BlockTypes::EMPTY, UP | DOWN | LEFT | RIGHT },
				{ BlockTypes::WALL_RIGHT, UP | DOWN | LEFT | RIGHT }
			},
			// Row 4
			{
				{ BlockTypes::WALL_LEFT, UP | DOWN | LEFT | RIGHT },
				{ BlockTypes::EMPTY, UP | DOWN | LEFT | RIGHT },
				{ BlockTypes::EMPTY, UP | DOWN | LEFT | RIGHT },
				{ BlockTypes::EMPTY, UP | DOWN | LEFT | RIGHT },
				{ BlockTypes::WALL_RIGHT, UP | DOWN | LEFT | RIGHT }
			},
			// Row 5
			{
				{ BlockTypes::WALL_LEFT, UP | DOWN | LEFT | RIGHT },
				{ BlockTypes::WALL_BACK, UP | DOWN | LEFT | RIGHT },
				{ BlockTypes::WALL_BACK, UP | DOWN | LEFT | RIGHT },
				{ BlockTypes::WALL_BACK, UP | DOWN | LEFT | RIGHT },
				{ BlockTypes::WALL_RIGHT, UP | DOWN | LEFT | RIGHT }
			}
		},
		// Layer 3
		{
			// Row 1
			{
				{ BlockTypes::CEILING, 0 },
				{ BlockTypes::CEILING, 0 },
				{ BlockTypes::CEILING, 0 },
				{ BlockTypes::CEILING, 0 },
				{ BlockTypes::CEILING, 0 }
			},
			// Row 2
			{
				{ BlockTypes::CEILING, 0 },
				{ BlockTypes::CEILING, 0 },
				{ BlockTypes::CEILING, 0 },
				{ BlockTypes::CEILING, 0 },
				{ BlockTypes::CEILING, 0 }
			},
			// Row 3
			{
				{ BlockTypes::CEILING, 0 },
				{ BlockTypes::CEILING, 0 },
				{ BlockTypes::CEILING, 0 },
				{ BlockTypes::CEILING, 0 },
				{ BlockTypes::CEILING, 0 }
			},
			// Row 4
			{
				{ BlockTypes::CEILING, 0 },
				{ BlockTypes::CEILING, 0 },
				{ BlockTypes::CEILING, 0 },
				{ BlockTypes::CEILING, 0 },
				{ BlockTypes::CEILING, 0 }
			},
			// Row 5
			{
				{ BlockTypes::CEILING, 0 },
				{ BlockTypes::CEILING, 0 },
				{ BlockTypes::CEILING, 0 },
				{ BlockTypes::CEILING, 0 },
				{ BlockTypes::CEILING, 0 }
			}
		}
	};
}
