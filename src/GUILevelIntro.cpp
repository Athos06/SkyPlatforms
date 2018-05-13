#include <CEGUI.h>

#include "AdvancedOgreFramework.h"
#include "AppStateManager.h"

#include "GUIManager.h"
#include "GUILevelIntro.h"


//|||||||||||||||||||||||||||||||||||||||||||||||

template<> GUILevelIntro* Ogre::Singleton<GUILevelIntro>::msSingleton = 0;

//|||||||||||||||||||||||||||||||||||||||||||||||

GUILevelIntro& GUILevelIntro::getSingleton(){
assert (msSingleton); 
return (*msSingleton);
}
//|||||||||||||||||||||||||||||||||||||||||||||||

GUILevelIntro* GUILevelIntro::getSingletonPtr(){
assert (msSingleton);
return msSingleton;
}
//|||||||||||||||||||||||||||||||||||||||||||||||

GUILevelIntro::GUILevelIntro(){
}
//|||||||||||||||||||||||||||||||||||||||||||||||

GUILevelIntro::~GUILevelIntro(){
	
}
//|||||||||||||||||||||||||||||||||||||||||||||||

void GUILevelIntro::enter()
{	
	loadCEGUI();

	m_pGUImgr->m_pGUISystem->setGUISheet(m_pGUIsheet);
	CEGUI::System::getSingleton().signalRedraw();

}
//|||||||||||||||||||||||||||||||||||||||||||||||

void GUILevelIntro::loadCEGUI()
{
	CEGUI::MouseCursor::getSingleton().hide();
	m_pGUImgr = OgreFramework::getSingletonPtr()->m_pGUImgr;
	//por alguna razon no se cambia el GUI sheet hasta que no se mueve el raton
	//por ejemplo, es como si no se actualizara automaticamente, con esto se marca
	//para actualizar en el siguiente frame 
	m_pGUIsheet = m_pGUImgr->m_pGUIsheetLevelIntro;	 
}
//|||||||||||||||||||||||||||||||||||||||||||||||

void GUILevelIntro::exit()
{
	//m_pGUImgr->m_pGUISystem->setGUISheet(0);
}

void GUILevelIntro::resume(){
	m_pGUImgr->m_pGUISystem->setGUISheet(m_pGUIsheet);
	//por alguna razon no se cambia el GUI sheet hasta que no se mueve el raton
	//por ejemplo, es como si no se actualizara automaticamente, con esto se marca
	//para actualizar en el siguiente frame 
	CEGUI::System::getSingleton().signalRedraw();
}

//|||||||||||||||||||||||||||||||||||||||||||||||

