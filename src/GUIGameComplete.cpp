#include <CEGUI.h>

#include "AdvancedOgreFramework.h"
#include "AppStateManager.h"
#include "GUIManager.h"
#include "GUIGameComplete.h"

#include "GameManager.h"
 

//|||||||||||||||||||||||||||||||||||||||||||||||

template<> GUIGameComplete* Ogre::Singleton<GUIGameComplete>::msSingleton = 0;

//|||||||||||||||||||||||||||||||||||||||||||||||

GUIGameComplete& GUIGameComplete::getSingleton(){
assert (msSingleton); 
return (*msSingleton);
}
//|||||||||||||||||||||||||||||||||||||||||||||||

GUIGameComplete* GUIGameComplete::getSingletonPtr(){
assert (msSingleton);
return msSingleton;
}

GUIGameComplete::GUIGameComplete(){
}
//|||||||||||||||||||||||||||||||||||||||||||||||

GUIGameComplete::~GUIGameComplete(){
	
}
//|||||||||||||||||||||||||||||||||||||||||||||||

void GUIGameComplete::enter()
{	
	loadCEGUI();

	CEGUI::MouseCursor::getSingleton().hide();
	m_pGUImgr->m_pGUISystem->setGUISheet(m_pGUIsheet);
	CEGUI::System::getSingleton().signalRedraw();

}
//|||||||||||||||||||||||||||||||||||||||||||||||

void GUIGameComplete::loadCEGUI()
{
	CEGUI::MouseCursor::getSingleton().show();
	m_pGUImgr = OgreFramework::getSingletonPtr()->m_pGUImgr;
	m_pGUIsheet = m_pGUImgr->m_pGUIsheetGameComplete;	 
}


void GUIGameComplete::exit()
{
	//m_pGUImgr->m_pGUISystem->setGUISheet(0);
	CEGUI::MouseCursor::getSingleton().show();
}

void GUIGameComplete::resume(){
	m_pGUImgr->m_pGUISystem->setGUISheet(m_pGUIsheet);
	//por alguna razon no se cambia el GUI sheet hasta que no se mueve el raton
	//por ejemplo, es como si no se actualizara automaticamente, con esto se marca
	//para actualizar en el siguiente frame 
	CEGUI::System::getSingleton().signalRedraw();

}

