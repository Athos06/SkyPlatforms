#include "BossAnimation.h"

BossAnimation::BossAnimation(Ogre::Entity* entity){
	m_pEntity = entity;
	m_pActiveAnimation = 0;
	activeAnimID = ANIM_NONE;
}

BossAnimation::~BossAnimation(){
	m_pEntity = 0;
}

void BossAnimation::setupAnimations(){
	m_pAnimations[ANIM_IDLE] = m_pEntity->getAnimationState("Idle");
	m_pAnimations[ANIM_IDLE]->setLoop(true);
	m_pAnimations[ANIM_ATTACKING] = m_pEntity->getAnimationState("Attack");
	m_pAnimations[ANIM_ATTACKING]->setLoop(true);
	m_pAnimations[ANIM_DIE] = m_pEntity->getAnimationState("Die");
	m_pAnimations[ANIM_DIE]->setLoop(false);

	//por defecto activamos la animacion idle
	setAnimation(ANIM_IDLE);
	activeAnimID = ANIM_IDLE;
}

void BossAnimation::setAnimation(BossAnimation::AnimID ANIM_ID){
	//desactivar la anterior animacion
	if(	m_pActiveAnimation){
		m_pActiveAnimation->setEnabled(false);
		//reiniciamos a la posicion de inicio cada vez que cambiamos a una 
		//animacion diferente para que en el caso de que no sea loop
		//no se quede siempre fija en el ultimo frame de la animacion al 
		//volver a ejecutarla
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

void BossAnimation::reset(){
	m_pAnimations[ANIM_IDLE]->setTimePosition(0);
	m_pAnimations[ANIM_ATTACKING]->setTimePosition(0);
	m_pAnimations[ANIM_DIE]->setTimePosition(0);
}

void BossAnimation::update(double deltaTime){
	float animationSpeed = 1.15;
	
	m_pActiveAnimation->addTime(deltaTime/1000 * animationSpeed);
}