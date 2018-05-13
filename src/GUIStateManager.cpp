#include "GUIStateManager.h"
#include <OgreWindowEventUtilities.h>

//|||||||||||||||||||||||||||||||||||||||||||||||

GUIStateManager::GUIStateManager()
{
}

//|||||||||||||||||||||||||||||||||||||||||||||||

GUIStateManager::~GUIStateManager()
{	
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void GUIStateManager::manageGUIState(Ogre::String stateName, GUIState* state)
{
	try
	{
		state_info new_state_info;
		new_state_info.name = stateName;
		new_state_info.state = state;
		m_GUIStates.push_back(new_state_info);
	}
	catch(std::exception& e)
	{
		delete state;
		throw Ogre::Exception(Ogre::Exception::ERR_INTERNAL_ERROR, "Error while trying to manage a new GUIState\n" + Ogre::String(e.what()), "GUIStateManager.cpp (39)");
	}
}

//|||||||||||||||||||||||||||||||||||||||||||||||

GUIState* GUIStateManager::findByName(Ogre::String stateName)
{
	std::vector<state_info>::iterator itr;

	for(itr=m_GUIStates.begin();itr!=m_GUIStates.end();itr++)
	{
		if(itr->name==stateName)
			return itr->state;
	}

	return 0;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void GUIStateManager::changeGUIState(GUIState* state)
{
	if(!m_ActiveGUIStateStack.empty())
	{
		m_ActiveGUIStateStack.back()->exit();
		m_ActiveGUIStateStack.pop_back();
	}

	m_ActiveGUIStateStack.push_back(state);
	init(state);
	m_ActiveGUIStateStack.back()->enter();
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool GUIStateManager::pushGUIState(GUIState* state)
{
	if(!m_ActiveGUIStateStack.empty())
	{
		if(!m_ActiveGUIStateStack.back()->pause())
			return false;
	}

	m_ActiveGUIStateStack.push_back(state);
	init(state);
	m_ActiveGUIStateStack.back()->enter();

	return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void GUIStateManager::popGUIState()
{
	if(!m_ActiveGUIStateStack.empty())
	{
		m_ActiveGUIStateStack.back()->exit();
		m_ActiveGUIStateStack.pop_back();
	}

	if(!m_ActiveGUIStateStack.empty())
	{
		init(m_ActiveGUIStateStack.back());
		m_ActiveGUIStateStack.back()->resume();
	}

}

//|||||||||||||||||||||||||||||||||||||||||||||||

void GUIStateManager::pauseGUIState()
{
	if(!m_ActiveGUIStateStack.empty())
	{
		m_ActiveGUIStateStack.back()->pause();
	}

	if(m_ActiveGUIStateStack.size() > 2)
	{
		init(m_ActiveGUIStateStack.at(m_ActiveGUIStateStack.size() - 2));
		m_ActiveGUIStateStack.at(m_ActiveGUIStateStack.size() - 2)->resume();
	}
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void GUIStateManager::popAllAndPushGUIState(GUIState* state)
{
    while(!m_ActiveGUIStateStack.empty())
    {
        m_ActiveGUIStateStack.back()->exit();
        m_ActiveGUIStateStack.pop_back();
    }

    pushGUIState(state);
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void GUIStateManager::init(GUIState* state)
{
}
