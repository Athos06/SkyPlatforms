	//||||||||||||||||||||||||||||||||||||||||||||||||

#include "CharacterState.h"
#include "CharacterAnimation.h"
#include "CharacterController.h"
#include "CharacterMotor.h"
#include "Player.h"
#include "AudioManager.h"
#include "Constants.h"
#include "StepTrailEmitter.h"

//||||||||||||||||||||||||||||||||||||||||||||||||


CharacterState::CharacterState(CharacterController* cc, 
							   CharacterAnimation* charAnim,
							   CharacterMotor* charMotor, Player* player){
	
	m_bStepSoundPlaying	= false;
	m_pCharacterController = cc;
	m_pCharAnimation		= charAnim;
	m_pCharacterMotor		= charMotor;
	m_pPlayer				= player;

}

CharacterState::~CharacterState(){
}


//dependidendo el estado en el que nos encontremos ejecutamos
//una animacion u otra
void CharacterState::processState(){
	switch(m_currentState){
		case IDLE:
			m_bStepSoundPlaying = false;
			AudioManager::getSingletonPtr()->stopFX("Step");
			m_pCharAnimation->setAnimation(
				CharacterAnimation::ANIM_IDLE);
			break;
		case RUNNING:
			m_pCharAnimation->setAnimation(
				CharacterAnimation::ANIM_RUNNING);
			//un caso especial de reproduccion de efecto de sonido
			//el efecto de los pasos al andar tiene que reproducir siempre
			//en loop mientras el personaje este andando
			//Si no se esta reproduciendo lo reproducimos
			if(!m_bStepSoundPlaying){
				AudioManager::getSingletonPtr()->playFX("Step", -1, FOOTSTEP_CHANNEL);
				m_bStepSoundPlaying = true;
			}
			break;
		case JUMPING:
			m_bStepSoundPlaying = false;
			//en el resto de casos que no este andando dejamos de reproducir
			//el efecto de los pasos
			AudioManager::getSingletonPtr()->stopFX("Step");
			m_pCharAnimation->setAnimation(
				CharacterAnimation::ANIM_JUMPING);
			break;
		case FALLING:
			m_bStepSoundPlaying = false;
			AudioManager::getSingletonPtr()->stopFX("Step");
			m_pCharAnimation->setAnimation(
				CharacterAnimation::ANIM_FALLING);
			break;
		case DIE:
			m_bStepSoundPlaying = false;
			AudioManager::getSingletonPtr()->stopFX("Step");
			m_pCharAnimation->setAnimation(
				CharacterAnimation::ANIM_DIE1);
			break;
	}	
}

//obtenemos el estado actual en el que se encuentra el personaje
//y realizmos la accion correspondiente a dicho estado
void CharacterState::calculateCurrentState(){
	//antes de nada ver si esta muerto
	if(m_pPlayer->isDead()){
		setCurrentState(DIE);
		return;
	}
	//esta en el suelo
	if(m_pCharacterController->isGrounded()){
		//mostramos particulas de atterizar
		if(m_currentState == FALLING){
				m_pPlayer->m_pTrailEmitter->start(false, 0.15);
		}
		//si el personaje esta en movimento actualizamos a estado running
		if(Ogre::Math::Abs(m_pCharacterMotor->getMovementVector().x) > 0 ||		
		Ogre::Math::Abs(m_pCharacterMotor->getMovementVector().z) > 0){			
			//mostramos particulas de empezar a correr
			if(m_currentState != RUNNING){
				//cuando empezamos a andar activamos el efecto
				//del rastro de polvo del personaje
				m_pPlayer->m_pTrailEmitter->start(false, 0.3);
			}
			setCurrentState(RUNNING);
		}
		else{
			//si esta en el suelo sin moverse esta en idle
			setCurrentState(IDLE);
			//m_pPlayer->m_pTrailEmitter->stop();
		}
	}
	//esta en el aire
	else{
		//si tenemos velocidad positiva en el eje y es que estamos saltando
		if(m_pCharacterController->getVerticalVelocity() > 0){
			//mostramos particulas de salto
			if(m_currentState == IDLE || m_currentState == RUNNING){
				m_pPlayer->m_pTrailEmitter->start(false, 0.15);
			}
			setCurrentState(JUMPING);
		}
		//si es negativa estamos cayendo
		if(m_pCharacterController->getVerticalVelocity() < 0){
			setCurrentState(FALLING);
		}
	}
}
//||||||||||||||||||||||||||||||||||||||||||||||||
//cambiamos el estado al actual al que le pasemos como parametro
void CharacterState::setCurrentState(CharacterState::States nextState){
	m_currentState = nextState;
}
//||||||||||||||||||||||||||||||||||||||||||||||||

CharacterState::States CharacterState::getCurrentState(){
	return m_currentState;
}
//||||||||||||||||||||||||||||||||||||||||||||||||

void CharacterState::update(double deltaTime){
	calculateCurrentState();
	processState();
	m_pCharAnimation->update(deltaTime);
}

void CharacterState::reset(){
	m_pCharAnimation->reset();
}