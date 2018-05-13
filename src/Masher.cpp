#include "Masher.h"
#include "PhysicsManager.h"
#include "Constants.h"

Masher::Masher(Ogre::SceneNode *node){
	tag = "masher";
	m_pNode = node;
	m_pRigidbody = PhysicsManager::getSingletonPtr()->createPhysicObject(node,
		KINEMATIC, PRIMITIVE_BOX, 0, "", true, 0,0,0,  Ogre::Vector3::ZERO, 
		Ogre::Quaternion::IDENTITY, false);
	//inicializo a mi motionstate propio
	m_pMotionState = static_cast<BtOgre::RigidBodyState*>
		(m_pRigidbody->getMotionState());
	//anadimos el elemento al final del vector..
	
	m_pRigidbody->setUserPointer(this);

	//anadimos el elemento al final del vector..
	m_trapsList.push_front(this);
	//y guardamos el iterador al elemento que acabamos de crear
	it = m_trapsList.begin();
}

Masher::~Masher(){
}

void Masher::update(double deltaTime){
	m_pMotionState->setKinematicPos(
	btTransform( BtOgre::Convert::toBullet(m_pNode->_getDerivedOrientation()), 
	BtOgre::Convert::toBullet(m_pNode->_getDerivedPosition()) ));

	//m_pRigidbody->getMotionState()->getWorldTransform(transform);
	//transform.setOrigin(BtOgre::Convert::toBullet(m_pNode->_getDerivedPosition()));
	
}
