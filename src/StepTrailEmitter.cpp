#include "StepTrailEmitter.h"
#include "AdvancedOgreFramework.h"

StepTrailEmitter::StepTrailEmitter(Ogre::SceneNode* node){
	m_pNode = node;

	Ogre::SceneManager* sceneMgr = OgreFramework::getSingletonPtr()
		->m_pActiveSceneMgr;

	m_pParticle =  sceneMgr->createParticleSystem("particleStepTrail", 
		"StepDust");
	m_pNode->attachObject(m_pParticle);

	m_timer = 0;
	m_duration = 0;
	m_bContinuous = false;
	m_bActive = false;

	m_pParticle->setEmitting(false);
}

StepTrailEmitter::~StepTrailEmitter(){
}

void StepTrailEmitter::update(double deltaTime){
	//si hemos indicado que es continuo el efecto de particulas
	//emite siempre (hasta que manualmente llamemos a stop)
	if(m_bContinuous){
		m_pParticle->setEmitting(true);
	}
	else{
		//Comprobacion de si esta activo y el tiempo transcurrido
		//para saber si debemos detener la emision
		if(m_bActive){
			m_pParticle->setEmitting(true);
			m_timer += deltaTime/1000;
			if(m_timer >= m_duration)
			{
				stop();
			}
		}
	}
}

void StepTrailEmitter::start(bool continuous, float duration ){
	m_bContinuous = continuous;
	m_timer = 0;
	m_duration = duration;
	m_bActive = true;


}
void StepTrailEmitter::stop(){
	//dejamos de emitir las particulas
	m_pParticle->setEmitting(false);
	m_timer = 0;
	m_duration = 0;
	m_bContinuous = false;
	m_bActive = false;
}