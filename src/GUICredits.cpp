#include <CEGUI.h>

#include "AdvancedOgreFramework.h"
#include "AppStateManager.h"
#include "AudioManager.h"
#include "GUICredits.h"
#include "GUIManager.h"

#include <fstream>
#include <iostream>
#include <string>
#include <string.h>

#include <stdio.h>

template<> GUICredits* Ogre::Singleton<GUICredits>::msSingleton = 0;

GUICredits& GUICredits::getSingleton(){
assert (msSingleton); 
return (*msSingleton);
}
 
GUICredits* GUICredits::getSingletonPtr(){
assert (msSingleton);
return msSingleton;
}


GUICredits::GUICredits(){}

GUICredits::~GUICredits(){
	
}
void GUICredits::enter()
{
	
	loadCEGUI();
	m_pGUImgr->m_pGUISystem->setGUISheet(m_pGUIsheet);
	//por alguna razon no se cambia el GUI sheet hasta que no se mueve el raton
	//por ejemplo, es como si no se actualizara automaticamente, con esto se marca
	//para actualizar en el siguiente frame 
	CEGUI::System::getSingleton().signalRedraw();
	
	
}
void GUICredits::loadCEGUI()
{
	CEGUI::MouseCursor::getSingleton().hide();
	m_pGUImgr = OgreFramework::getSingletonPtr()->m_pGUImgr;
	m_pGUIsheet = m_pGUImgr->m_pGUIsheetCredits;

	 
}
void GUICredits::exit()
{
	m_pGUImgr->m_pGUISystem->setGUISheet(0);
}

bool GUICredits::onBackButton(const CEGUI::EventArgs &args)
{
	popGUIState();
	return true;
}

///################################################################


template<> CreditsHelper* Ogre::Singleton<CreditsHelper>::msSingleton = 0;

CreditsHelper& CreditsHelper::getSingleton(){
assert (msSingleton); 
return (*msSingleton);
}
 
CreditsHelper* CreditsHelper::getSingletonPtr(){
assert (msSingleton);
return msSingleton;
}

CreditsHelper::~CreditsHelper(){
	m_creditsGUIsheet->setAlpha(1);
	m_creditsGUIsheet = 0;
	m_creditsText.clear();
}
CreditsHelper::CreditsHelper(){
	m_bFinished = false;
	m_bStop = false;
	m_bFadeIn = true;
	m_bFadeOut = false;
	m_keepThere	= false;
	m_bExit = false;
	m_timer = 0;
	m_activeWindow = 0;
	m_alphaValue	= 0;
	
	loadCredits();

}


void CreditsHelper::loadCredits(){
	m_creditsGUIsheet = GUIManager::getSingletonPtr()->m_pGUIsheetCredits;

	CEGUI::Window *temp = CEGUI::WindowManager::getSingleton().
		getWindow("CreditsRoot/credits0");
	temp->setVisible(false);
	m_creditsText.push_back(temp);

	temp = CEGUI::WindowManager::getSingleton().
		getWindow("CreditsRoot/credits1");
	temp->setVisible(false);
	m_creditsText.push_back(temp);

	temp = CEGUI::WindowManager::getSingleton().
		getWindow("CreditsRoot/credits2");
	temp->setVisible(false);
	m_creditsText.push_back(temp);

	temp = CEGUI::WindowManager::getSingleton().
		getWindow("CreditsRoot/credits3");
	temp->setVisible(false);
	m_creditsText.push_back(temp);

	temp = CEGUI::WindowManager::getSingleton().
		getWindow("CreditsRoot/credits4");
	temp->setVisible(false);
	m_creditsText.push_back(temp);

	temp = CEGUI::WindowManager::getSingleton().
		getWindow("CreditsRoot/credits5");
	temp->setVisible(false);
	m_creditsText.push_back(temp);

	temp = CEGUI::WindowManager::getSingleton().
		getWindow("CreditsRoot/credits6");
	temp->setVisible(false);
	m_creditsText.push_back(temp);

	temp = CEGUI::WindowManager::getSingleton().
		getWindow("CreditsRoot/credits7");
	temp->setVisible(false);
	m_creditsText.push_back(temp);

	temp = CEGUI::WindowManager::getSingleton().
		getWindow("CreditsRoot/credits11");
	temp->setVisible(false);
	m_creditsText.push_back(temp);

	temp = CEGUI::WindowManager::getSingleton().
		getWindow("CreditsRoot/credits8");
	temp->setVisible(false);
	m_creditsText.push_back(temp);

	temp = CEGUI::WindowManager::getSingleton().
		getWindow("CreditsRoot/credits9");
	temp->setVisible(false);
	m_creditsText.push_back(temp);

	temp = CEGUI::WindowManager::getSingleton().
		getWindow("CreditsRoot/credits10");
	temp->setVisible(false);
	m_creditsText.push_back(temp);
	
	
}

void CreditsHelper::update(double deltaTime){
	
	m_creditsText[m_activeWindow]->setVisible(true);
	
	if(m_bExit){
		m_timer += deltaTime/1000;
		m_alphaValue -= 0.9* deltaTime/1000;
		
		if(m_alphaValue < 0){
				m_alphaValue = 0;
		}

		m_creditsGUIsheet->setAlpha(m_alphaValue);

		if(m_timer >= 2)
			m_bFinished = true;

		return;
		
	}

	if(m_bStop){
		exitCredits();
		return;
	}

	if(m_keepThere){
		m_timer += deltaTime;
		if(m_timer >= 3000){
			m_keepThere = false;
			m_bFadeOut = true;
			m_timer = 0;
			
			if(m_activeWindow == m_creditsText.size()-1)
				m_bStop = true;
		}
	}
	else{
		if(m_bFadeIn){
			m_alphaValue += 0.7* deltaTime/1000;
			if(m_alphaValue > 1){
				m_alphaValue = 1;
				if(!m_keepThere){
					m_bFadeIn = false;
					m_keepThere = true;
				}
			}
			m_creditsText[m_activeWindow]->setAlpha(m_alphaValue);

		}
		if(m_bFadeOut){
			m_alphaValue -= 0.7* deltaTime/1000;
			if(m_alphaValue < 0){
				m_alphaValue = 0;
				if(!m_keepThere){
					m_bFadeOut = false;
					m_keepThere = false;
					m_bFadeIn = true;
					if(m_activeWindow < m_creditsText.size()-1)
						m_activeWindow++;
						
				}
			}
			m_creditsText[m_activeWindow]->setAlpha(m_alphaValue);
		}
	}
}

void CreditsHelper::exitCredits(){
	if(!m_bExit){
		AudioManager::getSingletonPtr()->fadeOutTrack(2);
		m_timer = 0;
		m_bExit = true;
		m_alphaValue = 1;
	}
}