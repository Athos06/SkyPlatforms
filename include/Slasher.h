#ifndef SLASHER_H
#define SLASHER_H

#include <Ogre.h>
#include "Trap.h"
#include "BtOgrePG.h"
#include "BtOgreGP.h"
#include "BtOgreExtras.h"

/**
 *  @author Jorge Rosique Contreras
 *  @date 14-6-2014
 *
 *  clase de las cuchillas con movimiento de pendulo
 */
class Slasher : public Trap{
public:
	Slasher(Ogre::SceneNode* node);
	~Slasher();
	/**
	@brief actualiza el slasher cada frame
	@param deltaTime tiempo transcurrido desde el ultimo frame en ms
	*/
	void update(double deltaTime);
private:
	BtOgre::RigidBodyState* m_pMotionState;
};

#endif