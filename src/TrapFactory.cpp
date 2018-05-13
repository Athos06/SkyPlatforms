#include "TrapFactory.h"
#include "Trap.h"
#include "Masher.h"
#include "Slasher.h"
#include "DeathMill.h"
#include "Spikes.h"

TrapFactory::TrapFactory(){}

TrapFactory::~TrapFactory(){}

Trap* TrapFactory::createDeathMill(Ogre::SceneNode* node){
	return new DeathMill(node);
}

Trap* TrapFactory::createMasher(Ogre::SceneNode* node){
	return new DeathMill(node);
}

Trap* TrapFactory::createSlasher(Ogre::SceneNode* node){
	return new Slasher(node);
}

Trap* TrapFactory::createSpikes(Ogre::SceneNode* node){
	return new Spikes(node);
}