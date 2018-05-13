//||||||||||||||||||||||||||||||||||||||||||||||||

#ifndef BOSS_STATE_H
#define BOSS_STATE_H
//||||||||||||||||||||||||||||||||||||||||||||||||

#include <Ogre.h>

class BossAnimation;
class BossMotor;
class Boss;

/**
 *  @author Jorge Rosique Contreras
 *  @date 13-6-2014
 *
 *  Maquina de estados del boss
 */
class BossState{
public:
	//diferentes estados que puede tener el boss
	enum States{
		IDLE,
		ACTIVE,
		INNACTIVE,
		DEAD,
		DYING,
		LOCKACTION
	};

	/**
	@brief constructor
	@param bossAnim puntero a la clase bossAnimation del boss
	@param bossMotor puntero al bossMotor
	@para boss puntero al boss
	*/
	BossState(BossAnimation* bossAnim, BossMotor* bossMotor, 
		Boss* boss);
	~BossState();
	/**
	@brief llamado cada frame, en el nos encargamos de obtener el estado actual
	del boss y actuar en consecuencia
	@param deltaTime tiempo en ms desde el ultimo frame
	*/
	void update(double deltaTime);
	/**
	@brief llamado cada ves en el update para comprobar cual
	sera el siguiente estado
	*/
	void calculateCurrentState();
	/**
	@brief se encarga de realizar la logica dependiente del estado actual
	*/
	void processState();
	/**
	@brief establece el estado actual 
	@param nextState estado que qeremos establecer como actual
	*/
	void setCurrentState(States nextState);
	/**
	@brief resetea todas las variables relativas al BossState
	*/
	void reset();
	/**
	@brief obtener el estado actual
	@return devuelve el estado actual
	*/
	States getCurrentState();

private:
	//estado actual
	States m_currentState;
	//puntero al animator del boss
	BossAnimation* m_pBossAnimation;
	BossMotor* m_pBossMotor;
	Boss* m_pBoss;

	bool m_bDying;
	float m_bDieTimer;
};

#endif