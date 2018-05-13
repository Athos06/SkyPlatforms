//|||||||||||||||||||||||||||||||||||||||||||||||
#include <CEGUI.h>

#include "MenuState.h"
#include "GUIManager.h"
#include "GUIStateManager.h"
#include "DotSceneLoader.h"
#include "AudioManager.h"
#include "Constants.h"
#include "Animations.h"


MenuState::MenuState()
{
    m_bQuit         = false;
    m_FrameEvent    = Ogre::FrameEvent();
}

void MenuState::enter()
{
	m_bQuit = false;
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Entering MenuState...");
	
	//el scene manager es el mismo que se creo en la splash screen
	//asi que no se crea de nuevo
	m_pSceneMgr = OgreFramework::getSingletonPtr()->m_pActiveSceneMgr;

	//activar la GUI del main menu
	GUIManager::getSingletonPtr()->m_pGUIStateManager->
		pushGUIState( GUIManager::getSingletonPtr()->
		m_pGUIStateManager->findByName("GUIMainMenu") );

    createScene();
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void MenuState::createScene()
{		
	//reiniciamos todas las animaciones para que cada vez que se vuelva al 
	//menu este todo desde el principio
	Animations::resetEach();
	m_pSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

	//obtener la camara con la animacion para el menu
	m_pCamera = m_pSceneMgr->getCamera("Camera#0");
	OgreFramework::getSingletonPtr()->m_pViewport->setCamera(m_pCamera);
	OgreFramework::getSingletonPtr()->m_pViewport->setBackgroundColour(Ogre::ColourValue(0,0,0));
	m_pCamera->setAspectRatio(Ogre::Real(OgreFramework::getSingletonPtr()->m_pViewport->getActualWidth()) /
        Ogre::Real(OgreFramework::getSingletonPtr()->m_pViewport->getActualHeight()));	

	//para reiniciar a la posicion inicial cada vez que entramos en el menu (por si hemos vuelto
	//a la splash screen y de nuevo entramos al menu)
	m_pCamNode = m_pSceneMgr->getSceneNode("MainCameraNode");
	m_pCamNode->setPosition(0,0,0);
	m_pCamNode->setOrientation(Ogre::Quaternion::IDENTITY);
	m_pCamNode->setInitialState();
}

void MenuState::exit()
{	
	OgreFramework::getSingletonPtr()->m_pLog->logMessage("Leaving MenuState...");

	GUIManager::getSingletonPtr()->m_pGUIStateManager->popAllAndPushGUIState(
		GUIManager::getSingletonPtr()->m_pGUIStateManager->findByName("GUISplashScreen"));
	
	Animations::resetEach();
	//al salir del main menu no se destruyen ni la camara ni el scene manager
	//porque es compartido con el del splash screen y se destruyen al salir de este
	
}


bool MenuState::keyPressed(const OIS::KeyEvent &keyEventRef)
{
    if(keyEventRef.key == OIS::KC_ESCAPE)
    {
		m_bQuit = true;
        return true;
    }

    OgreFramework::getSingletonPtr()->keyPressed(keyEventRef);
    return true;
}


bool MenuState::keyReleased(const OIS::KeyEvent &keyEventRef)
{
    OgreFramework::getSingletonPtr()->keyReleased(keyEventRef);
    return true;
}


bool MenuState::mouseMoved(const OIS::MouseEvent &evt)
{
	CEGUI::System::getSingleton().injectMouseMove(evt.state.X.rel, evt.state.Y.rel);  
    return true;
}


bool MenuState::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
	if(id == OIS::MB_Left)
		CEGUI::System::getSingleton().injectMouseButtonDown(CEGUI::LeftButton);
    return true;
}

bool MenuState::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{

	if(id == OIS::MB_Left)
		CEGUI::System::getSingleton().injectMouseButtonUp(CEGUI::LeftButton);
    return true;
	
}

void MenuState::update(double timeSinceLastFrame)
{	   
	m_FrameEvent.timeSinceLastFrame = timeSinceLastFrame;
	
	Animations::updateEach(timeSinceLastFrame);

    if(m_bQuit == true)
    {
        popAppState();
        return;
    }

	CEGUI::System::getSingleton().injectTimePulse( m_FrameEvent.timeSinceLastFrame);
}
//|||||||||||||||||||||||||||||||||||||||||||||||

