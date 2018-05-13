#include "CheckPoint.h"
#include "PhysicsManager.h"
#include "GameManager.h"
#include "Constants.h"

CheckPoint::CheckPoint(Ogre::SceneNode* node){
	m_pNode = node;
	tag = "checkpoint";

	//crea el collider del checkpoint en el que al entrar activamos
	//dicho checkpoint
	m_pRigidbody = PhysicsManager::getSingletonPtr()->createPhysicObject(node,
		STATIC, PRIMITIVE_BOX, 0, "", true, 0,0,0,  Ogre::Vector3::ZERO, 
		Ogre::Quaternion::IDENTITY, false);

	m_pRigidbody->setUserPointer(this);
 
	Ogre::String spname = "SpawnPoint" + m_pNode->getName();
	//obtenemos el spawnpoint que esta como nodo hijo del nodo del checkpoint
	m_pSpawnPoint = dynamic_cast<Ogre::SceneNode*>(m_pNode->getChild(spname));
}

CheckPoint::~CheckPoint(){
}

void CheckPoint::setDefaultCheckPoint(){
	//establecemos el spawnPoint de inicio del nivel
	GameManager::getSingletonPtr()->setDefaultSpawnPoint(m_pSpawnPoint);
}

