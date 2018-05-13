#include <CEGUI.h>

#include "AdvancedOgreFramework.h"
#include "AppStateManager.h"

#include "AudioManager.h"
#include "Constants.h"
#include "GUIManager.h"
#include "GUIOptions.h"

//|||||||||||||||||||||||||||||||||||||||||||||||

template<> GUIOptions* Ogre::Singleton<GUIOptions>::msSingleton = 0;

//|||||||||||||||||||||||||||||||||||||||||||||||

GUIOptions& GUIOptions::getSingleton(){
assert (msSingleton); 
return (*msSingleton);
}
//|||||||||||||||||||||||||||||||||||||||||||||||

GUIOptions* GUIOptions::getSingletonPtr(){
assert (msSingleton);
return msSingleton;
}
//|||||||||||||||||||||||||||||||||||||||||||||||

GUIOptions::GUIOptions(){
}
//|||||||||||||||||||||||||||||||||||||||||||||||

GUIOptions::~GUIOptions(){
	
}
//|||||||||||||||||||||||||||||||||||||||||||||||

void GUIOptions::enter()
{	
	loadCEGUI();

	m_pGUImgr->m_pGUISystem->setGUISheet(m_pGUIsheet);
	CEGUI::System::getSingleton().signalRedraw();
}
//|||||||||||||||||||||||||||||||||||||||||||||||

void GUIOptions::loadCEGUI()
{
	CEGUI::MouseCursor::getSingleton().show();
	m_pGUImgr = OgreFramework::getSingletonPtr()->m_pGUImgr;
	m_pGUIsheet = m_pGUImgr->m_pGUIsheetOptions;	 
}
//|||||||||||||||||||||||||||||||||||||||||||||||

void GUIOptions::exit()
{
	//m_pGUImgr->m_pGUISystem->setGUISheet(0);
}

void GUIOptions::resume(){
	CEGUI::MouseCursor::getSingleton().show();
	m_pGUImgr->m_pGUISystem->setGUISheet(m_pGUIsheet);
	//por alguna razon no se cambia el GUI sheet hasta que no se mueve el raton
	//por ejemplo, es como si no se actualizara automaticamente, con esto se marca
	//para actualizar en el siguiente frame 
	CEGUI::System::getSingleton().signalRedraw();
	
}
//|||||||||||||||||||||||||||||||||||||||||||||||

bool GUIOptions::onAudioButton(const CEGUI::EventArgs &args){
	AudioManager::getSingletonPtr()->playFX("Clicked", 0, MENU1_CHANNEL);
	pushGUIState(findByName("GUIAudio"));
	return true;
}

bool GUIOptions::onControlsButton(const CEGUI::EventArgs &args){
	AudioManager::getSingletonPtr()->playFX("Clicked", 0, MENU1_CHANNEL);
	pushGUIState(findByName("GUIControls"));
	return true;
}

bool GUIOptions::onBackButton(const CEGUI::EventArgs &args){
	AudioManager::getSingletonPtr()->playFX("Clicked", 0, MENU1_CHANNEL);
	popGUIState();
	return true;
}