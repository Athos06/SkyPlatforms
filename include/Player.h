//|||||||||||||||||||||||||||||||||||||

#ifndef PLAYER_H
#define PLAYER_H
//|||||||||||||||||||||||||||||||||||||

#include <Ogre.h>
#include "GameObject.h"


//|||||||||||||||||||||||||||||||||||||
class CharacterState;
class CharacterMotor;
class CameraController;
class StepTrailEmitter;
/**
 *  @author Jorge Rosique Contreras
 *  @date 14-6-2014
 *
 *  Clase que contiene todo lo relacionado al Player
 */
class Player : public GameObject{
public:
	Player();
	~Player();
	/**
	@brief inicializa el player
	@param charNode el sceneNode del Player
	*/
	void setupPlayer(Ogre::SceneNode* charNode);
	/**
	@brief inicializa la camara del jugador
	@param camera puntero al cameraController
	*/
	void setupCamera(CameraController* camera);
	/**
	@brief llamado cada frame. LLama a actualizar los diferentes componentes
	del player
	@param deltaTime tiempo transcurrido desde el ultimo frame en ms
	*/
	void update(double deltaTime);
	/**
	@brief establece el vector de la direccion de movimiento. Se encarga de 
	calcular el vector de la direccion de movimiento a partir del movimiento
	que el jugador indica por teclado
	@param vec el vector que indica la direccion que se obtiene de la entrada de teclado
	del jugador
	*/
	void setMovementVector(Ogre::Vector3 vec);
	/**
	@brief llamado cuando se pulsa la tecla de salto
	*/
	void jump();
	/**
	@brief llamado cuando el personaje muere
	*/
	void die();
	/**
	@brief resetea todo lo relativo al player y sus componentes. Llamado cuando
	se continua tras morir
	*/
	void reset();
	/**
	@brief mueve al personaje a la posicion indicada
	@param pos posicion donde se movera el personaje
	*/
	void teleport(Ogre::Vector3 pos);
	/**
	@brief se encarga de llamar a la clase que se encarga del movimiento de la camara
	@param xRot rotacion de la camara en el eje x
	@param yRot rotacion de la camara en el eje y
	@param zoom distancia de la camara al jugador
	*/
	void rotateCamera(float xRot, float yRot, float zoom);
	/**
	@brief comprueba si el personaje esta muerto o no
	@return si esl player esta muerto o no
	*/
	bool isDead() const{ return m_bDead;};
	/**
	@brief comprueba si el personaje esta bloqueado o no
	@return si esl player esta bloqueado o no
	*/
	bool isBlocked() const{ return m_bIsBlocked;};
	/**
	@brief bloqueamos el player por un tiempo determinado. 
	@param time tiempo por el cual se bloquea el player
	*/
	void blockPlayerForTime(float blockTime);
	/**
	@brief llamado cuando se recoge una skystone
	*/
	void pickUpStone();
	/**
	@brief acceso al numero de skystones que el jugador a recogido
	@return numero de skystones recogidas
	*/
	int getSkystones() const{return m_nSkystones;};
	/**
	@brief acceso al numero de vidas restantes del jugaodr
	@return numero de vidas restantes
	*/
	int getLives() const{return m_nLives;};

	CharacterMotor* m_pCharacterMotor;
	StepTrailEmitter* m_pTrailEmitter;

	Ogre::Vector3 movementDirVector;


private:
	int m_nSkystones;
	int m_nLives;

	Ogre::SceneNode* m_pStepTrailNode;
	
	CharacterState* m_pCharacterState;
	
	CameraController* m_pCamera;

	float m_blockTimer;
	float m_blockTime;
	bool m_bIsBlocked;
	bool m_bDead;


};
//|||||||||||||||||||||||||||||||||||||

#endif