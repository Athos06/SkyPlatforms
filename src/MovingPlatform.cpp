#include "MovingPlatform.h"
#include "Constants.h"
#include "PhysicsManager.h"
#include "AdvancedOgreFramework.h"

std::list<MovingPlatform*> MovingPlatform::m_platformsList;
std::vector<std::list<MovingPlatform*>::iterator> MovingPlatform::m_deletionList;

void MovingPlatform::updateEach(double deltaTime){
	if(!m_deletionList.empty()){
		for(int i = 0; i < m_deletionList.size(); i++){
			m_platformsList.erase(m_deletionList[i]);
		}
		m_deletionList.clear();
	}
	
	if(m_platformsList.empty())
		return;

	std::list<MovingPlatform*>::iterator aux;
	for(aux = m_platformsList.begin();
		aux != m_platformsList.end(); aux++){
		(*aux)->update(deltaTime);
	}
}

void MovingPlatform::destroyAllPlatforms(){
	m_deletionList.clear();
	if(m_platformsList.empty())
		return;

	std::list<MovingPlatform*>::iterator aux;
	for(aux = m_platformsList.begin();
		aux != m_platformsList.end(); aux++){
		delete (*aux);
	}

	m_platformsList.clear();
}


MovingPlatform::MovingPlatform(Ogre::SceneNode *node){
	tag = "platform";
	m_pNode = node;
	direction = 0;

	m_pRigidbody = PhysicsManager::getSingletonPtr()->createPhysicObject(node,
		KINEMATIC, PRIMITIVE_BOX, 0, "" , false, 0,0,0,  Ogre::Vector3::ZERO, 
		Ogre::Quaternion::IDENTITY, false);

	//inicializo a mi motionstate propio
	m_pMotionState = static_cast<BtOgre::RigidBodyState*>(m_pRigidbody->getMotionState());
	m_pRigidbody->setUserPointer(this);

	//anadimos el elemento al final del vector..
	m_platformsList.push_front(this);
	//y guardamos el iterador al elemento que acabamos de crear
	it = m_platformsList.begin();

	lastPos = m_pNode->_getDerivedPosition();
}

MovingPlatform::~MovingPlatform(){
	m_pMotionState = 0;
	m_pRigidbody = 0;
}

void MovingPlatform::update(double deltaTime){
	Ogre::Vector3 pos = m_pNode->_getDerivedPosition();
	currentPos = pos;
	//la interpolacion de objetos kinematic en bullet
	//da errores, y hay un cierto retraso, con esta pequena chapuza
	//soluciona ese problema provisionalmente
	pos += 2 * (currentPos - lastPos);
	lastPos = currentPos;
	//actualizamos el motion state del objeto kinematic
	m_pMotionState->setKinematicPos(btTransform( 
		BtOgre::Convert::toBullet(m_pNode->_getDerivedOrientation()), 
		BtOgre::Convert::toBullet(pos)) );//(m_pNode->_getDerivedPosition()) ));
 
}