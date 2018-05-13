#ifndef CHARACTER_MOTOR_H
#define CHARACTER_MOTOR_H

#include <Ogre.h>

class CharacterController;

/**
 *  @author Jorge Rosique Contreras
 *  @date 14-6-2014
 *
 *  Clase que se encarga de gestionar todo lo relacionado con el movimiento
	del character
 */
class CharacterMotor{
public:
	/**
	@brief constructor
	@param cc puntero al CharacterController utilizado para efectuar el movimiento
	calculado en el CharacterMotor
	*/
	CharacterMotor(CharacterController* cc);
	~CharacterMotor();
	/**
	@brief funcion llamada en cada frame y encargada de calcular la orientacion 
	 del character con respecto a su vector de movimiento teniendo en cuenta
	 la posicion de la camara
	*/
	void snapRotation();
	/**
	@brief se encarga de llamar a la funcion correspondiente del CharacterController
	para efectuar el movimiento del character en el frame actual
	*/
	void applyMovement();
	/**
	@brief se encarga de llamar a la funcion correspondiente del CharacterController 
	para mover al personaje a una posicion determinada. Utilizar la funcion applyMovement
	para el movimiento del personaje, la funcion teleport solo debe ser utilizada precisamente
	para eso, para teletransportar al personaje a una posicion determinada en un momento
	específico.
	@param pos Posicion a la que se movera el character
	*/
	void teleport(Ogre::Vector3 pos);
	/**
	@brief resetea el CharacterMotor
	@param resetPos Posicion a la que queremos mover el character cuando lo reseteamos
	*/
	void reset(Ogre::Vector3 resetPos);
	/**
	@brief llamado cada frame. actualiza el CharacterMotor
	@param deltaTime tiempo transcurrido desde el ultimo frame en ms
	*/
	void update(double deltaTime);
	/**
	@brief se encarga de llamar a la funcion correspondiente en el CharacterController
	para efectuar el salto
	*/
	bool jump();
	/**
	@brief establece el vector de movimiento del character
	@param movVec vector de movimiento
	*/
	void setMovementVector(Ogre::Vector3 movVec);
	/**
	@brief establece el vector de la direccion del movimiento
	@param movDir vector direccion
	*/
	void setMoveDirection(Ogre::Vector3 movDir);
	/**
	@brief acceso al vector de movimiento del character
	@return vector de movimiento
	*/
	Ogre::Vector3 getMovementVector(){ return m_movementVector;};

private:
	CharacterController* m_pCharController;

	Ogre::Vector3 m_movementVector;
	Ogre::Vector3 m_moveDirection;
	float m_walkSpeed;

};

#endif