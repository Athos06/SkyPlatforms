#include "CharacterMotor.h"
#include "CharacterController.h"
#include "CharacterControllerPhysics.h"
#include "Euler.h"

CharacterMotor::CharacterMotor(CharacterController* cc){
	m_walkSpeed			= 4;
	m_pCharController	= cc;
	m_moveDirection		= Ogre::Vector3::ZERO;
	m_movementVector	= Ogre::Vector3::ZERO;
}


CharacterMotor::~CharacterMotor(){
	if(m_pCharController)
		delete m_pCharController;
}

void CharacterMotor::reset(Ogre::Vector3 resetPos = Ogre::Vector3::ZERO){
	m_pCharController->reset(resetPos);
}

void CharacterMotor::teleport(Ogre::Vector3 pos){
	m_pCharController->teleport(pos);
}
bool CharacterMotor::jump(){
	if(m_pCharController->m_character->canJump()){
		m_pCharController->m_character->jump();
		return true;
	}
}

void CharacterMotor::update(double deltaTime){
	snapRotation();
	applyMovement();
	m_pCharController->update(deltaTime);
}

//setters
void CharacterMotor::setMovementVector(Ogre::Vector3 movVec){
	m_movementVector = movVec;
}
void CharacterMotor::setMoveDirection(Ogre::Vector3 movDir){
	m_moveDirection = movDir;
}


void CharacterMotor::snapRotation(){	
	//hacemos que el personaje siempre mire hacia la direccion en la que se mueve
	Ogre::Quaternion goalOrientation = m_pCharController->m_pCharacterNode
		->_getDerivedOrientation().zAxis().getRotationTo(m_moveDirection,
		m_pCharController->m_pCharacterNode->_getDerivedOrientation().yAxis());

	float yawToGoal = goalOrientation.getYaw().valueDegrees();

	m_pCharController->m_pCharacterNode->yaw(Ogre::Degree(yawToGoal));
}

void CharacterMotor::applyMovement(){
	m_moveDirection.normalise();
	//este vector es el vector final de movimiento, con la velocidad aplicada, pero falta
	//multiplicarlo por delta time para que sea independiente del framerate. Eso lo hago en
	//el character controller en el update para cuando se aplica el movimiento
	m_movementVector = m_moveDirection * m_walkSpeed;

	m_pCharController->setMovementVector(m_movementVector);
}
