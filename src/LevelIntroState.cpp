//|||||||||||||||||||||||||||||||||||||||||||||||

#include "LevelIntroState.h"
#include "GUIManager.h"
#include "GUIStateManager.h"
#include "AdvancedOgreFramework.h"
#include "PhysicsManager.h"
#include "Effect.h"
#include "LevelsManager.h"
#include "Fader.h"
#include "GameManager.h"
#include "HUD.h"
#include "AudioManager.h"

LevelIntroState::LevelIntroState()
{
    m_bQuit             = false;
    m_FrameEvent        = Ogre::FrameEvent();
	m_bFading = false;
	m_faderTimer = 0;
}


void LevelIntroState::enter()
{
	m_bFading = false;
	m_faderTimer = 0;

    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Entering LevelIntroState...");

	GUIManager::getSingletonPtr()->m_pGUIStateManager->
		pushGUIState(GUIManager::getSingletonPtr()->m_pGUIStateManager->findByName("GUILevelIntro") );
   
	m_pPhysicsMgr = PhysicsManager::getSingletonPtr();

	m_bQuit = false;


	//activamos la camara con la animacion de la intro del nivel
	m_pCamera  =OgreFramework::getSingletonPtr()->
	m_pActiveSceneMgr->getCamera("LevelIntroCamera");

	m_pCamera->setAspectRatio(Ogre::Real(OgreFramework::getSingletonPtr()->m_pViewport->getActualWidth()) /
        Ogre::Real(OgreFramework::getSingletonPtr()->m_pViewport->getActualHeight()));
	OgreFramework::getSingletonPtr()->m_pViewport->setCamera(m_pCamera);

	//hacemos el fade in de la GUI de la intro
	HUD::getSingletonPtr()->fadeInIntro();
 
}



void LevelIntroState::exit()
{
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Leaving LevelIntroState...");

	GUIManager::getSingletonPtr()->m_pGUIStateManager->popGUIState();

	//cuando salimos volvemos a activar la camara por defecto del juego
	m_pCamera  =OgreFramework::getSingletonPtr()->
		m_pActiveSceneMgr->getCamera("Camera#0");

	m_pCamera->setAspectRatio(Ogre::Real(OgreFramework::getSingletonPtr()->m_pViewport->getActualWidth()) /
        Ogre::Real(OgreFramework::getSingletonPtr()->m_pViewport->getActualHeight()));
	OgreFramework::getSingletonPtr()->m_pViewport->setCamera(m_pCamera);

	m_pCamera = 0;
	m_pPhysicsMgr = 0;
	
	//comenzamos el nivel al salir de la intro
	LevelsManager::getSingletonPtr()->startLevel();

	m_bFading = false;
	m_faderTimer = 0;
}

bool LevelIntroState::keyPressed(const OIS::KeyEvent &keyEventRef)
{
	CEGUI::System::getSingleton().injectKeyDown(keyEventRef.key);

    if( (OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_ESCAPE) 
		|| OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_RETURN)))
    {
		//comenzamos el efecto de fade out si no esta activo
		if(!m_bFading){
			AudioManager::getSingletonPtr()->fadeOutTrack(2);
			HUD::getSingletonPtr()->fadeOutIntro();
			Fader::getSingletonPtr()->startFadeOut(1.8);
			m_faderTimer = 0;
			m_bFading = true;
		}

        return true;
    }

    OgreFramework::getSingletonPtr()->keyPressed(keyEventRef);

    return true;
}


bool LevelIntroState::keyReleased(const OIS::KeyEvent &keyEventRef)
{
	CEGUI::System::getSingleton().injectKeyUp(keyEventRef.key);

    OgreFramework::getSingletonPtr()->keyReleased(keyEventRef);

    return true;
}

bool LevelIntroState::mouseMoved(const OIS::MouseEvent &evt)
{
	CEGUI::System::getSingleton().injectMouseMove(evt.state.X.rel, evt.state.Y.rel);  
	
    return true;
}


bool LevelIntroState::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{	
	if(id == OIS::MB_Left)
		CEGUI::System::getSingleton().injectMouseButtonDown(CEGUI::LeftButton);
	
    return true;
}

bool LevelIntroState::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
	if(id == OIS::MB_Left)
		CEGUI::System::getSingleton().injectMouseButtonUp(CEGUI::LeftButton);
  
    return true;
}


void LevelIntroState::update(double timeSinceLastFrame)
{
    m_FrameEvent.timeSinceLastFrame = timeSinceLastFrame;
	
	//actualiza el efecto de fade out cuando esta activo y cuando termina sale
	//del estado de la intro y pasamos al nivel
	if(m_bFading){
		m_faderTimer += timeSinceLastFrame/1000;
		if(m_faderTimer >= 2)
			m_bQuit = true;
	}

    if(m_bQuit == true)
    {
        popAppState();
        return;
    }
	
	if(m_pPhysicsMgr)
		m_pPhysicsMgr->update(timeSinceLastFrame);

	m_FrameEvent.timeSinceLastFrame = timeSinceLastFrame;
	
	if(m_bQuit == true)
	{
		popAppState();
		return;
	}
	
	HUD::getSingletonPtr()->update(timeSinceLastFrame);

	CEGUI::System::getSingleton().injectTimePulse( m_FrameEvent.timeSinceLastFrame);

	Effect::updateEach(timeSinceLastFrame);
	
	LevelsManager::getSingletonPtr()->update(timeSinceLastFrame);

	CEGUI::System::getSingleton().injectTimePulse( m_FrameEvent.timeSinceLastFrame);
}

