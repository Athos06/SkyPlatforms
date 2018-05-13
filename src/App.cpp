#include "App.h"

App::App ()
{
}

App::~App ()
{
	//if(m_pAppStateManager) delete m_pAppStateManager;
	delete OgreFramework::getSingletonPtr();
}

void
App::start ()
{
	new OgreFramework();
	if(!OgreFramework::getSingletonPtr()->initOgre("Sky Platforms", 0, 0))
		return;

	OgreFramework::getSingletonPtr()->m_pLog->logMessage("demo initialized");

	OgreFramework::getSingletonPtr()->initAppStateMgr();

}


