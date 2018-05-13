#include <CEGUI.h>

#include "AdvancedOgreFramework.h"
#include "AppStateManager.h"
#include "GameManager.h"
#include "AudioManager.h"
#include "Constants.h"
#include "GUIManager.h"
#include "GUIControls.h"

//|||||||||||||||||||||||||||||||||||||||||||||||

template<> GUIControls* Ogre::Singleton<GUIControls>::msSingleton = 0;

//|||||||||||||||||||||||||||||||||||||||||||||||

GUIControls& GUIControls::getSingleton(){
assert (msSingleton); 
return (*msSingleton);
}
//|||||||||||||||||||||||||||||||||||||||||||||||

GUIControls* GUIControls::getSingletonPtr(){
assert (msSingleton);
return msSingleton;
}
//|||||||||||||||||||||||||||||||||||||||||||||||

GUIControls::GUIControls(){
}
//|||||||||||||||||||||||||||||||||||||||||||||||

GUIControls::~GUIControls(){
	
}
//|||||||||||||||||||||||||||||||||||||||||||||||

void GUIControls::enter()
{	
	loadCEGUI();

	m_pGUImgr->m_pGUISystem->setGUISheet(m_pGUIsheet);
	CEGUI::System::getSingleton().signalRedraw();
}
//|||||||||||||||||||||||||||||||||||||||||||||||

void GUIControls::loadCEGUI()
{
	CEGUI::MouseCursor::getSingleton().show();
	m_pGUImgr = OgreFramework::getSingletonPtr()->m_pGUImgr;
	m_pGUIsheet = m_pGUImgr->m_pGUIsheetControls;	 
}
//|||||||||||||||||||||||||||||||||||||||||||||||

void GUIControls::exit()
{
	//m_pGUImgr->m_pGUISystem->setGUISheet(0);
}

void GUIControls::resume(){
	//m_pGUImgr->m_pGUISystem->setGUISheet(m_pGUIsheet);
	CEGUI::MouseCursor::getSingleton().show();
	m_pGUImgr->m_pGUISystem->setGUISheet(m_pGUIsheet);
	//por alguna razon no se cambia el GUI sheet hasta que no se mueve el raton
	//por ejemplo, es como si no se actualizara automaticamente, con esto se marca
	//para actualizar en el siguiente frame 
	CEGUI::System::getSingleton().signalRedraw();
	
}
//|||||||||||||||||||||||||||||||||||||||||||||||

bool GUIControls::onBackButton(const CEGUI::EventArgs &args){
	AudioManager::getSingletonPtr()->playFX("Clicked", 0, MENU1_CHANNEL);
	popGUIState();
	return true;
}


bool GUIControls::onXInvertCheck(const CEGUI::EventArgs &args){


	CEGUI::Checkbox* checkbox = static_cast<CEGUI::Checkbox*>
		(CEGUI::WindowManager::getSingleton().getWindow("ControlsRoot/controlsBG/xInvertCheck"));
	bool value = checkbox->isSelected(); 

	if(value)
		GameConfig::getSingletonPtr()->xInverted = 1;
	else
		GameConfig::getSingletonPtr()->xInverted = -1;
	
	return true;
}

bool GUIControls::onYInvertCheck(const CEGUI::EventArgs &args){
	

	CEGUI::Checkbox* checkbox = static_cast<CEGUI::Checkbox*>
		(CEGUI::WindowManager::getSingleton().getWindow("ControlsRoot/controlsBG/yInvertCheck"));
	bool value = checkbox->isSelected(); 

	if(value)
		GameConfig::getSingletonPtr()->yInverted = 1;
	else
		GameConfig::getSingletonPtr()->yInverted = -1;
	
	return true;
}


bool GUIControls::onSensitivity(const CEGUI::EventArgs &args){
	CEGUI::Spinner* vol = static_cast<CEGUI::Spinner*>
		(CEGUI::WindowManager::getSingleton().getWindow
		("ControlsRoot/controlsBG/sensitivity") );
	int sensitivity = vol->getCurrentValue();

	GameConfig::getSingletonPtr()->mouseSensitivity = sensitivity;

	return true;
}






