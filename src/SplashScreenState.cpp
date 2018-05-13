//|||||||||||||||||||||||||||||||||||||||||||||||
#include <CEGUI.h>
#include "AdvancedOgreFramework.h"
#include "AppStateManager.h"
#include "SplashScreenState.h"
#include "GUIManager.h"
#include "GUIStateManager.h"
#include "DotSceneLoader.h"
#include "AudioManager.h"
#include "Constants.h"
#include "Animations.h"

//|||||||||||||||||||||||||||||||||||||||||||||||

using namespace Ogre;

//|||||||||||||||||||||||||||||||||||||||||||||||

SplashScreenState::SplashScreenState()
{
    m_bQuit         = false;
    m_FrameEvent    = Ogre::FrameEvent();
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void SplashScreenState::enter()
{
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Entering SplashScreenState...");
	
    m_pSceneMgr = OgreFramework::getSingletonPtr()->m_pRoot->createSceneManager(ST_GENERIC, "MenuSceneMgr");
	OgreFramework::getSingletonPtr()->m_pActiveSceneMgr = m_pSceneMgr;
	AudioManager::getSingletonPtr()->playMainTrack("MainMenu.ogg");

	//activar la GUI de la splash screen
	GUIManager::getSingletonPtr()->m_pGUIStateManager->
		changeGUIState( GUIManager::getSingletonPtr()->
		m_pGUIStateManager->findByName("GUISplashScreen") );

    createScene();
}

void SplashScreenState::createScene()
{		

	m_pSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

	DotSceneLoader loader;
	//carga de la escena
	loader.parseDotScene("MainMenu.scene", "General", m_pSceneMgr);

	//Camara estatica para la splash screen
	m_pCamera = m_pSceneMgr->getCamera("SplashScreenCamera");

	
	OgreFramework::getSingletonPtr()->m_pViewport->setCamera(m_pCamera);
	OgreFramework::getSingletonPtr()->m_pViewport->setBackgroundColour(Ogre::ColourValue(0,0,0));
	m_pCamera->setAspectRatio(Ogre::Real(OgreFramework::getSingletonPtr()->m_pViewport->getActualWidth()) /
        Ogre::Real(OgreFramework::getSingletonPtr()->m_pViewport->getActualHeight()));	
}
//|||||||||||||||||||||||||||||||||||||||||||||||

void SplashScreenState::exit()
{	

	OgreFramework::getSingletonPtr()->m_pLog->logMessage("Leaving SplashScreenState...");

	AudioManager::getSingletonPtr()->unloadMainTrack();
    
	Animations::destroyAllAnimations();

	m_pSceneMgr->destroyCamera(m_pCamera);

	if(m_pSceneMgr){
        OgreFramework::getSingletonPtr()->m_pRoot->destroySceneManager(m_pSceneMgr);
		OgreFramework::getSingletonPtr()->m_pActiveSceneMgr = 0;
	}
	
}

void SplashScreenState::resume(){
	OgreFramework::getSingletonPtr()->m_pLog->logMessage("Resuming SplasScreenState...");
	OgreFramework::getSingletonPtr()->m_pViewport->setCamera(m_pCamera);
	m_bQuit = false;
}

bool SplashScreenState::keyPressed(const OIS::KeyEvent &keyEventRef)
{
	if(keyEventRef.key == OIS::KC_ESCAPE){
		m_bQuit = true;
        return true;
	}

    if(keyEventRef.key == OIS::KC_RETURN || keyEventRef.key == OIS::KC_SPACE){
		//sonido de entrar en el menu
		AudioManager::getSingletonPtr()->playFX("SkipSplash", 0, MENU1_CHANNEL);
		OgreFramework::getSingletonPtr()->m_pAppStateManager->
			pushAppState(OgreFramework::getSingletonPtr()->
			m_pAppStateManager->findByName("MenuState"));
    }

    OgreFramework::getSingletonPtr()->keyPressed(keyEventRef);
    return true;
}

bool SplashScreenState::keyReleased(const OIS::KeyEvent &keyEventRef)
{
    OgreFramework::getSingletonPtr()->keyReleased(keyEventRef);
    return true;
}

bool SplashScreenState::mouseMoved(const OIS::MouseEvent &evt)
{
	CEGUI::System::getSingleton().injectMouseMove(evt.state.X.rel, evt.state.Y.rel);  
    return true;
}

bool SplashScreenState::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
	if(id == OIS::MB_Left)
		CEGUI::System::getSingleton().injectMouseButtonDown(CEGUI::LeftButton);
    return true;
}

bool SplashScreenState::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{

	if(id == OIS::MB_Left)
		CEGUI::System::getSingleton().injectMouseButtonUp(CEGUI::LeftButton);
    return true;
	
}

void SplashScreenState::update(double timeSinceLastFrame)
{	   
	m_FrameEvent.timeSinceLastFrame = timeSinceLastFrame;
	
    if(m_bQuit == true)
    {
		//La splashscreen es el primer estado, cuando salimos ya cerramos
		//tambien el juego
        shutdown();
        return;
    }

	CEGUI::System::getSingleton().injectTimePulse( m_FrameEvent.timeSinceLastFrame);
}

