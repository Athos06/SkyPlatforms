#include "Switch.h"
#include "PhysicsManager.h"
#include "Constants.h"
#include "GameManager.h"
#include "LevelsManager.h"
#include "AudioManager.h"

Switch::Switch(Ogre::SceneNode* node, Ogre::SceneManager* sceneMgr){
	m_isActive = false;
	tag = "switch";
	//el nodo que le pasamos al constructor es solo el del collider
	//el nodo padre es el que incluye la entidad y el usaremos
	m_pNode = node->getParentSceneNode();
	m_pRigidbody = PhysicsManager::getSingletonPtr()->createPhysicObject(
		m_pNode, STATIC, PRIMITIVE_BOX, 0, "", true, 0, 0, 0, 
		Ogre::Vector3::ZERO, Ogre::Quaternion::IDENTITY, false);

	m_pRigidbody->setUserPointer(this);

	m_pLightNode = m_pNode->createChildSceneNode();
	
	Ogre::Entity *pEntity = 0;
	//creamos la entidad del efecto de luz del portal activado
	pEntity = sceneMgr->createEntity("SwitchLight_" + m_pNode->getName(), "PortalLight.mesh");
    pEntity->setCastShadows(false);
	pEntity->setMaterialName("SwitchLight");
    m_pLightNode->attachObject(pEntity);

	m_pLightNode->setVisible(false);

	//el boton del interruptor, para moverlo cuando se active
	m_pSwitchButton = dynamic_cast<Ogre::SceneNode*>(m_pNode->getChild(m_pNode->getName()+"Button"));

}

Switch::~Switch(){
}

void Switch::setActive(bool isActive){ 
	if(isActive){
		//ejecutamos sonido de activacion
		AudioManager::getSingletonPtr()->playFX("PickedStone", 0, PICKUP_CHANNEL);
		//incrementamos el contador de interruptores activados
		LevelsManager::getSingletonPtr()->getLevelProperties()
					->increaseCounter();
		//desplazamos el boton hacia abajo para que se muestre como activado
		m_pSwitchButton->translate(0, -0.2, 0);
	}
	m_isActive = isActive;
	//mostramos el efecto de luz del boton activado
	m_pLightNode->setVisible(isActive);
}