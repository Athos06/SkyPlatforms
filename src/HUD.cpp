#include "HUD.h"
#include "GameManager.h"
#include "GUIManager.h"
#include "Constants.h"


template <> HUD* Ogre::Singleton<HUD>::msSingleton = 0;

HUD& HUD::getSingleton(){
	assert (msSingleton);
	return (*msSingleton);
}

HUD* HUD::getSingletonPtr(){
	assert (msSingleton);
	return msSingleton;
}

HUD::HUD(){
	m_bFadeIn			= false;
	m_bFadeOut			= false;
	m_IntroAlphaValue	= 0;
	m_alphaValue		= 0;
	m_fadeTime			= 0;
	m_bIntroFadeIn		= false;
	m_bIntroFadeOut		= false;

	m_pHUDsheet = GUIManager::getSingletonPtr()->m_pGUIsheetInGame;

	//inicializamos todas las variables de punteros a los widgets correspondientes
	//de la GUI
	m_nLives = CEGUI::WindowManager::getSingleton().getWindow("HUDroot/livesText");
	m_nStones = CEGUI::WindowManager::getSingleton().getWindow("HUDroot/skystonesText");

	m_nLives->setText(Ogre::StringConverter::toString(0));
	m_nStones->setText(Ogre::StringConverter::toString(0));
	
	m_innactiveMsg = 
		CEGUI::WindowManager::getSingleton().getWindow("HUDroot/innactiveWarning");
	m_openMsg = 
		CEGUI::WindowManager::getSingleton().getWindow("HUDroot/activePortalText");
	
	//solo mostramos las widget necesarios en cada momento
	m_innactiveMsg->setVisible(false);
	m_openMsg->setVisible(false);

	CEGUI::Window* skystones = 
		CEGUI::WindowManager::getSingleton().getWindow("HUDroot/skystones");
	skystones->setVisible(true);
	m_nStones->setVisible(true);

}

HUD::~HUD(){
}


void HUD::reset(){
	m_bFadeIn			= false;
	m_bFadeOut			= false;
	m_IntroAlphaValue	= 0;
	m_alphaValue		= 0;
	m_fadeTime			= 0;
	m_bIntroFadeIn		= false;
	m_bIntroFadeOut		= false;

	m_pHUDsheet = GUIManager::getSingletonPtr()->m_pGUIsheetInGame;

	m_nLives->setText(Ogre::StringConverter::toString(0));
	m_nStones->setText(Ogre::StringConverter::toString(0));

	m_innactiveMsg->setVisible(false);
	m_openMsg->setVisible(false);

	CEGUI::Window* skystones = 
		CEGUI::WindowManager::getSingleton().getWindow("HUDroot/skystones");
	skystones->setVisible(true);
	m_nStones->setVisible(true);

}


void HUD::showOpenMsg(bool show){
	m_openMsg->setVisible(show);
}



void HUD::showHUD(bool show){
	m_pHUDsheet->setVisible(show);
}

void HUD::showMsg(Ogre::String message){
	m_innactiveMsg->setVisible(true);
}


void HUD::hideMsg(){
	m_innactiveMsg->setVisible(false);
}


void HUD::fadeIn(){
	
	m_alphaValue = 0;
	m_bFadeIn = true;
	m_pHUDsheet->setAlpha(m_alphaValue);
	m_pHUDsheet->setVisible(true);
}


void HUD::fadeOut(){
	m_alphaValue = 1;
	m_bFadeOut = true;
}


void HUD::update(double deltaTime){
	//si estamos haciendo un fadein o un fade out del HUD actualizamos
	//su valor de transparencia en cada frame
	if(m_bFadeOut){
		m_alphaValue -= 0.4 * deltaTime/1000;
		if(m_alphaValue < 0){
			m_alphaValue = 0;
			m_bFadeOut = false;
		}
		m_pHUDsheet->setAlpha(m_alphaValue);
	}
	if(m_bFadeIn){
		m_alphaValue += 0.4 * deltaTime/1000;
		if(m_alphaValue > 1){
			m_alphaValue = 1;
			m_bFadeIn = false;
		}
		m_pHUDsheet->setAlpha(m_alphaValue);
	}
	//igual que con el HUD pero en este caso con la GUI de 
	//la intro del nivel
	if(	m_bIntroFadeIn){
		m_IntroAlphaValue += 0.3 * deltaTime/1000;
		if(m_IntroAlphaValue > 1){
			m_IntroAlphaValue = 1;
			m_bIntroFadeIn = false;
		}
		GUIManager::getSingletonPtr()->m_pGUIsheetLevelIntro->
			setAlpha(m_IntroAlphaValue);
	}

	if(m_bIntroFadeOut){
		m_IntroAlphaValue -= 0.3 * deltaTime/1000;
		if(m_IntroAlphaValue < 0){
			m_IntroAlphaValue = 0;
			m_bIntroFadeIn = false;
		}
		GUIManager::getSingletonPtr()->m_pGUIsheetLevelIntro->
			setAlpha(m_IntroAlphaValue);
	}
	
	//actualizamos los indicadores del HUD
	m_nLives->setText(Ogre::StringConverter::toString(
		GameManager::getSingletonPtr()->getLives()));
	m_nStones->setText(Ogre::StringConverter::toString(
		GameManager::getSingletonPtr()->getSkystones()));

}


void HUD::selectIntro(int level){

	CEGUI::Window* level1IntroTittle = 
		CEGUI::WindowManager::getSingleton().getWindow("LevelIntroRoot/Level1Tittle");
	CEGUI::Window* level2IntroTittle = 
		CEGUI::WindowManager::getSingleton().getWindow("LevelIntroRoot/Level2Tittle");
	CEGUI::Window* level3IntroTittle =
		CEGUI::WindowManager::getSingleton().getWindow("LevelIntroRoot/Level3Tittle");
	CEGUI::Window* levelBossIntroTittle = 
		CEGUI::WindowManager::getSingleton().getWindow("LevelIntroRoot/LevelBossTittle");

	CEGUI::Window* levelIntroInstructions =
		CEGUI::WindowManager::getSingleton().getWindow("LevelIntroRoot/instructionsLvl");
	CEGUI::Window* bossIntroInstructions=
		CEGUI::WindowManager::getSingleton().getWindow("LevelIntroRoot/instructionsBoss");

	level1IntroTittle->setVisible(false);
	level2IntroTittle->setVisible(false);
	level3IntroTittle->setVisible(false);
	levelBossIntroTittle->setVisible(false);

	levelIntroInstructions->setVisible(false);
	bossIntroInstructions->setVisible(false);
	
	//mostramos la GUI de la intro correspondiente segun el nivel en el que estamos
	switch(level){
		case INTRO_LEVEL1:
			level1IntroTittle->setVisible(true);
			levelIntroInstructions->setVisible(true);
			break;
		case INTRO_LEVEL2:
			level2IntroTittle->setVisible(true);
			levelIntroInstructions->setVisible(true);

			break;
		case INTRO_LEVEL3:
			level3IntroTittle->setVisible(true);
			levelIntroInstructions->setVisible(true);
			break;
		case INTRO_LEVELBOSS:
			levelBossIntroTittle->setVisible(true);
			bossIntroInstructions->setVisible(true);
			break;
	}
}


void HUD::fadeInIntro(){
	GUIManager::getSingletonPtr()->m_pGUIsheetLevelIntro->setAlpha(0);
	m_bIntroFadeIn = true;
	m_IntroAlphaValue = 0;

}

void HUD::fadeOutIntro(){
	GUIManager::getSingletonPtr()->m_pGUIsheetLevelIntro->setAlpha(1);
	m_bIntroFadeOut = true;
	m_IntroAlphaValue = 1; 
}


void HUD::showBossHUD(){
	m_nStones->setVisible(false);

	CEGUI::Window* skystones = 
		CEGUI::WindowManager::getSingleton().getWindow("HUDroot/skystones");
	skystones->setVisible(false);
	
}
