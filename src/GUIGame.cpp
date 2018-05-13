#include <CEGUI.h>

#include "AdvancedOgreFramework.h"
#include "AppStateManager.h"


#include "GUIManager.h"
#include "GUIGame.h"

//|||||||||||||||||||||||||||||||||||||||||||||||

template<> GUIGame* Ogre::Singleton<GUIGame>::msSingleton = 0;

//|||||||||||||||||||||||||||||||||||||||||||||||

GUIGame& GUIGame::getSingleton(){
assert (msSingleton); 
return (*msSingleton);
}
//|||||||||||||||||||||||||||||||||||||||||||||||

GUIGame* GUIGame::getSingletonPtr(){
assert (msSingleton);
return msSingleton;
}
//|||||||||||||||||||||||||||||||||||||||||||||||

GUIGame::GUIGame(){
}
//|||||||||||||||||||||||||||||||||||||||||||||||

GUIGame::~GUIGame(){
	
}
//|||||||||||||||||||||||||||||||||||||||||||||||

void GUIGame::enter()
{	
	loadCEGUI();

	m_pGUImgr->m_pGUISystem->setGUISheet(m_pGUIsheet);
	CEGUI::System::getSingleton().signalRedraw();
}
//|||||||||||||||||||||||||||||||||||||||||||||||

void GUIGame::loadCEGUI()
{
	CEGUI::MouseCursor::getSingleton().hide();
	m_pGUImgr = OgreFramework::getSingletonPtr()->m_pGUImgr;
	m_pGUIsheet = m_pGUImgr->m_pGUIsheetInGame;	 
}
//|||||||||||||||||||||||||||||||||||||||||||||||

void GUIGame::exit()
{
	//m_pGUImgr->m_pGUISystem->setGUISheet(0);
	CEGUI::MouseCursor::getSingleton().show();
}

void GUIGame::resume(){
	//m_pGUImgr->m_pGUISystem->setGUISheet(m_pGUIsheet);
	CEGUI::MouseCursor::getSingleton().hide();
	m_pGUImgr->m_pGUISystem->setGUISheet(m_pGUIsheet);
	//por alguna razon no se cambia el GUI sheet hasta que no se mueve el raton
	//por ejemplo, es como si no se actualizara automaticamente, con esto se marca
	//para actualizar en el siguiente frame 
	CEGUI::System::getSingleton().signalRedraw();
	
}
//|||||||||||||||||||||||||||||||||||||||||||||||

