#ifndef SWITCH_H
#define SWITCH_H

#include <Ogre.h>
#include "btBulletDynamicsCommon.h"
#include "GameObject.h"

/**
 *  @author Jorge Rosique Contreras
 *  @date 14-6-2014
 *
 *  Clase de los interruptores del nivel del jefe final
 */
class Switch : public GameObject{
public:
	/**
	@brief constructor
	@param node nodo del interruptor
	@param sceneMgr scene manager actual
	*/
	Switch(Ogre::SceneNode* node, 
		Ogre::SceneManager* sceneMgr = 0);
	~Switch();
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
	Ogre::SceneNode* m_pSwitchButton;


};

#endif