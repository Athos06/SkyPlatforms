#include "Animations.h"
#include "AdvancedOgreFramework.h"

std::list<Animations*> Animations::m_AnimationsList;
std::vector<std::list<Animations*>::iterator> Animations::m_deletionList;

void Animations::updateEach(double deltaTime){
	//antes de nada en cada frame vemos si hay
	//animaciones que destruimos en el frame anterior
	//y que hay que eliminar
	if(!m_deletionList.empty()){
		for(int i = 0; i < m_deletionList.size(); i++){
			m_AnimationsList.erase(m_deletionList[i]);
		}
		m_deletionList.clear();
	}
	
	if(m_AnimationsList.empty())
		return;
	//llamamos al update de cada animacion activa
	std::list<Animations*>::iterator aux;
	for(aux = m_AnimationsList.begin();
		aux != m_AnimationsList.end(); aux++){
		(*aux)->update(deltaTime);
	}
}


void Animations::resetEach(){
	
	if(m_AnimationsList.empty())
		return;

	//recorremos la lista con todas las animaciones activas
	//y las reseteamos
	std::list<Animations*>::iterator aux;
	for(aux = m_AnimationsList.begin();
		aux != m_AnimationsList.end(); aux++){
		(*aux)->reset();
	}
}

void Animations::ChangeSpeedAll(float relativeSpeed){
	if(!m_deletionList.empty()){
		for(int i = 0; i < m_deletionList.size(); i++){
			m_AnimationsList.erase(m_deletionList[i]);
		}
		m_deletionList.clear();
	}
	
	if(m_AnimationsList.empty())
		return;
	
	std::list<Animations*>::iterator aux;
	for(aux = m_AnimationsList.begin();
		aux != m_AnimationsList.end(); aux++){
			(*aux)->changeSpeed(0, relativeSpeed);
	}
}

void Animations::destroyAllAnimations(){
	
	m_deletionList.clear();

	if(m_AnimationsList.empty())
		return;
	//recorremos la lista con todas las animaciones activas
	//y las eliminamos una a una
	std::list<Animations*>::iterator aux;
	for(aux = m_AnimationsList.begin();
		aux != m_AnimationsList.end(); aux++){
		delete (*aux);
	}

	m_AnimationsList.clear();
}

Animations::~Animations(){
}

Animations::Animations(Ogre::Animation* anim,
					   Ogre::AnimationState* animState, float speed,
					   int startFrame){
	m_pAnim = anim;
	m_pAnimState = animState;
	m_animSpeed = speed;
	m_startFrame = startFrame;

	//iniciamos en el frame que le pasamos de inicio
	m_pAnimState->addTime(m_startFrame);

	//anadimos el elemento al final del vector..
	m_AnimationsList.push_front(this);
	//y guardamos el iterador al elemento que acabamos de crear
	it = m_AnimationsList.begin();
}


void Animations::changeSpeed(float speed, float relativeSpeed){
	if(relativeSpeed != 0){
		float speedChange = m_animSpeed * relativeSpeed;
		m_animSpeed += speedChange;
	}
	else{
		m_animSpeed = speed;
	}
}

void Animations::update(double deltaTime){
	m_pAnimState->addTime(m_animSpeed * deltaTime/1000);

}
void Animations::reset(){
	m_pAnimState->setTimePosition(0);
	//iniciamos en el frame que le pasamos de inicio
	m_pAnimState->addTime(m_startFrame);
}

void Animations::destroy(){
	Ogre::SceneManager* sceneMgr = OgreFramework::getSingletonPtr()->m_pActiveSceneMgr;
	//destruimos la animacion de ogre
	m_pAnim->destroyAllNodeTracks();
	sceneMgr->destroyAnimation(m_pAnim->getName());
	//destruimos el animationState de ogre asociado
	sceneMgr->destroyAnimationState(m_pAnimState->getAnimationName());
	m_deletionList.push_back(it);
	delete this;
}

