#include "Skystone.h"
#include "PhysicsManager.h"
#include "Constants.h"
#include "ParticlesEffect.h"
#include "Animations.h"
#include "AnimationLoader.h"

Skystone::Skystone(){
	m_pNode = 0;
	tag = "skystone";
	m_pRigidbody = 0;
	m_pAnimation = 0;
}

Skystone::Skystone(Ogre::SceneNode *node){

	m_pNode = node;
	tag = "skystone";
	//llamamos al physics manager para crear el objeto fisico
	m_pRigidbody = PhysicsManager::getSingletonPtr()->createPhysicObject(
		m_pNode, STATIC, PRIMITIVE_SPHERE, 0, "", true, 0, 0, 0, 
		Ogre::Vector3::ZERO, Ogre::Quaternion::IDENTITY, false);

	m_pRigidbody->setUserPointer(this);

	m_collectiblesList.push_front(this);
	it = m_collectiblesList.begin();



	Ogre::SceneNode* parentNode = m_pNode->getParentSceneNode();
	Ogre::SceneManager* sceneMgr = parentNode->getCreator();
	
	//carga la animacion de rotacion de la skystone
	AnimationLoader animLoader;
	//se necesita guardar el puntero a la animacion para
	//poder destruirla junto a la skystone cuando esta
	//es destruida al recogerse
	m_pAnimation = animLoader.parseAnimation("DeathMillAnimation.anim", "General", parentNode, sceneMgr,
		false, 1.2, 0);


}

Skystone::~Skystone(){
	m_pNode = 0;
	m_pRigidbody = 0;	
	m_pAnimation = 0;
}

void Skystone::destroy(){
	Ogre::SceneManager* sceneMgr = m_pNode->getCreator();
	Ogre::SceneNode* parent = m_pNode->getParentSceneNode();

	//primero destruyo la animacion, luego los nodos
	m_pAnimation->destroy();

	Ogre::Vector3 pos = m_pNode->_getDerivedPosition();
	//hay que destruir tanto el nodo propio como el padre,
	//puesto que en este nodo se encuentra solo el collider
	//y en el padre es donde se encuentra la entidad grafica
	sceneMgr->destroySceneNode(m_pNode);
	sceneMgr->destroySceneNode(parent);

	PhysicsManager::getSingletonPtr()->destroyPhysicObject(m_pRigidbody);
	m_pNode = 0;
	m_pRigidbody = 0;	

	new ParticlesEffect(0.5, "PickUpStone", pos);

	m_collectiblesList.erase(it);
}
