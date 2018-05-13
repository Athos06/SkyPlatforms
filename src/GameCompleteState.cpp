//|||||||||||||||||||||||||||||||||||||||||||||||
#include "GameCompleteState.h"
#include "AdvancedOgreFramework.h"
#include "DotSceneLoader.h"
#include "AppStateManager.h"
#include "GUIManager.h"
#include "GameManager.h"
#include "GUIStateManager.h"
#include "AudioManager.h"
#include "Animations.h"
#include "CharacterAnimation.h"
#include "Fader.h"
#include "Effect.h"
//|||||||||||||||||||||||||||||||||||||||||||||||

using namespace Ogre;

//|||||||||||||||||||||||||||||||||||||||||||||||

GameCompleteState::GameCompleteState()
{
	m_pSceneMgr			= 0;
	m_pFader			= 0;
	m_faderTimer		= 0;
	m_bFading			= false;
    m_bQuit             = false;
    m_FrameEvent        = Ogre::FrameEvent();
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void GameCompleteState::enter()
{

	AudioManager::getSingletonPtr()->unloadMainTrack();
	AudioManager::getSingletonPtr()->playMainTrack("GameComplete.ogg");

    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Entering GameCompleteState...");

	 m_pSceneMgr = OgreFramework::getSingletonPtr()->m_pRoot
		 ->createSceneManager(ST_GENERIC, "GameCompleteSceneMgr");
	OgreFramework::getSingletonPtr()->m_pActiveSceneMgr = m_pSceneMgr;

	m_pFader = new Fader("Overlays/FadeInOut", "Materials/OverlayMaterial");

	 //cambiar la GUI
	GUIManager::getSingletonPtr()->m_pGUIStateManager->
		pushGUIState(GUIManager::getSingletonPtr()->m_pGUIStateManager->
		findByName("GUIGameComplete") );

    m_bQuit = false;

    createScene();
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void GameCompleteState::createScene()
{

	
	m_pSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

	DotSceneLoader loader;
	//carga de la escena
	loader.parseDotScene("GameComplete.scene", "General", m_pSceneMgr);

	m_pCamera = m_pSceneMgr->getCamera("MainCamera");

	OgreFramework::getSingletonPtr()->m_pViewport->setCamera(m_pCamera);
	OgreFramework::getSingletonPtr()->m_pViewport->setBackgroundColour(Ogre::ColourValue(0,0,0));
	m_pCamera->setAspectRatio(Ogre::Real(OgreFramework::getSingletonPtr()->m_pViewport->getActualWidth()) /
        Ogre::Real(OgreFramework::getSingletonPtr()->m_pViewport->getActualHeight()));
	//iniciar la animacion del personaje.
	//Esta escena es un caso especial, solo mostramos al personaje ejecutando
	//la animacion, no necesitamos controlar nada mas de el
	m_pCharAnim = new CharacterAnimation(m_pSceneMgr->getEntity("Character"));
	m_pCharAnim->setupAnimations();
	m_pCharAnim->setAnimation(CharacterAnimation::ANIM_RESTGAMECOMPLETE);
	
	//comenzamos un fade in al entrar en la escena
	Fader::getSingletonPtr()->startFadeIn(2);
}


void GameCompleteState::exit()
{

	m_faderTimer		= 0;
	m_bFading			= false;
    m_bQuit             = false;
	
	OgreFramework::getSingletonPtr()->
		m_pLog->logMessage("Leaving GameCompleteState...");

	m_pSceneMgr->destroyCamera(m_pCamera);

	Animations::destroyAllAnimations();
	
	if(m_pCharAnim){
		delete m_pCharAnim;
		m_pCharAnim = 0;
	}
	if(m_pSceneMgr){
		OgreFramework::getSingletonPtr()->	 m_pRoot->destroySceneManager(m_pSceneMgr);
		OgreFramework::getSingletonPtr()->m_pActiveSceneMgr = 0;
	}

	//el fader se elimna junto con el resto de effectos
	//en destroyAllEffects()
	m_pFader = 0;

	//desapilar la GUI del menu de pausa, volviendo a la gui del juego	
	GUIManager::getSingletonPtr()->m_pGUIStateManager->popGUIState();
	
	Effect::destroyAllEffects();
}


bool GameCompleteState::keyPressed(const OIS::KeyEvent &keyEventRef)
{


	CEGUI::System::getSingleton().injectKeyDown(keyEventRef.key);

    OgreFramework::getSingletonPtr()->keyPressed(keyEventRef);

	//al pulsar esc o enter empezamos la transicion con fade out 
	//para pasar a los creditos
	if(keyEventRef.key == OIS::KC_ESCAPE || 
		keyEventRef.key == OIS::KC_RETURN){
		if(!m_bFading){
			AudioManager::getSingletonPtr()->fadeOutTrack(1.5);
			m_faderTimer = 0;
			m_bFading = true;
			Fader::getSingletonPtr()->startFadeOut(1.5);
		}
	
        //
        return true;

	}
   
    return true;
}



bool GameCompleteState::keyReleased(const OIS::KeyEvent &keyEventRef)
{
	CEGUI::System::getSingleton().injectKeyUp(keyEventRef.key);

    OgreFramework::getSingletonPtr()->keyReleased(keyEventRef);

    return true;
}



bool GameCompleteState::mouseMoved(const OIS::MouseEvent &evt)
{
	
	CEGUI::System::getSingleton().injectMouseMove(evt.state.X.rel, evt.state.Y.rel);  
	
    return true;
}



bool GameCompleteState::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{	
	if(id == OIS::MB_Left)
		CEGUI::System::getSingleton().injectMouseButtonDown(CEGUI::LeftButton);
	
    return true;
}


bool GameCompleteState::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
	if(id == OIS::MB_Left)
		CEGUI::System::getSingleton().injectMouseButtonUp(CEGUI::LeftButton);
  
    return true;
}


void GameCompleteState::update(double timeSinceLastFrame)
{
    m_FrameEvent.timeSinceLastFrame = timeSinceLastFrame;
  
	//se encarga de ejecutar y actualizar el efecto de fade in y fade out
	if(m_bFading){
		m_faderTimer += timeSinceLastFrame/1000;
		if(m_faderTimer >= 2)
			m_bQuit = true;
	}

    if(m_bQuit == true){
		OgreFramework::getSingletonPtr()->m_pAppStateManager->
			popAllAndPushAppState(OgreFramework::getSingletonPtr()->
			m_pAppStateManager->findByName("CreditsState"));
        return;
    }

	CEGUI::System::getSingleton().injectTimePulse( m_FrameEvent.timeSinceLastFrame);

	Effect::updateEach(timeSinceLastFrame);
	Animations::updateEach(timeSinceLastFrame);
	m_pCharAnim->update(timeSinceLastFrame);
}
