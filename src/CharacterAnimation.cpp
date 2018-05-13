//|||||||||||||||||||||||||||||||||||

#include "CharacterAnimation.h"
//|||||||||||||||||||||||||||||||||||



CharacterAnimation::CharacterAnimation(Ogre::Entity* entity){
	m_pEntity = entity;
	m_pActiveAnimation = 0;
	activeAnimID = ANIM_NONE;
}

CharacterAnimation::~CharacterAnimation(){
	m_pEntity = 0;

}

void CharacterAnimation::setupAnimations(){

	m_pAnimations[ANIM_IDLE] = m_pEntity->getAnimationState("Idle");
	m_pAnimations[ANIM_IDLE]->setLoop(true);
	m_pAnimations[ANIM_RUNNING] = m_pEntity->getAnimationState("Running");
	m_pAnimations[ANIM_RUNNING]->setLoop(true);
	m_pAnimations[ANIM_JUMPING] = m_pEntity->getAnimationState("Jumping");
	m_pAnimations[ANIM_JUMPING]->setLoop(false);
	m_pAnimations[ANIM_FALLING] = m_pEntity->getAnimationState("Falling");
	m_pAnimations[ANIM_FALLING]->setLoop(false);
	m_pAnimations[ANIM_DIE1] = m_pEntity->getAnimationState("Die1");
	m_pAnimations[ANIM_DIE1]->setLoop(false);
	m_pAnimations[ANIM_RESTGAMECOMPLETE] = m_pEntity->getAnimationState("RestGameComplete");
	m_pAnimations[ANIM_RESTGAMECOMPLETE]->setLoop(true);

	//por defecto activamos la animacion de idle
	setAnimation(ANIM_IDLE);
	activeAnimID = ANIM_IDLE;
}

void CharacterAnimation::reset(){

	m_pAnimations[ANIM_IDLE]->setTimePosition(0);
	m_pAnimations[ANIM_RUNNING]->setTimePosition(0);
	m_pAnimations[ANIM_JUMPING]->setTimePosition(0);
	m_pAnimations[ANIM_FALLING]->setTimePosition(0);
	m_pAnimations[ANIM_DIE1]->setTimePosition(0);
	m_pAnimations[ANIM_RESTGAMECOMPLETE]->setTimePosition(0);

}

void CharacterAnimation::setAnimation(AnimID ANIM_ID){
	//desactivar la anterior animacion
	if(	m_pActiveAnimation){
		m_pActiveAnimation->setEnabled(false);
		//reiniciamos a la posicion de inicio cada vez que cambiamos a una 
		//animacion diferente para que en el caso de que no sea loop
		//no se quede siempre fija en el ultimo frame
		//de la animacion al volver a ejecutarla
		if( !(activeAnimID == ANIM_ID) )
		{	
			m_pAnimations[ANIM_ID]->setTimePosition(0);
		} 
	}
	//activar la nueva animacion
	m_pAnimations[ANIM_ID]->setEnabled(true);
	m_pAnimations[ANIM_ID]->setWeight(1);	
	
	m_pActiveAnimation = m_pAnimations[ANIM_ID];
	activeAnimID = ANIM_ID;

}

void CharacterAnimation::update(double deltaTime){

	float animationSpeed = 1.0;
	//actualizamos la animacion
	m_pActiveAnimation->addTime(deltaTime/1000 * animationSpeed);


}

