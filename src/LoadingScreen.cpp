#include "LoadingScreen.h"
#include <GUIManager.h>

template<> LoadingScreen* Ogre::Singleton<LoadingScreen>::msSingleton = 0;


LoadingScreen& LoadingScreen::getSingleton(){
	assert (msSingleton); 
	return (*msSingleton);
}

LoadingScreen* LoadingScreen::getSingletonPtr(){
	assert (msSingleton);
	return msSingleton;
}

LoadingScreen::LoadingScreen(){
	m_pLoadingSheet = GUIManager::getSingletonPtr()->m_pGUIsheetLoading;

	m_pLoadingText = CEGUI::WindowManager::getSingleton().getWindow
		("LoadingRoot/background/StaticText");
}

LoadingScreen::~LoadingScreen(){
}

void LoadingScreen::setCompletePercent(int total){
	m_pLoadingText->setText("LOADING "+ Ogre::StringConverter::
		toString(total));
}
