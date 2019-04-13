#pragma once

#include <string>
#include <vector>

enum StatusEffects {
	ATTACK_UP = 1 << 0,
	ATTACK_DOWN = 1 << 1,
	DEFENSE_UP = 1 << 2,
	DEFENSE_DOWN = 1 << 3,
	POISON = 1 << 4
};

struct PartyMemberStats {
	int characterId = 0;
	int currentHP = 100;
	int maxHP = 100;
	int statusEffects = 0;
	int weaponType = -1;
	int armorType = -1;
};

class PartyMember {
public:
	PartyMember(std::string name);

	void applyStatusEffect(StatusEffects effect);
	float getAttackFactor() const;
	float getDefenseFactor() const;
	float getPercentageHP() const;
	const PartyMemberStats& getStats() const;
	void heal(int amount);
	bool isAlive() const;
	void removeStatusEffect(StatusEffects effect);
	void takeHit(int amount);
	void update();

private:
	std::string name;
	PartyMemberStats stats;
};

class Party {
public:
	~Party();

	void addMember(PartyMember* member);
	PartyMember* getMemberByIndex(int index) const;
	int getSize() const;
	void update();

private:
	std::vector<PartyMember*> members;
};
