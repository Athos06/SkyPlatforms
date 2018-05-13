#include "GameOverState.h"
#include "GUIManager.h"
#include "GameManager.h"
#include "GUIStateManager.h"
#include "AudioManager.h"


GameOverState::GameOverState()
{
    m_bQuit             = false;
    m_FrameEvent        = Ogre::FrameEvent();
}


void GameOverState::enter()
{
	AudioManager::getSingletonPtr()->unloadMainTrack();
	
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Entering GameOverState...");

	//cambiar la GUI
	// si hemos superado el nivel ejecutamos el track de nivel
	//superado y mostramos la GUI de nivel superado
	if(GameManager::getSingletonPtr()->isPassLevel()){
		AudioManager::getSingletonPtr()->playMainTrack("LevelComplete.ogg", 0);

		GUIManager::getSingletonPtr()->m_pGUIStateManager->
			pushGUIState(GUIManager::getSingletonPtr()->m_pGUIStateManager->
			findByName("GUILevelComplete") );
	}
	// si hemos muerto mostramos la GUI de game over y ejecutamos el track
	//de game over
	else{	
		//si no tenemos vidas restantes se ejecuta un track
		if(GameManager::getSingletonPtr()->getLives() <= 0){
			AudioManager::getSingletonPtr()->playMainTrack("GameOver.ogg", 0);
		}
		//si aun tenemos vidas y podemos continuar se ejecuta otro distinto
		else{
			AudioManager::getSingletonPtr()->fadeInTrack("RetryMenu.ogg", 1, 0);
			//AudioManager::getSingletonPtr()->playMainTrack("RetryMenu.ogg", 0);
		}
		GUIManager::getSingletonPtr()->m_pGUIStateManager->
			pushGUIState(GUIManager::getSingletonPtr()->m_pGUIStateManager->
			findByName("GUIGameOver") );
	}	 
    m_bQuit = false;
}



void GameOverState::exit()
{
	//desapilar la GUI del menu de gameOver
	GUIManager::getSingletonPtr()->m_pGUIStateManager->popGUIState();
	
}

bool GameOverState::keyPressed(const OIS::KeyEvent &keyEventRef)
{

	CEGUI::System::getSingleton().injectKeyDown(keyEventRef.key);

    OgreFramework::getSingletonPtr()->keyPressed(keyEventRef);

    return true;
}


bool GameOverState::keyReleased(const OIS::KeyEvent &keyEventRef)
{
	CEGUI::System::getSingleton().injectKeyUp(keyEventRef.key);

    OgreFramework::getSingletonPtr()->keyReleased(keyEventRef);

    return true;
}


bool GameOverState::mouseMoved(const OIS::MouseEvent &evt)
{
	
	CEGUI::System::getSingleton().injectMouseMove(evt.state.X.rel, evt.state.Y.rel);  
	
    return true;
}

bool GameOverState::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{	
	if(id == OIS::MB_Left)
		CEGUI::System::getSingleton().injectMouseButtonDown(CEGUI::LeftButton);
	
    return true;
}


bool GameOverState::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
	if(id == OIS::MB_Left)
		CEGUI::System::getSingleton().injectMouseButtonUp(CEGUI::LeftButton);
  
    return true;
}


void GameOverState::update(double timeSinceLastFrame)
{
    m_FrameEvent.timeSinceLastFrame = timeSinceLastFrame;
  
    if(m_bQuit == true)
    {
        popAppState();
        return;
    }

	CEGUI::System::getSingleton().injectTimePulse( m_FrameEvent.timeSinceLastFrame);
}
