#ifndef DEATH_TRIGGER_H 
#define DEATH_TRIGGER_H

#include <Ogre.h>
#include "btBulletDynamicsCommon.h"
#include "GameObject.h"

/**
 *  @author Jorge Rosique Contreras
 *  @date 14-6-2014
 *
 *  Clase encargada de crear el limite inferior del nivel
 *	en el que al entrar produce la muerte del jugador
 */
class DeathTrigger : public GameObject{
public:
	/**
	@brief constructor
	@param node el nodo que se utiliza para el collider
	*/ 
	DeathTrigger(Ogre::SceneNode* node);
	~DeathTrigger();
private:
	btRigidBody *m_pRigidbody;
};

#endif