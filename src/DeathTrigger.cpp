#include "DeathTrigger.h"

#include "PhysicsManager.h"
#include "Constants.h"

DeathTrigger::DeathTrigger(Ogre::SceneNode* node){
	tag = "deathTrigger";
	m_pNode = node;
	//crea el collider del deathTrigger
	m_pRigidbody = PhysicsManager::getSingletonPtr()->createPhysicObject(
		m_pNode, STATIC, PRIMITIVE_BOX, 0, "", true, 0, 0, 0, 
		Ogre::Vector3::ZERO, Ogre::Quaternion::IDENTITY, false);

	m_pRigidbody->setUserPointer(this);
}

DeathTrigger::~DeathTrigger(){
	m_pNode = 0;
	m_pRigidbody = 0;
}
