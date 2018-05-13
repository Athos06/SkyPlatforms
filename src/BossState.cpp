#include "BossState.h"
#include "BossAnimation.h"
#include "BossMotor.h"
#include "Boss.h"

BossState::BossState(BossAnimation* bossAnim, BossMotor* bossMotor, 
		Boss* boss){
	//estado por defecto idle
	m_currentState = IDLE;

	m_pBossAnimation = bossAnim;
	m_pBossMotor = bossMotor;
	m_pBoss = boss;

	m_bDying = false;
	m_bDieTimer = 0;
}

BossState::~BossState(){
	m_pBoss = 0;
}

void BossState::update(double deltaTime){

	//una vez el boss ha terminado la animacion de 
	//ser destruido  se pone en estado DEAD
	if(m_bDying){
		m_bDieTimer += deltaTime/1000;
		if(m_bDieTimer >= 3){
			setCurrentState(DEAD);
		}
	}
	calculateCurrentState();
	processState();
	m_pBossAnimation->update(deltaTime);
}


void BossState::calculateCurrentState(){
	//si esta muerto
	if(getCurrentState() == DEAD){
		return;
	}
	//si esta en la animacion de ser destruido
	if(getCurrentState() == DYING){
		m_bDying = true;
		return;
	}
	//si el movimiento esta bloqueado (lo que indica
	//que esta esperando a que empiece el nivel o que acabe)
	if(m_pBossMotor->isBlocked()){
		setCurrentState(LOCKACTION);
		return;
	}
	if(getCurrentState() == INNACTIVE){
		return;
	}
	//y si no es ninguno de los anteriores esta activo y atacando
	setCurrentState(ACTIVE);

}

void BossState::processState(){
	//dependiendo del estado en el que estemos
	//ejecutamos una animacion u otra
	switch(m_currentState){
		case IDLE:
			m_pBossAnimation->setAnimation
				(BossAnimation::ANIM_IDLE);
			break;
		case ACTIVE:
			m_pBossAnimation->setAnimation
				(BossAnimation::ANIM_ATTACKING);
			break;
		case INNACTIVE:
			m_pBossAnimation->setAnimation
				(BossAnimation::ANIM_IDLE);
			break;
		case DYING:
			m_bDying = true;
			m_pBossAnimation->setAnimation
				(BossAnimation::ANIM_DIE);
			break;
		case DEAD:
			m_pBoss->die();
			break;
		case LOCKACTION:
			m_pBossAnimation->setAnimation
				(BossAnimation::ANIM_IDLE);
			break;
	}
}

void BossState::setCurrentState(BossState::States nextState){
	m_currentState = nextState;
}

void BossState::reset(){
	//reseteo las animaciones
	m_pBossAnimation->reset();
	m_bDying = false;
	m_bDieTimer = 0;
}


BossState::States BossState::getCurrentState(){
	return m_currentState;
}
