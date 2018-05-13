#include <CEGUI.h>

#include "AdvancedOgreFramework.h"
#include "AppStateManager.h"


#include "GUIManager.h"
#include "GUISplashScreen.h"
#include "GUICredits.h"

//|||||||||||||||||||||||||||||||||||||||||||||||

template<> GUISplashScreen* Ogre::Singleton<GUISplashScreen>::msSingleton = 0;

//|||||||||||||||||||||||||||||||||||||||||||||||

GUISplashScreen& GUISplashScreen::getSingleton(){
assert (msSingleton); 
return (*msSingleton);
}
//|||||||||||||||||||||||||||||||||||||||||||||||

GUISplashScreen* GUISplashScreen::getSingletonPtr(){
assert (msSingleton);
return msSingleton;
}
//|||||||||||||||||||||||||||||||||||||||||||||||

GUISplashScreen::GUISplashScreen(){
}
//|||||||||||||||||||||||||||||||||||||||||||||||

GUISplashScreen::~GUISplashScreen(){
	
}
//|||||||||||||||||||||||||||||||||||||||||||||||

void GUISplashScreen::enter()
{	
	loadCEGUI();

	m_pGUImgr->m_pGUISystem->setGUISheet(m_pGUIsheet);
	CEGUI::System::getSingleton().signalRedraw();

}
//|||||||||||||||||||||||||||||||||||||||||||||||

void GUISplashScreen::loadCEGUI()
{
	CEGUI::MouseCursor::getSingleton().hide();
	m_pGUImgr = OgreFramework::getSingletonPtr()->m_pGUImgr;
	m_pGUIsheet = m_pGUImgr->m_pGUIsheetSplashScreen;	 
}
//|||||||||||||||||||||||||||||||||||||||||||||||

void GUISplashScreen::exit()
{
	//m_pGUImgr->m_pGUISystem->setGUISheet(0);
	CEGUI::MouseCursor::getSingleton().show();
}

void GUISplashScreen::resume(){
	CEGUI::MouseCursor::getSingleton().hide();
	m_pGUImgr->m_pGUISystem->setGUISheet(m_pGUIsheet);
	//por alguna razon no se cambia el GUI sheet hasta que no se mueve el raton
	//por ejemplo, es como si no se actualizara automaticamente, con esto se marca
	//para actualizar en el siguiente frame 
	CEGUI::System::getSingleton().signalRedraw();

}




