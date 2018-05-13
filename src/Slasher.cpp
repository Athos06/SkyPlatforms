#include "Slasher.h"
#include "PhysicsManager.h"
#include "Constants.h"
#include "AdvancedOgreFramework.h"

Slasher::Slasher(Ogre::SceneNode *node){
	tag = "slasher"; 
	m_pNode = node;
	//crea el collider con el rigidbody asociado
	m_pRigidbody = PhysicsManager::getSingletonPtr()->createPhysicObject(node,
		KINEMATIC, COLLISION_MESH , 0, "slasherCollider.mesh" , true, 0,0,0,  Ogre::Vector3::ZERO, 
		Ogre::Quaternion::IDENTITY, true);
	//inicializo a mi motionstate propio
	m_pMotionState = static_cast<BtOgre::RigidBodyState*>
		(m_pRigidbody->getMotionState());	

	m_pRigidbody->setUserPointer(this);

	//anadimos el elemento al final del vector..
	m_trapsList.push_front(this);
	//y guardamos el iterador al elemento que acabamos de crear
	it = m_trapsList.begin();

	//creo el efecto del trail
	Ogre::SceneManager* sceneMgr = OgreFramework::getSingletonPtr()->m_pActiveSceneMgr;

	Ogre::SceneNode* trailNode = static_cast<Ogre::SceneNode*>
		(node->getChild(node->getName()+"Trail"));
	
	Ogre::RibbonTrail* trail = sceneMgr->createRibbonTrail
		(trailNode->getName()+"trail");
	trail->setMaterialName("Trail");
	trail->setTrailLength(5);
	trail->setMaxChainElements(50);
	trail->setInitialColour(0, 1, 1, 1, 1);
	trail->setColourChange(0, 0, 0, 0.0, 1);
	trail->setInitialWidth(0, 1.4);
	trail->setWidthChange(0, 2);

	trail->addNode(trailNode);
	sceneMgr->getRootSceneNode()->attachObject(trail);
}

Slasher::~Slasher(){
}

void Slasher::update(double deltaTime){
	
	m_pMotionState->setKinematicPos(
	btTransform( BtOgre::Convert::toBullet(m_pNode->_getDerivedOrientation()), 
	BtOgre::Convert::toBullet(m_pNode->_getDerivedPosition()) ));
}

