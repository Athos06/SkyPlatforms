//|||||||||||||||||||||||||||||||||||||||||||||||||||

#include "CharacterController.h"

#include "BtOgrePG.h"
#include "BtOgreGP.h"
#include "BtOgreExtras.h"

#include "btBulletDynamicsCommon.h"
#include "BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"

#include <BulletDynamics/Character/btCharacterControllerInterface.h>
#include "CharacterControllerPhysics.h"

#include "PhysicsManager.h"
#include "Player.h"
#include "GameObject.h"
//|||||||||||||||||||||||||||||||||||||||||||||||||||

CharacterController::CharacterController(){
	m_character = 0;
	m_IsOnPlatform = false;
}
//|||||||||||||||||||||||||||||||||||||||||||||||||||

CharacterController::~CharacterController(){
	if(m_character)
		delete m_character;

}
//|||||||||||||||||||||||||||||||||||||||||||||||||||

void CharacterController::setupCharacterController(Ogre::SceneNode* node, Player* player){
	velocityPlatform = BtOgre::Convert::toBullet(Ogre::Vector3::ZERO);

	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setOrigin(BtOgre::Convert::toBullet(node->getPosition()));
	//startTransform.setOrigin (btVector3(5,15,0));
	m_ghostObject = new btPairCachingGhostObject();
	m_ghostObject->setWorldTransform(startTransform);
	//sweepBP->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());
	btScalar characterHeight=1.0;
	btScalar characterWidth =0.25;
	btVector3 size = btVector3(0.15, 0.85, 0.15);
	btConvexShape* capsule = new btBoxShape(size);
	m_ghostObject->setCollisionShape (capsule);
	m_ghostObject->setCollisionFlags (btCollisionObject::CF_CHARACTER_OBJECT | 
		btCollisionObject::CF_KINEMATIC_OBJECT);
	//anadir el puntero a player para la deteccion de colisiones
	m_ghostObject->setUserPointer(player);

	btScalar stepHeight = btScalar(0.5f);
	m_character = new CharacterControllerPhysics(m_ghostObject,capsule,stepHeight,
		PhysicsManager::getSingletonPtr()->getCollisionWorld());

	//PhysicsManager::getSingletonPtr()->getCollisionWorld()->
	//	addCollisionObject(m_ghostObject,btBroadphaseProxy::CharacterFilter, 
	//	btBroadphaseProxy::StaticFilter|btBroadphaseProxy::DefaultFilter);

	PhysicsManager::getSingletonPtr()->getDynamicsWorld()
		->getPairCache()->setInternalGhostPairCallback
		(new btGhostPairCallback());

	PhysicsManager::getSingletonPtr()->getDynamicsWorld()->
		addCollisionObject(m_ghostObject,btBroadphaseProxy::CharacterFilter, 
		btBroadphaseProxy::StaticFilter|btBroadphaseProxy::DefaultFilter);
	
	PhysicsManager::getSingletonPtr()->getDynamicsWorld()->
		addAction(m_character);

	//probando
	m_ghostObject->setCcdMotionThreshold(0.001);
	m_ghostObject->setCcdSweptSphereRadius(2);

	m_pCharacterNode = node;
}
//|||||||||||||||||||||||||||||||||||||||||||||||||||
void CharacterController::reset(Ogre::Vector3 resetPos = Ogre::Vector3::ZERO){
	m_character->warp(BtOgre::Convert::toBullet(resetPos));
	m_character->reset();
}

void CharacterController::teleport(Ogre::Vector3 pos){
	m_character->warp(BtOgre::Convert::toBullet(pos));
	m_character->reset();
};


void CharacterController::move(Ogre::Vector3 moveVector, float deltaTime){
	//m_character->setWalkDirection( (BtOgre::Convert::toBullet(moveVector*0.1) )); 
	//	+ velocityPlatform*1/60));
	//indicamos que se realize el movimiento. Si estamos sobre una plataforma
	//velocityPlatform sera distinto de cero y sumamos su velocidad para que el 
	//personaje se mueva junto a la plataforma sobre ella
	m_character->setVelocityForTimeInterval(BtOgre::Convert::toBullet(moveVector) + 
		velocityPlatform, 1);

}

void CharacterController::update(double timeSinceLastFrame){
	//hacemos un raycast para ver si estamos sobre una plataforma
	checkOnPlatformMovement();

	Ogre::Vector3 finalMovementVec = m_movementVector;// * timeSinceLastFrame/1000;
	move(finalMovementVec,timeSinceLastFrame/1000); //
	
	m_pCharacterNode->setPosition( (BtOgre::Convert::toOgre
		( m_ghostObject->getWorldTransform().getOrigin() ) ) );

	velocityPlatform = BtOgre::Convert::toBullet(Ogre::Vector3::ZERO);
}
//|||||||||||||||||||||||||||||||||||||||||||||||||||


bool CharacterController::checkOnPlatformMovement(){
	m_IsOnPlatform = false;

	btVector3 rayDirection = BtOgre::Convert::toBullet
		(Ogre::Vector3::NEGATIVE_UNIT_Y);
	rayDirection = rayDirection;

	btVector3 rayStart = m_ghostObject->getWorldTransform().getOrigin();
	rayDirection = rayStart + rayDirection;
	
	btCollisionWorld::ClosestRayResultCallback rayCallback
		(rayStart, rayDirection);
	
	PhysicsManager::getSingletonPtr()->getDynamicsWorld()->
		rayTest(rayStart, rayDirection, rayCallback);
	
	//si el callback ha detectado una colision comprobamos si es con una plataforma
	//y si es el caso obtenemos su velocidad para despues sumarla a la velocidad del 
	//personaje y asi que este se mueva sobre ella
	if(rayCallback.hasHit())
	{
		btCollisionObject* obA = const_cast<btCollisionObject*>(rayCallback.m_collisionObject);

		if(!obA)
			return false;

		if(obA->getUserPointer()){
			GameObject* goA = (GameObject*)obA->getUserPointer();
			if ( (goA->getTag() == "platform") ) { 
				
				if(m_character->onGround()){
					btRigidBody* pBody = (btRigidBody*)btRigidBody::upcast(obA);
					btVector3 velocity = pBody->getLinearVelocity();
					velocityPlatform = velocity;	
					m_IsOnPlatform = true;
				}
			}
		}		
	}

	return false;
}

bool CharacterController::isGrounded(){
	return m_character->onGround();
}

Ogre::Vector3 CharacterController::getMovementVector(){
	return BtOgre::Convert::toOgre(m_character->getWalkDirection());
}

float CharacterController::getVerticalVelocity() const{
	return  m_character->getVerticalVelocity();
}