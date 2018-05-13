#ifndef SPIKES_H
#define SPIKES_H

#include <Ogre.h>
#include "Trap.h"
/**
 *  @author Jorge Rosique Contreras
 *  @date 14-6-2014
 *
 *  clase de la trampa de los pinchos
 */
class Spikes : public Trap{
public:
	Spikes();
	Spikes(Ogre::SceneNode* node);
	~Spikes();
	/**
	@brief no hace nada, se trata de un trampa estatica
	por lo que no necesita update
	*/
	void update(double deltaTime){};
private:

};

#endif
