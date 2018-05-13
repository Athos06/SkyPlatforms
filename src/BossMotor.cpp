#include "BossMotor.h"


#include "Euler.h"

#include "PhysicsManager.h"
#include "Constants.h"

#include "btBulletDynamicsCommon.h"

#include "BtOgrePG.h"
#include "BtOgreGP.h"
#include "BtOgreExtras.h"
#include "AudioManager.h"
#include "Proyectile.h"
#include "Boss.h"
#include "GameManager.h"
#include "Player.h"

BossMotor::BossMotor(Ogre::SceneNode* enemyNode, Ogre::SceneNode* shootNode, Boss* boss){
	m_pShootNode = shootNode;
	m_pEnemyNode = enemyNode;

	m_pBoss = boss;
	m_shotForce = 0;
	m_shotTimer = 0;
	m_shotInterval = 1.5;

	m_blockLenght = 0;
	m_blockTimer = 0;

	m_bShotActive = false;

	m_bMovementBlocked = false;
	m_blockLenght = 3;
	m_blockTimer = 0;
}

BossMotor::~BossMotor(){
	m_pBoss = 0;
}

void BossMotor::update(double deltaTime){		

	//cuando el personaje muere destruimos todos los proyectiles
	//activos para que no se queden por ahi en medio
	if(GameManager::getSingletonPtr()->getPlayer()->isDead() ){
		Proyectile::destroyAll();
	}
	//si el boss no esta activo no hacemos nada
	if(!m_bIsActive)
		return;

	//si el movimiento esta bloqueado actualizmos el timer
	//del bloqueo
	if(m_bMovementBlocked)
	{
		Proyectile::destroyAll();
		m_blockTimer += deltaTime/1000;
		if(m_blockTimer >= m_blockLenght){
			m_bMovementBlocked = false;
			m_blockTimer = false;
		}
		return;
	} 

	rotate();	

	//si estamos listos para disparar reiniciamos la logica del disparo
	//y ponemos m_bShotActive a false lo que indica que tenemos la posibilidad
	//de disparar de nuevo
	if(m_bShotActive){
		m_shotTimer += deltaTime/1000;
		if(m_shotTimer >= m_shotInterval){
			m_shotTimer = 0;
			m_bShotActive = false;
		}
	}
	//si no estamos en un intervalo de separacion entre disparos 
	//indica que podemos disparar
	if(!m_bShotActive){
		m_bShotActive = true;
		//obtenemos el input con las variables necesaria para realizar
		//el disparo apuntando al jugador
		getInput();
		shoot();
	}
}

void BossMotor::reset(){
	
	m_pBoss->m_pNode->setOrientation(m_pBoss->m_pNode->getInitialOrientation());
	m_bShotActive = true;
	m_bMovementBlocked = false;
	m_blockTimer = 0;
	blockForTime(2);

	Proyectile::destroyAll();
}

void BossMotor::rotate(){

	//m_pNode->setAutoTracking(true, m_pEnemyNode, Ogre::Vector3::UNIT_Z);
	m_pBoss->m_pNode->setFixedYawAxis(true);
	//hacer que siempre persiga a el jugador pero que solo rote en el eje y
	m_pBoss->m_pNode->lookAt(Ogre::Vector3(m_pEnemyNode->_getDerivedPosition().x, 
		m_pBoss->m_pNode->_getDerivedPosition().y, m_pEnemyNode->_getDerivedPosition().z ),  
		Ogre::Node::TS_WORLD, 
		Ogre::Vector3::UNIT_Z);
}

void BossMotor::getInput(){

	//ecuaciones de tiro parabolico para calcular la fuerza
	int angle = 31;
	Ogre::Vector2 a = Ogre::Vector2(m_pShootNode->_getDerivedPosition().x,
		m_pShootNode->_getDerivedPosition().z);
	Ogre::Vector2 b = Ogre::Vector2(m_pEnemyNode->_getDerivedPosition().x,
		m_pEnemyNode->_getDerivedPosition().z);
		
	float h = m_pShootNode->_getDerivedPosition().y -
		 m_pEnemyNode->_getDerivedPosition().y - 0.2;

	Ogre::Vector2 a2 = a;
	Ogre::Vector2 b2 = b;

	if(h < 0){
		a2 = b;
		b2 = a;
	}


	float r = abs(a2.distance(b2));
	

	float t = sqrt( (h + (r*tan(angle*PI/180))) / 4.9 );
	
	m_shotForce = (r/(cos(angle*PI/180)*t));  
}


void BossMotor::shoot(){
	//creamos el proyectil, primero el nodo y luego el objecto fisico de bullet
	Ogre::Vector3 position = m_pShootNode->_getDerivedPosition();

	Ogre::Euler angle;
	angle = angle.fromQuaternion(m_pShootNode->_getDerivedOrientation() );

	Ogre::Vector3 direction = m_pShootNode->_getDerivedPosition() - 
		m_pBoss->m_pNode->_getDerivedPosition(); 

	//direction.y += 1;

	direction.normalise();

	Ogre::SceneNode *node =  GameManager::getSingletonPtr()->
		 getSceneMgr()->getRootSceneNode()->createChildSceneNode();
     //node->attachObject(entity);
	 node->setPosition(position);
	 node->setScale(1.5, 1.5, 1.5);
	//crear el objeto fisico
	btRigidBody* proyectileBody =  PhysicsManager::getSingletonPtr()->createPhysicObject(node,
		 DYNAMIC, PRIMITIVE_SPHERE, 0 , "", true, 0.0, 0.0, 1.0, 
		position, Ogre::Quaternion::IDENTITY, false);  
	node->setScale(1, 1, 1);
	proyectileBody->setLinearVelocity( BtOgre::Convert::toBullet(
		(direction * m_shotForce)) ); 

	Proyectile* proyectile = new Proyectile(node, proyectileBody);

	AudioManager::getSingletonPtr()->playFX("BossShoot", 0, SHOT_CHANNEL);		
}

void BossMotor::setActive(bool isActive){
	m_bIsActive = isActive;
}
void BossMotor::blockForTime(float time){
	m_blockLenght = time;
	m_blockTimer = 0;
	m_bMovementBlocked = true;

}
