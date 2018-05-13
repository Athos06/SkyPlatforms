#ifndef COLLECTIBLE_H
#define COLLECTIBLE_H

#include <Ogre.h>
#include "btBulletDynamicsCommon.h"
#include "GameObject.h"
/**
 *  @author Jorge Rosique Contreras
 *  @date 14-6-2014
 *
 *  Clase de la que heredan los objetos que se pueden recoger
 */
class Collectible : public GameObject{
public:
	Collectible();
	~Collectible();
	/**
	@brief destruye todos los objetos de este tipo activos
	*/ 
	static void destroyAll();

protected:
	static std::list<Collectible*> m_collectiblesList;
	//static std::vector<std::list<Collectible*>::iterator> 
	//	m_deletionList;
	std::list<Collectible*>::iterator it;

	btRigidBody *m_pRigidbody;
};

#endif 