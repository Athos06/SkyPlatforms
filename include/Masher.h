#ifndef MASHER_H
#define MASHER_H

#include <Ogre.h>
#include "Trap.h"
#include "BtOgrePG.h"
#include "BtOgreGP.h"
#include "BtOgreExtras.h"

/**
 *  @author Jorge Rosique Contreras
 *  @date 14-6-2014
 *
 *  clase de los bloques con pinchos que aplastan
 */
class Masher : public Trap{
public:
	Masher();
	Masher(Ogre::SceneNode* node);
	~Masher();
	/**
	@brief actualiza el masher cada frame
	@param deltaTime tiempo transcurrido desde el ultimo frame en ms
	*/
	void update(double deltaTime);
private:
	BtOgre::RigidBodyState* m_pMotionState;
};

#endif