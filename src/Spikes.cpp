#include "Spikes.h"
#include "PhysicsManager.h"
#include "Constants.h"

Spikes::Spikes(Ogre::SceneNode *node){
	m_pNode = node;
	tag = "spikes";
	
	m_pRigidbody = PhysicsManager::getSingletonPtr()->createPhysicObject(node,
		STATIC, PRIMITIVE_BOX, 0, "", true, 0,0,0,  Ogre::Vector3::ZERO, 
		Ogre::Quaternion::IDENTITY, false);

	m_pRigidbody->setUserPointer(this);

	//anadimos el elemento al final del vector..
	m_trapsList.push_front(this);
	//y guardamos el iterador al elemento que acabamos de crear
	it = m_trapsList.begin();
}

Spikes::~Spikes(){
}
