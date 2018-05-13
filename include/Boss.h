#ifndef BOSS_H
#define BOSS_H

#include <Ogre.h>
#include "btBulletDynamicsCommon.h"
#include "GameObject.h"

class BossMotor;
class BossState;

/**
 *  @author Jorge Rosique Contreras
 *  @date 13-6-2014
 *
 *  Clase que contiene todo lo relacionado al boss
 */
class Boss : public GameObject{
public:
	Boss();
	/**
	@brief constructor
	@param node el sceneNode del boss
	@param rb el rigidbody usado para el collider del boss
	*/
	Boss(Ogre::SceneNode* node, btRigidBody* rb);
	~Boss();
	/**
	@brief llamado cada frame. LLama a actualizar los diferentes componentes
	del boss
	@param deltaTime tiempo transcurrido desde el ultimo frame en ms
	*/
	void update(double deltaTime);
	/**
	@brief inicializa el boss y sus componentes
	*/
	void init();
	/**
	@brief resete las variables del boss y sus componentes.
	Llamado tras continuar despues de morir
	*/
	void reset();
	/**
	@brief establece si el boss esta activo o inactivo
	@param isActive si activamos o desactivamos el boss
	*/
	void setActive(bool isActive);
	/**
	@brief bloqueamos el boss por un tiempo determinado. 
	Llama a bloquear el BossMotor
	@param time tiempo por el cual se bloquea el boss
	*/
	void blockForTime(float time);
	/**
	@brief comienza la escena de la destruccion del boss
	*/
	void startDeath();
	/**
	@brief matamos al boss, ejecutamos los sonidos, efectos de particula
	correspondientes y desactivamos la barrera de fuego y ocultamos el nodo
	*/
	void die();
	/**
	@brief comprueba si el boss esta muerto o no
	@return si el boss esta muerto o no
	*/
	bool isDead(){ return m_dead;};

private:
	//acceso al bossMotor
	BossMotor* m_pBossMotor;
	//acceso al bossState
	BossState* m_pBossState;
	
	//el efecto de particulas de la barrera de fuego
	Ogre::SceneNode* m_pFireWallNode;
	Ogre::ParticleSystem* m_pFireWallParticle;

	bool m_dead;
	
	btRigidBody* m_pRigidbody;

};

/**
 *  @author Jorge Rosique Contreras
 *  @date 13-6-2014
 *
 *  Clase que actua como gestor del boss y nos permite el acceso a este
 */
class BossManager : public Ogre::Singleton<BossManager>{
public:
	static BossManager& getSingleton();
	static BossManager* getSingletonPtr();

	BossManager();
	~BossManager();
	/**
	@brief Crea el boss. Llamado al crear la escena
	@param node el sceneNode del boss
	*/
	void createBoss(Ogre::SceneNode* node);
	/**
	@brief llama a inicializar el boss
	*/
	void initBoss();
	/**
	@brief llama a resetear el boss
	*/
	void resetBoss();
	/**
	@brief llama a activar o desactivar el boss
	@param isActive si activamos o desactivamos el boss
	*/
	void setBossActive(bool isActive);
	/**
	@brief llama a bloquear el boss durante un tiempo indicado
	@param time tiempo por el que bloquear el boss
	*/
	void blockBossForTime(float time);
	/**
	@brief elimina el boss. Llamado al salir del nivel
	*/
	void destroy();
	/**
	@brief llama a actualizar el boss en cada frame
	@param deltaTime tiempo en ms desde el ultimo frame
	*/
	void update(double deltaTime);
	/**
	@brief llama a matar al boss. No elimina el boss, solo
	ejecuta la logica de matarlo
	*/
	void killBoss();

private:
	Boss* m_pBoss;
	
	
};

#endif