//|||||||||||||||||||||||||||||||||||||||||||||||

#include "PauseState.h"
#include "GUIManager.h"
#include "GUIStateManager.h"
 
 

//|||||||||||||||||||||||||||||||||||||||||||||||

using namespace Ogre;

//|||||||||||||||||||||||||||||||||||||||||||||||

PauseState::PauseState()
{
    m_bQuit             = false;
    m_FrameEvent        = Ogre::FrameEvent();
	m_pBackground		= 0;
	m_pNodeBg			= 0;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void PauseState::enter()
{
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Entering PauseState...");

	//cambiar la GUI
	GUIManager::getSingletonPtr()->m_pGUIStateManager->
		pushGUIState(GUIManager::getSingletonPtr()->m_pGUIStateManager->findByName("GUIPauseMenu") );

    m_bQuit = false;

    createScene();
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void PauseState::createScene()
{
	// Create background m_pBackgroundangle covering the whole screen
	m_pBackground = new Ogre::Rectangle2D(true);
	m_pBackground->setCorners(-1.0, 1.0, 1.0, -1.0);
	m_pBackground->setMaterial("PauseBackground");
	// Render the background before everything else
	m_pBackground->setRenderQueueGroup(Ogre::RENDER_QUEUE_MAX);
	// Use infinite AAB to always stay visible
	Ogre::AxisAlignedBox aabInf;
	aabInf.setInfinite();
	m_pBackground->setBoundingBox(aabInf);	 
	// Attach background to the scene
	m_pNodeBg = OgreFramework::getSingletonPtr()->m_pActiveSceneMgr
		->getRootSceneNode()->createChildSceneNode("Background");
	m_pNodeBg->attachObject(m_pBackground);
}


void PauseState::exit()
{
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Leaving PauseState...");
	
	//eliminamos el fondo del pause al salir
	if(m_pBackground)	
		delete m_pBackground;
	if(m_pNodeBg){
		OgreFramework::getSingletonPtr()->m_pActiveSceneMgr
			->destroySceneNode(m_pNodeBg);
	}		

	//desapilar la GUI del menu de pausa, volviendo a la gui del juego	
	GUIManager::getSingletonPtr()->m_pGUIStateManager->popGUIState();
	GUIManager::getSingletonPtr()->m_pGUIStateManager->popAllAndPushGUIState(
		GUIManager::getSingletonPtr()->m_pGUIStateManager->findByName("GUIGame"));
}

bool PauseState::keyPressed(const OIS::KeyEvent &keyEventRef)
{

	CEGUI::System::getSingleton().injectKeyDown(keyEventRef.key);

    if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_ESCAPE))
    {
        m_bQuit = true;
        return true;
    }

    OgreFramework::getSingletonPtr()->keyPressed(keyEventRef);

    return true;
}

bool PauseState::keyReleased(const OIS::KeyEvent &keyEventRef)
{
	CEGUI::System::getSingleton().injectKeyUp(keyEventRef.key);

    OgreFramework::getSingletonPtr()->keyReleased(keyEventRef);

    return true;
}

bool PauseState::mouseMoved(const OIS::MouseEvent &evt)
{
	
	CEGUI::System::getSingleton().injectMouseMove
		(evt.state.X.rel, evt.state.Y.rel);  
    return true;
}

bool PauseState::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{	
	if(id == OIS::MB_Left)
		CEGUI::System::getSingleton().injectMouseButtonDown(CEGUI::LeftButton);
	
    return true;
}

bool PauseState::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
	if(id == OIS::MB_Left)
		CEGUI::System::getSingleton().injectMouseButtonUp(CEGUI::LeftButton);
  
    return true;
}

void PauseState::update(double timeSinceLastFrame)
{
    m_FrameEvent.timeSinceLastFrame = timeSinceLastFrame;
  
    if(m_bQuit == true)
    {
        popAppState();
        return;
    }

	CEGUI::System::getSingleton().injectTimePulse( m_FrameEvent.timeSinceLastFrame);
}

