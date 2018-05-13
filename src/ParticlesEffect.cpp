#include "ParticlesEffect.h"
#include "AdvancedOgreFramework.h"

std::list<ParticlesEffect*>  ParticlesEffect::m_particlesEffectsList;
std::vector<std::list<ParticlesEffect*>::iterator>  ParticlesEffect::m_deletionList;
		

int ParticlesEffect::m_NumInstanced = 0;


ParticlesEffect::ParticlesEffect(float lifeTime, Ogre::String templateName,
								 Ogre::Vector3 position){
	
	tag="particle";

	m_AliveTime = 0;
	m_LifeTime = lifeTime;

	Ogre::SceneManager* sceneMgr = OgreFramework::getSingletonPtr()
		->m_pActiveSceneMgr;
	//creo el nodo que tendra asociado el sistema de particulas
	m_pNode = sceneMgr->getRootSceneNode()->createChildSceneNode(
		"nodeParticleEffect"+Ogre::StringConverter::toString(
		m_NumInstanced) );
	//crep el sistema de particula
	m_pParticle =  sceneMgr->createParticleSystem
		("particleParticleEffect"+Ogre::StringConverter::toString(
		m_NumInstanced), templateName);
	m_pNode->attachObject(m_pParticle);
	m_pNode->setPosition(position);
	//anado el efecto de particulas a la lista
	m_particlesEffectsList.push_front(this);
	it = m_particlesEffectsList.begin();
	//incremento el contador para crear nombres sin conflictos
	m_NumInstanced++;
}


ParticlesEffect::~ParticlesEffect(){
}


void ParticlesEffect::updateEach(double deltaTime){
	//elimina de la lista los efectos de particulas destruidos
	//en el anterior update
	if(!m_deletionList.empty()){
		for(int i = 0; i < m_deletionList.size(); i++){
			m_particlesEffectsList.erase(m_deletionList[i]);
		}
		m_deletionList.clear();
	}
	
	if(m_particlesEffectsList.empty())
		return;

	std::list<ParticlesEffect*>::iterator aux;
	for(aux = m_particlesEffectsList.begin();
		aux != m_particlesEffectsList.end(); aux++){
		(*aux)->update(deltaTime);
	}
}

void ParticlesEffect::destroyAllParticles(){
	if(m_particlesEffectsList.empty())
		return;

	std::list<ParticlesEffect*>::iterator aux;
	for(aux = m_particlesEffectsList.begin();
		aux != m_particlesEffectsList.end(); aux++){
		delete (*aux);
	}
	m_particlesEffectsList.clear();
	m_deletionList.clear();
}



void ParticlesEffect::update(double deltaTime){
	m_AliveTime += (deltaTime/1000);
	//si ha sobrepasado su tiempo de vida util lo destruyo
	if( m_AliveTime >= m_LifeTime  ){
		destroy();
	}	
}

void ParticlesEffect::destroy(){
	//se anade a la cola de objetos eliminados
	//para que en el siguiente update se elimine de la lista
	m_deletionList.push_back(it);

	//destruyo el sistema de particulas y el nodo
	Ogre::SceneManager* sceneMgr = OgreFramework::getSingletonPtr()
		->m_pActiveSceneMgr;
	sceneMgr->destroyParticleSystem(m_pParticle);
	m_pNode->detachAllObjects();
	sceneMgr->destroySceneNode(m_pNode);

	m_pParticle = 0;
	m_pNode = 0;
	
	delete this;
	
}	
