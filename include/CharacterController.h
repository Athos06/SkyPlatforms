//|||||||||||||||||||||||||||||||||||||||||||||||||||
#ifndef CHARACTER_CONTROLLER_H
#define CHARACTER_CONTROLLER_H
//|||||||||||||||||||||||||||||||||||||||||||||||||||


#include <Ogre.h>
#include "BulletCollision/CollisionShapes/btConvexHullShape.h"



class CharacterControllerPhysics;
class btCharacterControllerInterface;
class btDynamicCharacterController;
class btCollisionShape;
class btPairCachingGhostObject;
class btBroadphaseInterface;
class btCollisionDispatcher;
class btConstraintSolver;
class btDefaultCollisionConfiguration;
class btTriangleIndexVertexArray;

class Player;
//|||||||||||||||||||||||||||||||||||||||||||||||||||
/**
 *  @author Jorge Rosique Contreras
 *  @date 14-6-2014
 *
 *  Clase que se encarga de ejecutar el movimiento final del personaje
 * y que actua como una interfaz entre las fisicas del personaje el control de este
 */
class CharacterController{
public:
	CharacterController();
	~CharacterController();
	/**
	@brief inicializa el character controller
	@param node el sceneNode del Player
	@param player puntero al Player
	*/
	void setupCharacterController(Ogre::SceneNode* node, Player* player);
	/**
	@brief resetea todo lo relativo al CharacterController y sus componentes. Llama
	tambien a resetear las fisicas del personaje
	*/
	void reset(Ogre::Vector3 resetPos);
	/**
	@brief llamado cada frame. actualiza el CharacterController
	@param deltaTime tiempo transcurrido desde el ultimo frame en ms
	*/
	void update(double timeSinceLastFrame);
	/**
	@brief llamado cada frame. Realiza el movimiento final del personaje en dicho frame
	@param moveVector vector con la direccion de movimiento. Sera multiplicado por la velocidad
	de movimiento para obtener el vector de movimiento final
	@param deltaTime tiempo transcurrido desde el ultimo frame en ms
	*/
	void move(Ogre::Vector3 moveVector, float deltaTime);
	/**
	@brief mueve al personaje a la posicion indicada
	@param pos posicion donde se movera el personaje
	*/
	void teleport(Ogre::Vector3 pos);
	/**
	@brief establece el vector de la direccion de movimiento. 
	*/
	void setMovementVector(Ogre::Vector3 vec){m_movementVector = vec;};
	/**
	@brief se encarga de hacer un raycast desde el personaje hasta un poco debajo de sus 
	pies para ver si esta sobre una plataforma
	@return true si esta sobre una plataforma
	*/
	bool checkOnPlatformMovement();
	/**
	@brief comprueba si el personaje esta en el suelo o no
	@return si el personaje se encuentra en el suelo
	*/
	bool isGrounded();
	/**
	@brief comprueba si el personaje se encuentra sobre una plataforma movil o no
	@return si el personaje se encuentra sobre una plataforma movil
	*/
	bool isOnPlatform() const{return m_IsOnPlatform;};
	/**
	@brief obtiene el vector de movimiento de el personaje.
	No se trata del m_movementVector si no del vector movimiento real que se pasa
	utiliza las fisicas del personaje en cada update 
	@return vector de movimiento actual
	*/
	Ogre::Vector3 getMovementVector();
	/**
	@brief obtiene la velocidad en el eje y, que indica si esta cayendo o saltando
	@return velocidad vertical del personaje
	*/
	
	float getVerticalVelocity() const;
	
	//componentes para la fisica del personaje
	btPairCachingGhostObject* m_ghostObject;
	btBroadphaseInterface*	m_overlappingPairCache;
	btCollisionDispatcher*	m_dispatcher;
	btConstraintSolver*	m_constraintSolver;
	btDefaultCollisionConfiguration* m_collisionConfiguration;
	btTriangleIndexVertexArray*	m_indexVertexArrays;
	
	CharacterControllerPhysics *m_character;
	
	Ogre::SceneNode* m_pCharacterNode;

	btVector3 velocityPlatform;

	Ogre::Vector3  m_movementVector;

	bool m_IsOnPlatform;
private:
	
};
//|||||||||||||||||||||||||||||||||||||||||||||||||||

#endif
//|||||||||||||||||||||||||||||||||||||||||||||||||||