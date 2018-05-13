#ifndef PROYECTILE_H
#define PROYECTILE_H

#include <Ogre.h>
#include "btBulletDynamicsCommon.h"
#include "GameObject.h"

/**
 *  @author Jorge Rosique Contreras
 *  @date 13-6-2014
 *
 *  Los proyectiles que dispara el boss
 */
class Proyectile : public GameObject{
public:
	/**
	@brief destruye todos los proyectiles activos en la lista
	*/
	static void destroyAll();
	/**
	@brief constructor
	@param node el sceneNode del proyectil
	@param rb el rigidbody del proyectil
	*/
	Proyectile(Ogre::SceneNode* node, btRigidBody* rb);
	~Proyectile();
	/**
	@brief destruye el proyectil. No se llama a delete, solo se elimina
	de la lista y destruye el nodo y rigidbody asociados. Se encarga de llamar
	a delete
	*/
	void destroy();
	
private:
	//rigidbody asociado al proyectil
	btRigidBody* m_pRigidBody;	
	//lista de los proyectiles activos
	static std::list<Proyectile*> m_proyectilesList;
	//lista de los proyectiles inactivos a eliminar
	static std::vector<std::list<Proyectile*>::iterator> m_deletionList;
	//static std::vector<Proyectile*>	m_deletionList;
	//iterador para facilitar eliminacion
	std::list<Proyectile*>::iterator it;

};

#endif