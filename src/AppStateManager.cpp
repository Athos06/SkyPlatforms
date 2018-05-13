//|||||||||||||||||||||||||||||||||||||||||||||||

#include "AppStateManager.h"
#include "PhysicsManager.h"
#include <OgreWindowEventUtilities.h>
#include <OgreRenderWindow.h>
#include <Ogre.h>
#include "AdvancedOgreFramework.h"
//|||||||||||||||||||||||||||||||||||||||||||||||

AppStateManager::AppStateManager()
{
	m_bShutdown = false;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

AppStateManager::~AppStateManager()
{
	state_info si;

    while(!m_ActiveStateStack.empty())
	{
		m_ActiveStateStack.back()->exit();
		m_ActiveStateStack.pop_back();
	}

	while(!m_States.empty())
	{
		si = m_States.back();
        si.state->destroy();
        m_States.pop_back();
	}
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void AppStateManager::manageAppState(Ogre::String stateName, AppState* state)
{
	try
	{
		state_info new_state_info;
		new_state_info.name = stateName;
		new_state_info.state = state;
		m_States.push_back(new_state_info);
	}
	catch(std::exception& e)
	{
		delete state;
		throw Ogre::Exception(Ogre::Exception::ERR_INTERNAL_ERROR, "Error while trying to manage a new AppState\n" + Ogre::String(e.what()), "AppStateManager.cpp (39)");
	}
}

//|||||||||||||||||||||||||||||||||||||||||||||||

AppState* AppStateManager::findByName(Ogre::String stateName)
{
	std::vector<state_info>::iterator itr;

	for(itr=m_States.begin();itr!=m_States.end();itr++)
	{
		if(itr->name==stateName)
			return itr->state;
	}

	return 0;
}
	
//|||||||||||||||||||||||||||||||||||||||||||||||

void AppStateManager::start(AppState* state)
{
	changeAppState(state);
	
	
	//framerate limitado a 60fps (1/60 = 0.0166)
	//framerate limitado a 30fps (1/30 = 0.033)
	//framerate de la logica del juego

	const double TickMs = 34; 
	
	/// The physics clock is just used to run the physics and runs asynchronously with the gameclock
	unsigned long time_physics_prev, time_physics_curr;
	/// There's an inner loop in here where things happen once every TickMs. These variables are for that.
	unsigned long time_gameclock;

	/// Reset all of our timers
	time_physics_prev = OgreFramework::getSingletonPtr()->m_pTimer->getMilliseconds();
	time_gameclock = OgreFramework::getSingletonPtr()->m_pTimer->getMilliseconds();
	
	
	int timeSinceLastFrame = 1;
	int startTime = 0;
	 
	const double dt = 34;
	double currentTime = OgreFramework::getSingletonPtr()->m_pTimer->getMilliseconds()/1000.0f;
	double accumulator = 0.0;

	while(!m_bShutdown)
	{
		if(OgreFramework::getSingletonPtr()->m_pRenderWnd->isClosed())m_bShutdown = true;
		 		
		Ogre::WindowEventUtilities::messagePump();

		if(OgreFramework::getSingletonPtr()->m_pRenderWnd->isActive())
		{	
			double newTime = OgreFramework::getSingletonPtr()->
				m_pTimer->getMilliseconds();
	         double frameTime = newTime - currentTime;
	         //para que cuando hay un bajon de fps no rompa el main loop
			 //al perder tiempo y despues tener recuperarlo, lo que aceleraria 
			 //el juego
			 
			 if ( frameTime > 34 )
          		frameTime = 34;	  
	         currentTime = newTime;

	         accumulator += frameTime;


	         while ( accumulator >= dt )
	         {
				accumulator -= dt;

				OgreFramework::getSingletonPtr()->m_pKeyboard->capture();
				OgreFramework::getSingletonPtr()->m_pMouse->capture();

				
				m_ActiveStateStack.back()->update(dt);
				//PhysicsManager::getSingletonPtr()->update(dt);	
				OgreFramework::getSingletonPtr()->updateOgre(dt);
     		 } 
			
			OgreFramework::getSingletonPtr()->m_pRoot->renderOneFrame();
	


	}
			
	else
	{
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            Sleep(1000);
			
#else
            sleep(1);
#endif
		}
	}

	OgreFramework::getSingletonPtr()->m_pLog->logMessage("Main loop quit");
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void AppStateManager::changeAppState(AppState* state)
{
	if(!m_ActiveStateStack.empty())
	{
		m_ActiveStateStack.back()->exit();
		m_ActiveStateStack.pop_back();
	}

	m_ActiveStateStack.push_back(state);
	init(state);
	m_ActiveStateStack.back()->enter();
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool AppStateManager::pushAppState(AppState* state)
{
	if(!m_ActiveStateStack.empty())
	{
		if(!m_ActiveStateStack.back()->pause())
			return false;
	}

	m_ActiveStateStack.push_back(state);
	init(state);
	m_ActiveStateStack.back()->enter();

	return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void AppStateManager::popAppState()
{
	if(!m_ActiveStateStack.empty())
	{
		m_ActiveStateStack.back()->exit();
		m_ActiveStateStack.pop_back();
	}

	if(!m_ActiveStateStack.empty())
	{
		init(m_ActiveStateStack.back());
		m_ActiveStateStack.back()->resume();
	}
    else
		shutdown();
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void AppStateManager::popAllAndPushAppState(AppState* state)
{
    while(!m_ActiveStateStack.empty())
    {
        m_ActiveStateStack.back()->exit();
        m_ActiveStateStack.pop_back();
    }

    pushAppState(state);
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void AppStateManager::pauseAppState()
{
	if(!m_ActiveStateStack.empty())
	{
		m_ActiveStateStack.back()->pause();
	}

	if(m_ActiveStateStack.size() > 2)
	{
		init(m_ActiveStateStack.at(m_ActiveStateStack.size() - 2));
		m_ActiveStateStack.at(m_ActiveStateStack.size() - 2)->resume();
	}
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void AppStateManager::shutdown()
{
	m_bShutdown = true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void AppStateManager::init(AppState* state)
{
    OgreFramework::getSingletonPtr()->m_pKeyboard->setEventCallback(state);
	OgreFramework::getSingletonPtr()->m_pMouse->setEventCallback(state);

	OgreFramework::getSingletonPtr()->m_pRenderWnd->resetStatistics();
}

//|||||||||||||||||||||||||||||||||||||||||||||||
