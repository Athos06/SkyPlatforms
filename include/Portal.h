#ifndef PORTAL_H
#define PORTAL_H

#include <Ogre.h>
#include "btBulletDynamicsCommon.h"
#include "GameObject.h"

/**
 *  @author Jorge Rosique Contreras
 *  @date 14-6-2014
 *
 *  Clase de el portal de los niveles
 */
class Portal : public GameObject{
public:
	/**
	@brief constructor
	@param node nodo del interruptor
	@param sceneMgr scene manager actual
	*/
	Portal(Ogre::SceneNode* node, 
		Ogre::SceneManager* sceneMgr = 0);
	~Portal();
	/**
	@brief activa o desactiva el interruptor
	@param isActiva si activamos o desactivamos le interruptor
	*/
	void setActive(bool isActive);
	/**
	@brief comprueba is el interruptor esta activo
	@return si el interruptor esta activo o no
	*/
	bool isActive(){return m_isActive;};

private:
	bool m_isActive;
	btRigidBody* m_pRigidbody;

	Ogre::SceneNode* m_pLightNode;
};

#endif