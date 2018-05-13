//|||||||||||||||||||||||||||||||||||||||||||||
#include "Player.h"
#include "CharacterController.h"
#include "CharacterAnimation.h"
#include "CharacterState.h"
#include "CameraController.h"
#include "CharacterMotor.h"
#include "GameManager.h"
#include "AudioManager.h"
#include "Constants.h"
#include "StepTrailEmitter.h"
#include "LevelsManager.h"

//|||||||||||||||||||||||||||||||||||||||||||||

Player::Player(){
	m_pCharacterMotor	= 0;
	m_pCharacterState	= 0;
	m_pStepTrailNode	= 0;
	tag					= "player";
	m_pNode				= 0;
	m_nSkystones		= 0;
	m_nLives			= 3;
	m_bDead				= false;
	m_bIsBlocked		= false;
	m_blockTime			= 0;
	m_blockTimer		= 0;
}

Player::~Player(){
	if(m_pCharacterMotor)
		delete m_pCharacterMotor;
}


void Player::setupPlayer(Ogre::SceneNode* charNode){
	CharacterController *charController = new CharacterController();
	m_pNode = charNode;
	charController->setupCharacterController(charNode, this);

	m_pCharacterMotor = new CharacterMotor(charController);

	Ogre::Entity *ent = static_cast<Ogre::Entity*>(charNode->getAttachedObject(0));
	CharacterAnimation *charAnim = new CharacterAnimation(ent);
	charAnim->setupAnimations();

	m_pCharacterState = new CharacterState(charController,
		charAnim, m_pCharacterMotor, this);

	m_pStepTrailNode = m_pNode->createChildSceneNode("nodeStepTrail");
	m_pStepTrailNode->translate(0, -0.2, 0);

	m_pTrailEmitter = new StepTrailEmitter(m_pStepTrailNode);

}

void Player::teleport(Ogre::Vector3 pos){
	m_pCharacterMotor->teleport(pos);
}



void Player::setupCamera(CameraController* camera){
	m_pCamera = camera;
}


void Player::update(double deltaTime){
	if(isBlocked()){
		m_blockTimer += deltaTime/1000;
		if(m_blockTimer >= m_blockTime){
			m_bIsBlocked = false;
			m_blockTimer = 0;
			m_blockTime	= 0;
		}
	}

	m_pCharacterMotor->update(deltaTime);
	m_pCharacterState->update(deltaTime);
	m_pCamera->update(deltaTime);
	m_pTrailEmitter->update(deltaTime);
}


void Player::setMovementVector(Ogre::Vector3 movementDir){
	//si el movimiento esta bloqueado el vector movimiento es cero
	if(isBlocked() || isDead()){
		m_pCharacterMotor->setMoveDirection(Ogre::Vector3::ZERO);
		return;
	}
	Ogre::Vector3 goalDirection = Ogre::Vector3::ZERO;
	//lo que hago con esto es crear un vector que apunte en la direccion
	//de desplazamiento que dependa de la direccion en la que se encuentra la camara
	//por lo que obtengo la orientacion de la camara (en realidad del nodo al que se encuentra
	//anadida la camara) y multiplico esa orientacion por la direccion de desplazamiento que le 
	//pasamos con las teclas (adelante, atras, izq, der) por lo nos movemos en la direccion deseada
	//pero siempre respecto a la orientacion de la camara.
	goalDirection += movementDir.z * 
		m_pCamera->m_pCameraNode->_getDerivedOrientation().zAxis();
	goalDirection += movementDir.x * 
		m_pCamera->m_pCameraNode->_getDerivedOrientation().xAxis();
	goalDirection.y = 0;
	
	goalDirection.normalise();

	m_pCharacterMotor->setMoveDirection(goalDirection);
	 
	
}


void Player::rotateCamera(float xRot, float yRot, float zoom){
	//si el movimiento del jugador esta bloqueado tampoco permite
	//mover la camara
	if(isBlocked() || isDead())
		return;
	//llamamos a actualizar el movimiento de la camara
	m_pCamera->updateCameraGoal(xRot, yRot, zoom);
}

void Player::jump(){
	//si el personaje esta bloqueado o muerto no puede saltar
	if( (!isBlocked() && !isDead()) && m_pCharacterMotor->jump())
		AudioManager::getSingletonPtr()->playFX("Jump",0, MAINFX_CHANNEL);
}

void Player::pickUpStone(){
	//al recoger la skystones reproducimos el sonido y actualizamos
	//la variable que indica el numero de skystones recogidas
	AudioManager::getSingletonPtr()->playFX("PickedStone", 0, PICKUP_CHANNEL);
	m_nSkystones++;
	GameManager::getSingletonPtr()->updateStones();
	LevelsManager::getSingletonPtr()->getLevelProperties()->increaseCounter();
}

void Player::blockPlayerForTime(float blockTime){
	
	m_bIsBlocked = true;
	m_blockTimer = 0;
	m_blockTime = blockTime;
}

void Player::die(){
	m_bDead = true;
	AudioManager::getSingletonPtr()->playFX("getHit", 0, HIT_CHANNEL);
	//cuando el personaje muere restamos una vida y tambien lo actualizamos
	//en el gameManager que a su vez lo actualizara en el indicador del hud
	m_nLives--;
	GameManager::getSingletonPtr()->updateLives();
	//la camara deja de seguir al personaje cuando muere
	m_pCamera->setFollowPlayer(false);
}

void Player::reset(){
	m_bDead = false;
	Ogre::Vector3 resetPos = GameManager::getSingletonPtr()->
	m_pActiveSpawnPoint->_getDerivedPosition();
	m_pCharacterMotor->reset(resetPos);
	m_pNode->setOrientation(Ogre::Quaternion::IDENTITY);
	m_pCharacterState->reset();
	m_pCamera->reset();
}