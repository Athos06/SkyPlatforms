#ifndef MOVING_PLATFORM_H
#define MOVING_PLATFORM_H

#include <Ogre.h>
#include "btBulletDynamicsCommon.h"
#include "BtOgrePG.h"
#include "BtOgreGP.h"
#include "BtOgreExtras.h"
#include "GameObject.h"

class MovingPlatform : public GameObject{
public:
	/**
	@brief Llama a actualizar cada plataforma en cada frame
	@param deltaTime tiempo transcurrido en ms desde el ultimo frame
	*/
	static void updateEach(double deltaTime);
	/**
	@brief destruye todas las plataformas
	*/
	static void destroyAllPlatforms();
	/**
	@brief constructor
	*/
	MovingPlatform(Ogre::SceneNode* node);
	/**
	@brief actualiza la posicion de la plataforma
	@param deltaTime tiempo transcurrido en ms desde el ultimo frame
	*/
	void update(double deltaTime);
	~MovingPlatform();
private:
	static std::list<MovingPlatform*> m_platformsList;
	static std::vector<std::list<MovingPlatform*>::iterator> m_deletionList;

	std::list<MovingPlatform*>::iterator it; //para eliminarlo facilmente
	//las plataformas son objetos kinematic y utilizan un motionState propio
	//al que necesitamos acceder cada vez que la movemos
	//para la actualizacion en el mundo fisico
	BtOgre::RigidBodyState* m_pMotionState;
	btRigidBody *m_pRigidbody;

	Ogre::Vector3 lastPos;
	Ogre::Vector3 currentPos;
	int direction;
};
#endif