#include "DeathMill.h"
#include "PhysicsManager.h"
#include "Constants.h"
#include "AdvancedOgreFramework.h"

DeathMill::DeathMill(Ogre::SceneNode *node){
	tag = "deathmill";
	m_pNode = node;
	//el nodo del deathmill tiene otros tres nodos hijos, cada uno utilizado 
	//para cada collider o cada brazo del molino
	Ogre::SceneNode::ChildNodeIterator nodeIterator = 
		m_pNode->getChildIterator();

	Ogre::SceneNode* provNode;
	btRigidBody* tempRigidbody;
	int i = 0;
	while (nodeIterator.hasMoreElements())
	{
		//EXTRA CUIDADO AQUI, SI TOCO EL ITERADOR LA PUEDO LIAR PARDA
		provNode = dynamic_cast<Ogre::SceneNode*>(nodeIterator.getNext());
		m_armsNodes.push_back(provNode);
		
	}	

	Ogre::SceneManager* sceneMgr = OgreFramework::getSingletonPtr()->m_pActiveSceneMgr;

	for(int i = 0; i < m_armsNodes.size(); i++){
		tempRigidbody = PhysicsManager::getSingletonPtr()->createPhysicObject(m_armsNodes[i],
			KINEMATIC, PRIMITIVE_BOX, 0, "", true, 0,0,0,  Ogre::Vector3::ZERO, 
			Ogre::Quaternion::IDENTITY, false);
		//anado el rigidbody de cada brazo
		m_multipleRigidBodies.push_back(tempRigidbody);
		//inicializo a mi motionstate propio de cada brazo
		m_multipleMotionStates.push_back(
			static_cast<BtOgre::RigidBodyState*>(
			tempRigidbody->getMotionState()) );

		tempRigidbody->setUserPointer(this);


		Ogre::SceneNode* trailNode = static_cast<Ogre::SceneNode*>(m_armsNodes[i]->getChild("Trail"+m_armsNodes[i]->getName()));
		

		
		Ogre::RibbonTrail* trail = sceneMgr->createRibbonTrail
			(trailNode->getName()+"trail");
		trail->setMaterialName("Trail");
		trail->setTrailLength(8);
		trail->setMaxChainElements(30);
		trail->setInitialColour(0, 0.7, 0.9, 1, 1);
		trail->setColourChange(0, 0, 0, 0.0, 1);
		trail->setInitialWidth(0, 0.4);
		trail->setWidthChange(0, 0.4);

		trail->addNode(trailNode);
		sceneMgr->getRootSceneNode()->attachObject(trail);
		
	}

	//anadimos el elemento al final del vector..
	m_trapsList.push_front(this);
	//y guardamos el iterador al elemento que acabamos de crear
	it = m_trapsList.begin();
}

DeathMill::~DeathMill(){
}

void DeathMill::update(double deltaTime){
	Ogre::Quaternion pos;

	//actualizo el motion state de cada brazo/aspa
	for(int i = 0; i < m_armsNodes.size(); i++){
		m_multipleMotionStates[i]->setKinematicPos(
			btTransform( BtOgre::Convert::toBullet((m_armsNodes[i]->_getDerivedOrientation())), 
			BtOgre::Convert::toBullet(m_armsNodes[i]->_getDerivedPosition()) ));
	}
}

