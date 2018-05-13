#ifndef CHECKPOINT_H
#define CHECKPOINT_H

#include <Ogre.h>
#include "GameObject.h"
#include "btBulletDynamicsCommon.h"


/**
 *  @author Jorge Rosique Contreras
 *  @date 14-6-2014
 *
 *  Clase encargada de crear los diferentes checkPoints que hay
 *	en los niveles. Cada checkpoint contiene un spawnPoint que es la posicion
 *	donde el jugador aparecera tras continuar al morir
 */
class CheckPoint : GameObject{
public:
	/**
	@brief constructor
	@param node el nodo que se utiliza para el collider
	*/ 
	CheckPoint(Ogre::SceneNode* node);
	~CheckPoint();

	/**
	@brief establece el checpoint de inicio por defecto del nivel
	*/ 
	void setDefaultCheckPoint();
	/**
	@brief obtiene el nodo con la posicion donde reaparecera
	el jugador
	*/ 
	Ogre::SceneNode* getSpawnPoint() const{ return m_pSpawnPoint;};

private:
	Ogre::SceneNode* m_pSpawnPoint;
	btRigidBody *m_pRigidbody;

};

#endif