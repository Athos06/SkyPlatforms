//||||||||||||||||||||||||||||||||||||||||||||||||

#ifndef  CHARACTER_STATE_H
#define CHARACTER_STATE_H
//||||||||||||||||||||||||||||||||||||||||||||||||

#include <Ogre.h>

class CharacterAnimation;
class CharacterController;
class CharacterMotor;
class Player;

/**
 *  @author Jorge Rosique Contreras
 *  @date 14-6-2014
 *
 *  Maquina de estados del personaje
 */
class CharacterState{
public:
	//diferentes estados que puede tener el player
	enum States{
		IDLE,
		JUMPING,
		FALLING,
		RUNNING,
		DIE,
		LOCKACTION
	};

	/**
	@brief constructor
	@param charAnim puntero a la clase CharacterAnimation del boss
	@param charMotor puntero al CharacterMotor
	@para boss puntero al Player
	*/
	CharacterState(CharacterController* cc, 
		CharacterAnimation* charAnim, CharacterMotor* charMotor, 
		Player* player);
	~CharacterState();
	/**
	@brief llamado cada frame, en el nos encargamos de obtener el estado actual
	del player y actuar en consecuencia
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
	@brief resetea todas las variables relativas al CharacterState
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
	CharacterController* m_pCharacterController;
	CharacterAnimation* m_pCharAnimation;
	CharacterMotor* m_pCharacterMotor;
	Player* m_pPlayer;
	//para ver cuando se reproduce el sonido de los pasos del personaje
	bool m_bStepSoundPlaying;
};

#endif