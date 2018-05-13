#include "Proyectile.h"
#include "AdvancedOgreFramework.h"
#include "PhysicsManager.h"
#include "AudioManager.h"
#include "Constants.h"
#include "ParticlesEffect.h"

std::list<Proyectile*>  Proyectile::m_proyectilesList;
std::vector<std::list<Proyectile*>::iterator> Proyectile::m_deletionList;

void Proyectile::destroyAll(){
/*
	if(!m_deletionList.empty()){
		for(int i = 0; i < m_deletionList.size(); i++){
				m_proyectilesList.erase(m_deletionList[i]);
		}
		m_deletionList.clear();
	}
	*/ 		
	if(m_proyectilesList.empty())
		return;

	//se anaden a la lista de eliminacion para que node error al eliminar un iterador
	//mientras se recorre la lista
	std::list<Proyectile*>::iterator aux;
	for(aux = m_proyectilesList.begin();
	aux != m_proyectilesList.end(); aux++){
		m_deletionList.push_back(aux);
	}

	//finalmente se eliminan todos los proyectiles
	if(!m_deletionList.empty()){
		for(int i = 0; i < m_deletionList.size(); i++){
			(*m_deletionList[i])->destroy();
		}
		m_deletionList.clear();
	} 
	
	m_deletionList.clear();
	m_proyectilesList.clear();
}

Proyectile::Proyectile(Ogre::SceneNode* node, btRigidBody* rb){
	m_pRigidBody = rb;
	m_pNode = node;
	tag = "proyectile";
	m_pRigidBody->setUserPointer(this);

	Ogre::Entity* ent = OgreFramework::getSingletonPtr()->m_pActiveSceneMgr
		->createEntity("entity"+m_pNode->getName(), "Proyectile.mesh");
	m_pNode->attachObject(ent);

	m_proyectilesList.push_front(this);
	//guardamos el iterador para una eliminacion mas facil de la lista
	it = m_proyectilesList.begin();

	//creo el efecto del trail
	Ogre::SceneManager* sceneMgr = OgreFramework::getSingletonPtr()->m_pActiveSceneMgr;

	Ogre::RibbonTrail* trail = sceneMgr->createRibbonTrail
		(node->getName()+"trail");
	trail->setMaterialName("Trail");
	trail->setTrailLength(7);
	trail->setMaxChainElements(100);
	trail->setInitialColour(0, 1, 0.1, 0, 1);
	trail->setColourChange(0, 0, 0, 0.0, 5);
	trail->setInitialWidth(0, 2);
	trail->setWidthChange(0, 10);

	trail->addNode(node);
	sceneMgr->getRootSceneNode()->attachObject(trail);

}

Proyectile::~Proyectile(){

}

void Proyectile::destroy(){

	Ogre::Vector3 pos = m_pNode->_getDerivedPosition();

	OgreFramework::getSingletonPtr()->m_pActiveSceneMgr->
		destroyRibbonTrail(m_pNode->getName()+"Trail");
	
	OgreFramework::getSingletonPtr()->m_pActiveSceneMgr
		->destroySceneNode(m_pNode);
	//reproducimos el sonido de la explosion
	AudioManager::getSingletonPtr()->playFX("ProyectileExplosion", 0, EXPLOSION_CHANNEL);	
	//destruimos el rigidbody
	PhysicsManager::getSingletonPtr()->destroyPhysicObject(m_pRigidBody);
	m_pNode = 0;
	m_pRigidBody = 0;	
	
	//creamos el efecto de particulas de la explosion
	new ParticlesEffect(1.5, "SmallExplosionSmoke", pos);
	new ParticlesEffect(1.5, "SmallExplosionChunks", pos);

	//se elimina de la lista
	m_proyectilesList.erase(it);
	
	delete this;


}



















/*
#include "Proyectile.h"
#include "AdvancedOgreFramework.h"
#include "PhysicsManager.h"
#include "AudioManager.h"
#include "Constants.h"
#include "ParticlesEffect.h"

std::list<Proyectile*>  Proyectile::m_proyectilesList;
std::vector<Proyectile*> Proyectile::m_deletionList;

void Proyectile::destroyAll(){
	if(m_proyectilesList.empty())
		return;

	//primero se destruyen los proyectiles y despues se 
	//anaden los punteros cola de eliminacion
	std::list<Proyectile*>::iterator aux;
	for(aux = m_proyectilesList.begin();
	aux != m_proyectilesList.end(); aux++){
		(*aux)->destroy();
		m_deletionList.push_back((*aux));
	}
	
	//finalmente se eliminan todos los proyectiles
	if(!m_deletionList.empty()){
		for(int i = 0; i < m_deletionList.size(); i++){
			delete m_deletionList[i];
		}
		m_deletionList.clear();
	}
	
	m_deletionList.clear();
	m_proyectilesList.clear();
}

Proyectile::Proyectile(Ogre::SceneNode* node, btRigidBody* rb){
	m_pRigidBody = rb;
	m_pNode = node;
	tag = "proyectile";
	m_pRigidBody->setUserPointer(this);

	Ogre::Entity* ent = OgreFramework::getSingletonPtr()->m_pActiveSceneMgr
		->createEntity("entity"+m_pNode->getName(), "Proyectile.mesh");
	m_pNode->attachObject(ent);

	m_proyectilesList.push_front(this);
	//guardamos el iterador para una eliminacion mas facil de la lista
	it = m_proyectilesList.begin();

	
	//creo el efecto del trail
	Ogre::SceneManager* sceneMgr = OgreFramework::getSingletonPtr()->m_pActiveSceneMgr;

	Ogre::RibbonTrail* trail = sceneMgr->createRibbonTrail
		(node->getName()+"trail");
	trail->setMaterialName("Trail");
	trail->setTrailLength(7);
	trail->setMaxChainElements(50);
	trail->setInitialColour(0, 1, 0.1, 0, 1);
	trail->setColourChange(0, 0, 0, 0.0, 7);
	trail->setInitialWidth(0, 2);
	trail->setWidthChange(0, 15);

	trail->addNode(node);
	sceneMgr->getRootSceneNode()->attachObject(trail);
}

Proyectile::~Proyectile(){
	m_proyectilesList.erase(it);
}

void Proyectile::destroy(){
	Ogre::Vector3 pos = m_pNode->_getDerivedPosition();
	OgreFramework::getSingletonPtr()->m_pActiveSceneMgr
		->destroySceneNode(m_pNode);
	OgreFramework::getSingletonPtr()->m_pActiveSceneMgr->
		destroyRibbonTrail(m_pNode->getName()+"Trail");
	//reproducimos el sonido de la explosion
	AudioManager::getSingletonPtr()->playFX("ProyectileExplosion", 0, EXPLOSION_CHANNEL);	
	//destruimos el rigidbody
	PhysicsManager::getSingletonPtr()->destroyPhysicObject(m_pRigidBody);
	m_pNode = 0;
	m_pRigidBody = 0;	
	//creamos el efecto de particulas de la explosion
	new ParticlesEffect(1.5, "SmallExplosionSmoke", pos);
	new ParticlesEffect(1.5, "SmallExplosionChunks", pos);

}
*/ 