#include <Party.h>
#include <string>
#include <algorithm>

/**
 * PartyMember
 * -----------
 */
PartyMember::PartyMember(std::string name, int characterId) {
	this->name = name;
	stats.characterId = characterId;
}

void PartyMember::applyStatusEffect(StatusEffects effect) {
	stats.statusEffects |= effect;
}

void PartyMember::removeStatusEffect(StatusEffects effect) {
	stats.statusEffects &= ~effect;
}

float PartyMember::getAttackFactor() const {

}

float PartyMember::getDefenseFactor() const {

}

const std::string& PartyMember::getName() const {
	return name;
}

float PartyMember::getPercentageHP() const {
	return (float)stats.currentHP / stats.maxHP;
}

const PartyMemberStats& PartyMember::getStats() const {
	return stats;
}

void PartyMember::heal(int amount) {
	stats.currentHP = std::min(stats.currentHP + amount, stats.maxHP);
}

bool PartyMember::isAlive() const {
	return stats.currentHP > 0;
}

void PartyMember::takeHit(int amount) {
	int damage = (int)(amount * (1.0f - getDefenseFactor()));

	stats.currentHP = std::max(stats.currentHP - damage, 0);
}

void PartyMember::update() {

}

/**
 * Party
 * -----
 */
Party::~Party() {
	for (auto* member : members) {
		delete member;
	}

	members.clear();
}

void Party::addMember(PartyMember* member) {
	members.push_back(member);
}

PartyMember* Party::getMemberByIndex(int index) const {
	return members[index];
}

const std::vector<PartyMember*>& Party::getMembers() const {
	return members;
}

int Party::getSize() const {
	return members.size();
}

void Party::update() {
	for (auto* member : members) {
		member->update();
	}
}
