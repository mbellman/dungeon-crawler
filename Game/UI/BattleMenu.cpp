#include <UI/BattleMenu.h>
#include <UI/Frame.h>

/**
 * BattleMenu
 * ----------
 */
BattleMenu::BattleMenu() {

}

void BattleMenu::initialize() {
	Frame* frame = new Frame({ 0, 612, 1200, 108 });

	add(frame);
}
