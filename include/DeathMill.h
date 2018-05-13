#ifndef DEATHMILL_H
#define DEATHMILL_H

#include <Ogre.h>
#include "Trap.h"
#include "BtOgrePG.h"
#include "BtOgreGP.h"
#include "BtOgreExtras.h"
#include <vector>

/**
 *  @author Jorge Rosique Contreras
 *  @date 14-6-2014
 *
 *  clase de el molino con aspas de cuchillas
 */
class DeathMill : public Trap{
public:
	DeathMill(Ogre::SceneNode* node);
	~DeathMill();
	/**
	@brief actualiza el deathMill cada frame
	@param deltaTime tiempo transcurrido desde el ultimo frame en ms
	*/
	void update(double deltaTime);
private:
	//el molino esta compuesto por 3 aspas, cada aspa
	//tiene un nodo propio y su propio collider
	std::vector<Ogre::SceneNode*> m_armsNodes;

	std::vector<btRigidBody*> m_multipleRigidBodies;
	std::vector<BtOgre::RigidBodyState*> m_multipleMotionStates;
};

#endif