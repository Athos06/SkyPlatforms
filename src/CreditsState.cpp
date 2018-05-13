//|||||||||||||||||||||||||||||||||||||||||||||||

#include <Ogre.h>
#include "DotSceneLoader.h"
#include "CreditsState.h"

#include "Constants.h"

#include "GUIManager.h"
#include "GUIStateManager.h"
#include "AppStateManager.h"

#include "LoadingScreen.h"
#include "GUICredits.h"

#include "AudioManager.h"

//|||||||||||||||||||||||||||||||||||||||||||||||

CreditsState::CreditsState()
{
    m_bQuit             = false;
}
//|||||||||||||||||||||||||||||||||||||||||||||||

CreditsState::~CreditsState(){

}
//|||||||||||||||||||||||||||||||||||||||||||||||

void CreditsState::enter()
{
	m_bQuit = false;
	
	AudioManager::getSingletonPtr()->unloadMainTrack();

	GUIManager::getSingletonPtr()->m_pGUIStateManager->
		changeGUIState(GUIManager::getSingletonPtr()->m_pGUIStateManager->findByName("GUILoading") );
	
	//pantalla de carga
	GUIManager::getSingletonPtr()->m_pLoadingScreen->setCompletePercent(25);
	OgreFramework::getSingletonPtr()->m_pRoot->renderOneFrame();
	
	//una pequena clase que se encarga de actualizar y 
	//gestionar las transiciones de los creditos
	new CreditsHelper;

	 m_pSceneMgr = OgreFramework::getSingletonPtr()->
		 m_pRoot->createSceneManager(Ogre::ST_GENERIC, "MenuSceneMgr");
	OgreFramework::getSingletonPtr()->m_pActiveSceneMgr = m_pSceneMgr;
	
	//pantalla de carga
	GUIManager::getSingletonPtr()->m_pLoadingScreen->setCompletePercent(65);
	OgreFramework::getSingletonPtr()->m_pRoot->renderOneFrame();

	AudioManager::getSingletonPtr()->playMainTrack("Credits.ogg", 0);
	
	//pantalla de carga
	GUIManager::getSingletonPtr()->m_pLoadingScreen->setCompletePercent(100);
	OgreFramework::getSingletonPtr()->m_pRoot->renderOneFrame();

	GUIManager::getSingletonPtr()->m_pGUIStateManager->
		pushGUIState( GUIManager::getSingletonPtr()->
		m_pGUIStateManager->findByName("GUICredits") );

    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Entering CreditsState...");

	createScene();
}
//|||||||||||||||||||||||||||||||||||||||||||||||

void CreditsState::createScene(){


	DotSceneLoader loader;
	loader.parseDotScene("Credits.scene", "General", m_pSceneMgr);

	m_pSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

	//Camara estatica solo para mostrar los creditos
	m_pCamera = m_pSceneMgr->getCamera("Camera#0");
	OgreFramework::getSingletonPtr()->m_pViewport->setCamera(m_pCamera);
	OgreFramework::getSingletonPtr()->m_pViewport->setBackgroundColour(Ogre::ColourValue(0,0,0));
	m_pCamera->setAspectRatio(Ogre::Real(OgreFramework::getSingletonPtr()->m_pViewport->getActualWidth()) /
        Ogre::Real(OgreFramework::getSingletonPtr()->m_pViewport->getActualHeight()));	
}
//|||||||||||||||||||||||||||||||||||||||||||||||

bool CreditsState::pause()
{
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Pausing CreditsState...");
    return true;
}
//|||||||||||||||||||||||||||||||||||||||||||||||

void CreditsState::resume()
{
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Resuming CreditsState...");

    m_bQuit = false;
}
//|||||||||||||||||||||||||||||||||||||||||||||||

void CreditsState::exit()
{
	m_bQuit = false;

    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Leaving CreditsState...");
	
	delete CreditsHelper::getSingletonPtr();

	m_pSceneMgr->destroyCamera(m_pCamera);

	if(m_pSceneMgr){
        OgreFramework::getSingletonPtr()->m_pRoot->destroySceneManager(m_pSceneMgr);
		OgreFramework::getSingletonPtr()->m_pActiveSceneMgr = 0;
	}


}
//|||||||||||||||||||||||||||||||||||||||||||||||

bool CreditsState::keyPressed(const OIS::KeyEvent &keyEventRef)
{

	CEGUI::System::getSingleton().injectKeyDown(keyEventRef.key);

	if(keyEventRef.key == OIS::KC_ESCAPE || keyEventRef.key == OIS::KC_RETURN){
		CreditsHelper::getSingletonPtr()->exitCredits();
        return true;
    }	

    return true;
}
//|||||||||||||||||||||||||||||||||||||||||||||||

bool CreditsState::keyReleased(const OIS::KeyEvent &keyEventRef)
{
	
    return true;
}
//|||||||||||||||||||||||||||||||||||||||||||||||


bool CreditsState::mouseMoved(const OIS::MouseEvent &evt)
{

	float deltaTime = m_FrameEvent.timeSinceLastFrame/1000;

	CEGUI::System::getSingleton().injectMouseMove(evt.state.X.rel, evt.state.Y.rel);  
    
    return true;
}
//|||||||||||||||||||||||||||||||||||||||||||||||

bool CreditsState::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{

    if(id == OIS::MB_Left)
    {
		CEGUI::System::getSingleton().injectMouseButtonDown(CEGUI::LeftButton);
    }
    else if(id == OIS::MB_Right)
    {
		CEGUI::System::getSingleton().injectMouseButtonDown(CEGUI::RightButton);
    }

    return true;
}
//|||||||||||||||||||||||||||||||||||||||||||||||

bool CreditsState::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    if(id == OIS::MB_Left)
    {
		CEGUI::System::getSingleton().injectMouseButtonUp(CEGUI::LeftButton);
    }
    else if(id == OIS::MB_Right)
    {
		CEGUI::System::getSingleton().injectMouseButtonUp(CEGUI::RightButton);
    }

    return true;
}
//|||||||||||||||||||||||||||||||||||||||||||||||


void CreditsState::update(double timeSinceLastFrame)
{

	m_FrameEvent.timeSinceLastFrame = timeSinceLastFrame;
	
	if(m_bQuit == true || CreditsHelper::getSingletonPtr()->hasFinished())
	{
		//cuando salimos de los creditos siempre volvemos al menu principal
		popAllAndPushAppState(OgreFramework::getSingletonPtr()->m_pAppStateManager->
			findByName("SplashScreenState"));
		//popAppState();
		return;
	}

	CEGUI::System::getSingleton().injectTimePulse( m_FrameEvent.timeSinceLastFrame);
	//actualizamos la clase auxiliar que se encarga del efecto de los creditos
	CreditsHelper::getSingletonPtr()->update(timeSinceLastFrame);

}
