#ifndef TRAP_H
#define TRAP_H

#include <Ogre.h>
#include "btBulletDynamicsCommon.h"
#include "GameObject.h"
/**
 *  @author Jorge Rosique Contreras
 *  @date 14-6-2014
 *
 *  Clase de la que heredan los diferentes obstaculos o trampas
 */
class Trap : public GameObject{
public:
	/**
	@brief llamado cada frame, actualiza todos los obstaculos activos
	@param deltaTime tiempo transcurrido desde el ultimo frame en ms
	*/ 
	static void updateEach(double deltaTime);
		/**
	@brief destruye todos los obstaculos activos
	*/
	static void destroyAllTraps();

	Trap();
	virtual ~Trap();

	virtual void update(double deltaTime) = 0;

protected:
	static std::list<Trap*> m_trapsList;
	static std::vector<std::list<Trap*>::iterator> m_deletionList;
	std::list<Trap*>::iterator it; //para eliminarlo facilmente

	btRigidBody *m_pRigidbody;
};

#endif