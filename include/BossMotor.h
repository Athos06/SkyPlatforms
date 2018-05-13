#ifndef BOSS_MOTOR_H
#define BOSS_MOTOR_H

#include <Ogre.h>

class Boss;
/**
 *  @author Jorge Rosique Contreras
 *  @date 13-6-2014
 *
 *  Clase encargada de gestionar todo lo relacionado al movmiento del Boss
 */
class BossMotor{
public:
	/**
	@brief constructor
	@param enemyNode el scenenode  del objetivo al que sigue 
	para disparar el boss, el jugador en este caso
	@param shootNode nodo de la posicion donde se crea el disparo
	@boss puntero a el Boss
	*/
	BossMotor(Ogre::SceneNode* enemyNode, 
		Ogre::SceneNode* shootNode, Boss* boss);
	~BossMotor();
	/**
	@brief llamado cada frame para actualizar el BossMotor
	@param deltaTime tiempo transcurrido desde el ultimo frame en ms
	*/
	void update(double deltaTime);
	/**
	@brief rota para mirar hacia el jugador
	*/
	void rotate();
	/**
	@brief llamado cada vez que se realiza un disparo
	*/
	void shoot();
	/**
	@brief llamado cada frame para obtener el input de la IA del boss
	*/
	void getInput();
	/**
	@brief resetea todas las variables y componentes relacionadas con el 
	BossMotor. LLamado al continuar nivel tras morir
	*/
	void reset();
	/**
	@brief Establece si es boss esta activo o no
	@param isActive indica si activamos o no el movimiento
	*/
	void setActive(bool isActive);
	/**
	@brief bloquear movimiento durante el tiempo que se seleccione
	@para time el tiempo a bloquer el movimiento
	*/
	void blockForTime(float time = 3);
	/**
	@brief comprueba si el moviemiento del boss esta bloqueado o no
	@return si el movimiento esta bloqueado o no
	*/
	bool isBlocked(){ return m_bMovementBlocked;};

private:
	//nodo posicion para la creacion del disparo
	Ogre::SceneNode* m_pShootNode;
	//nodo del jugador que es el objetivo del boss
	Ogre::SceneNode* m_pEnemyNode;
	//puntero al boss
	Boss* m_pBoss;

	bool m_bIsActive;
	float m_shotForce;
	float m_shotTimer;
	float m_shotInterval;

	bool m_bShotActive;
	bool m_bMovementBlocked;

	float m_blockLenght;
	float m_blockTimer;
};



#endif