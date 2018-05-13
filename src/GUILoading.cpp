#include <CEGUI.h>

#include "AdvancedOgreFramework.h"
#include "AppStateManager.h"


#include "GUIManager.h"
#include "GUILoading.h"

//|||||||||||||||||||||||||||||||||||||||||||||||

template<> GUILoading* Ogre::Singleton<GUILoading>::msSingleton = 0;

//|||||||||||||||||||||||||||||||||||||||||||||||

GUILoading& GUILoading::getSingleton(){
assert (msSingleton); 
return (*msSingleton);
}
//|||||||||||||||||||||||||||||||||||||||||||||||

GUILoading* GUILoading::getSingletonPtr(){
assert (msSingleton);
return msSingleton;
}
//|||||||||||||||||||||||||||||||||||||||||||||||

GUILoading::GUILoading(){
}
//|||||||||||||||||||||||||||||||||||||||||||||||

GUILoading::~GUILoading(){
	
}
//|||||||||||||||||||||||||||||||||||||||||||||||

void GUILoading::enter()
{	
	loadCEGUI();

	m_pGUImgr->m_pGUISystem->setGUISheet(m_pGUIsheet);
	CEGUI::System::getSingleton().signalRedraw();
}
//|||||||||||||||||||||||||||||||||||||||||||||||

void GUILoading::loadCEGUI()
{
	CEGUI::MouseCursor::getSingleton().hide();
	m_pGUImgr = OgreFramework::getSingletonPtr()->m_pGUImgr;
	m_pGUIsheet = m_pGUImgr->m_pGUIsheetLoading;	 
}
//|||||||||||||||||||||||||||||||||||||||||||||||

void GUILoading::exit()
{
	CEGUI::MouseCursor::getSingleton().show();
}

void GUILoading::resume(){
	//m_pGUImgr->m_pGUISystem->setGUISheet(m_pGUIsheet);
	CEGUI::MouseCursor::getSingleton().hide();
	m_pGUImgr->m_pGUISystem->setGUISheet(m_pGUIsheet);
	//por alguna razon no se cambia el GUI sheet hasta que no se mueve el raton
	//por ejemplo, es como si no se actualizara automaticamente, con esto se marca
	//para actualizar en el siguiente frame 
	CEGUI::System::getSingleton().signalRedraw();
	
}
//|||||||||||||||||||||||||||||||||||||||||||||||

