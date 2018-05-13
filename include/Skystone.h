#ifndef SKYSTONE_H
#define SKYSTONE_H

#include <Ogre.h>
#include "btBulletDynamicsCommon.h"
#include "GameObject.h"
#include "Collectible.h"

class Animations;
/**
 *  @author Jorge Rosique Contreras
 *  @date 14-6-2014
 *
 *  Clase que alberga las skystones del juego y se encarga de la gestion de estas
 */
class Skystone : public Collectible{
public:
	Skystone();
	/**
	@brief constructor
	@param node el nodo de la skystone
	*/ 
	Skystone(Ogre::SceneNode* node);
	~Skystone();
	/**
	@brief Llamada para destruir la skystone.
	La elimina de la lista de objetos que se pueden recoger activos
	pero no llama a delete por lo que hay que hacerlo explicitamente
	*/ 
	void destroy();
private:
	btRigidBody *m_pRigidbody;
	Animations *m_pAnimation;

};

#endif