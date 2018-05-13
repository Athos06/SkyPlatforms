#ifndef SAVEGAME_LOADER_H
#define SAVEGAME_LOADER_H

#include <Ogre.h>
#include "rapidxml.h"

/**
 *  @author Jorge Rosique Contreras
 *  @date 14-6-2014
 *
 *  Clase encargada de gestionar el savegame
 *	El savegame no es mas que un fichero de texto plano que indica 
 *	SI el nivel ha sido ya desbloqueado o no
 */
class SavegameLoader{
public:
	SavegameLoader();
	~SavegameLoader();
	
	/**
	@brief escribe en el savegame para desbloquear niveles
	*/
	void write();
	/**
	@brief lee el saveGame
	*/
	void read();

private:
	Ogre::String m_groupName;
	Ogre::String m_savegameName;
	



};

#endif