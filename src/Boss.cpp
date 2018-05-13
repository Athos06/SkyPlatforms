#include "Boss.h"
#include "GameManager.h"
#include "Player.h"

#include "Euler.h"

#include "PhysicsManager.h"
#include "Constants.h"

#include "btBulletDynamicsCommon.h"


#include "BossMotor.h"
#include "BossAnimation.h"
#include "BossState.h"

#include "ParticlesEffect.h"
#include "AudioManager.h"

Boss::Boss(){
}

Boss::Boss(Ogre::SceneNode *node, btRigidBody* rb){
	
	m_dead = false;
	m_pNode = node;
	m_pRigidbody = rb;
	tag = "boss";
}

Boss::~Boss(){

	m_pRigidbody = 0;	

	if(m_pBossMotor)
		delete m_pBossMotor;
	if(m_pBossState)
		delete m_pBossState;
	
}

void Boss::update(double deltaTime){
	if(!isDead()){	
		if(GameManager::getSingletonPtr()->getPlayer()->isDead()){
			m_pBossMotor->blockForTime(5);
			
		}
		m_pBossMotor->update(deltaTime);
		m_pBossState->update(deltaTime);
	}
}

void Boss::init(){
	Player* player = GameManager::getSingletonPtr()->getPlayer();

	//al bossMotor le pasamos el nodo del jugador, que es el enemigo del boss
	//y el nodo que representa la posicion donde se crean los disparos
	Ogre::SceneNode* enemyNode = player->m_pNode;
	Ogre::SceneNode* shootNode = dynamic_cast<Ogre::SceneNode*> (m_pNode->getChild("shootNode"));
	m_pBossMotor = new BossMotor(enemyNode, shootNode, this);
	
	//el boss esta formado por 2 mayas cada una en su nodo, la parte del canon
	//esta en un nodo y el personaje animado en otro. Necesito acceder a la entidad
	//del personaje animado que esta como nodo hijo del canon
	Ogre::SceneNode* characterBossNode = dynamic_cast<Ogre::SceneNode*>
		(m_pNode->getChild(m_pNode->getName()+"Character"));
	Ogre::Entity *ent = static_cast<Ogre::Entity*>
		(characterBossNode->getAttachedObject(0));
	
	BossAnimation* bossAnim = new BossAnimation(ent);
	bossAnim->setupAnimations();
	
	m_pBossState = new BossState(bossAnim, m_pBossMotor, this);

	//para tener control sobre cuando mostrar y ocultar el sistema
	//de particulas de la barrera de fuego
	m_pFireWallNode = GameManager::getSingletonPtr()->
		getSceneMgr()->getSceneNode("fireWallNode");
	
	m_pFireWallParticle = static_cast<Ogre::ParticleSystem*>
		(m_pFireWallNode->getAttachedObject("fireWall"));
	m_pFireWallParticle->setEmitting(true);

	setActive(false);
	//m_pBossMotor->blockForTime(3);
}

void Boss::reset(){
	m_pBossMotor->reset();
	m_pBossState->reset();
	blockForTime(2);

}

void Boss::blockForTime(float time){
	m_pBossMotor->blockForTime(time);
}

void Boss::setActive(bool isActive){
	if(isActive)
		m_pBossState->setCurrentState(BossState::ACTIVE);
	else
		m_pBossState->setCurrentState(BossState::INNACTIVE);

	m_pBossMotor->setActive(isActive);
}

void Boss::startDeath(){
	//comienza la logica de la muerte del boss
	//lo reseteamos a su posicion de inicio, mirando al frente
	m_pNode->setOrientation(m_pNode->getInitialOrientation());
	//lo bloqueamos mientras dura la escena para que no rote ni dispare
	m_pBossMotor->blockForTime(5);
	//y ponemos el estado del boss a DYING
	m_pBossState->setCurrentState(BossState::DYING);
}

void Boss::die(){
	if(m_dead)
		return;
	//sonido de explosion cuando se destruye el boss
	AudioManager::getSingletonPtr()->playFX("BigExplosion", 0, EXPLOSION_CHANNEL);

	m_dead = true;
	m_pNode->setVisible(false);
	m_pFireWallParticle->setEmitting(false);
	
	Ogre::Vector3 pos = m_pNode->_getDerivedPosition();
	//efectos de particulas de la trampa ejecutandose y la explosion
	pos.y -= 1;
	new ParticlesEffect(4, "trapSpiral", 
		 pos);
	pos.y += 3;
	new ParticlesEffect(2, "ExplosionSmoke", 
		 pos);
	new ParticlesEffect(1, "ExplosionChunks", 
		 pos);
	
}

/////////////////////////////////////////////////////////////////////

template<> BossManager* Ogre::Singleton<BossManager>::msSingleton = 0;


BossManager& BossManager::getSingleton(){
	assert (msSingleton); 
	return (*msSingleton);
}

 
BossManager* BossManager::getSingletonPtr(){
	assert (msSingleton);
	return msSingleton;
}

BossManager::BossManager(){
}


BossManager::~BossManager(){
	if(m_pBoss)
		delete m_pBoss;
}

void BossManager::createBoss(Ogre::SceneNode* node){
	//creamos el collider del boss (en realidad se usa para el fuego)
	//le pasamos nodo hijo al physicsmanager con las escala correspondiente
	Ogre::SceneNode* collNode = dynamic_cast<Ogre::SceneNode*> 
		(node->getChild("COLL"+ node->getName()));

	btRigidBody* rb = PhysicsManager::getSingletonPtr()->createPhysicObject(
		collNode, STATIC, PRIMITIVE_BOX, 0, "", true, 0, 0, 0, 
		Ogre::Vector3::ZERO, Ogre::Quaternion::IDENTITY, false);

	m_pBoss = new Boss(node, rb);

	rb->setUserPointer(m_pBoss);

	
	
}
void BossManager::destroy(){
	delete m_pBoss;
	m_pBoss = 0;
}

void BossManager::initBoss(){
	m_pBoss->init();
}

void BossManager::setBossActive(bool isActive){
	m_pBoss->setActive(isActive);
}

void BossManager::blockBossForTime(float time){
	m_pBoss->blockForTime(time);
}

void BossManager::resetBoss(){
	m_pBoss->reset();
}

void BossManager::update(double deltaTime){
	if(m_pBoss)
		m_pBoss->update(deltaTime);
}


void BossManager::killBoss(){
	m_pBoss->startDeath();
}