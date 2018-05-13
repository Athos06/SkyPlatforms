#ifndef GUI_STATE_MANAGER_h
#define GUI_STATE_MANAGER_h

#include "GUIState.h"


class GUIStateManager : public GUIStateListener
{
public:
	typedef struct
	{
		Ogre::String name;
		GUIState* state;
	} state_info;

	GUIStateManager();
	~GUIStateManager();

	void manageGUIState(Ogre::String stateName, GUIState* state);
	GUIState* findByName(Ogre::String stateName);
	void changeGUIState(GUIState* state);
	bool pushGUIState(GUIState* state);
	void popGUIState();
	void pauseGUIState();
    void popAllAndPushGUIState(GUIState* state);
	

protected:
	void init(GUIState *state);

	std::vector<GUIState*>		m_ActiveGUIStateStack;
	std::vector<state_info>		m_GUIStates;
	
};

#endif
