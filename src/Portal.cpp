#include "Portal.h"
#include "PhysicsManager.h"
#include "Constants.h"
#include "GameManager.h"

Portal::Portal(Ogre::SceneNode* node, Ogre::SceneManager* sceneMgr){
	m_isActive = false;
	tag = "portal";
	//el nodo que le pasamos al constructor es solo el del collider
	//el nodo padre es el que incluye la entidad y el usaremos
	m_pNode = node->getParentSceneNode();
	m_pRigidbody = PhysicsManager::getSingletonPtr()->createPhysicObject(
		m_pNode, STATIC, PRIMITIVE_BOX, 0, "", true, 0, 0, 0, 
		Ogre::Vector3::ZERO, Ogre::Quaternion::IDENTITY, false);

	m_pRigidbody->setUserPointer(this);

	GameManager::getSingletonPtr()->m_pPortal = this;

	m_pLightNode = m_pNode->createChildSceneNode();

	Ogre::Entity *pEntity = 0;
	pEntity = sceneMgr->createEntity("PortalLight", "PortalLight.mesh");
    pEntity->setCastShadows(false);
    m_pLightNode->attachObject(pEntity);

	m_pLightNode->setVisible(false);
	m_pLightNode->setScale(1,2,1);

}

Portal::~Portal(){
}

void Portal::setActive(bool isActive){ 
	m_isActive = isActive;
	//mostramos el efecto del haz de luz al activar el portal
	m_pLightNode->setVisible(isActive);
}