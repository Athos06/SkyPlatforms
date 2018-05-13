//|||||||||||||||||||||||||||||||||||||||||||||||

#include "CutsceneState.h"
#include "GUIManager.h"
#include "AppStateManager.h"
#include "GameManager.h"
#include "GUIStateManager.h"
#include "AudioManager.h"
#include "Player.h"
#include "Animations.h"
#include "Trap.h"
#include "MovingPlatform.h"
#include "Effect.h"
#include "Fader.h"
#include "ParticlesEffect.h"
#include "Constants.h"

CutsceneState::CutsceneState()
{
    m_bQuit             = false;
    m_bTransition		= false;
    m_FrameEvent        = Ogre::FrameEvent();
	m_pCutsceneCam		= 0;
	m_pCameraNode		= 0;
	m_timeLenght		= 0;
}


void CutsceneState::enter()
{
	m_timeLenght = 0;
	m_bTransition = false;

	AudioManager::getSingletonPtr()->unloadMainTrack();
	AudioManager::getSingletonPtr()->playMainTrack("WINteleport.ogg", 0);
	OgreFramework::getSingletonPtr()->m_pLog->logMessage("Entering CutsceneState...");

	 m_pSceneMgr = OgreFramework::getSingletonPtr()->
		 m_pRoot->getSceneManager("gameSceneMgr");
			
    m_bQuit = false;

    createScene();
}


void CutsceneState::createScene()
{
	
	Fader::getSingletonPtr()->startFadeIn(1);
	
	//colocamos al Player sobre el portal
	Player* player = GameManager::getSingletonPtr()->getPlayer();
	
	Ogre::SceneNode* portal = m_pSceneMgr->getSceneNode("portal");
	Ogre::Vector3 pos = portal->_getDerivedPosition();
	pos.y += 1.0;
	player->teleport(pos);
	player->m_pNode->setOrientation(Ogre::Quaternion::IDENTITY);

	pos = portal->_getDerivedPosition();
	
	//creamos y colcamos la camara de la cutscene en su posicion
	//correspondiente
	m_pCutsceneCam = m_pSceneMgr->createCamera("cutsceneCamera");
	m_pCutsceneCam->setPosition(pos.x, pos.y + 2, pos.z + 4);
	m_pCutsceneCam->setFOVy(Ogre::Degree(60));
	m_pCutsceneCam->setOrientation(Ogre::Quaternion::IDENTITY);
	pos.y += 1;
	m_pCutsceneCam->lookAt(pos);
	OgreFramework::getSingletonPtr()->m_pViewport->setCamera(m_pCutsceneCam);
	m_pCutsceneCam->setAspectRatio(Ogre::Real(OgreFramework::getSingletonPtr()->m_pViewport->getActualWidth()) /
        Ogre::Real(OgreFramework::getSingletonPtr()->m_pViewport->getActualHeight()));

	m_pCutsceneCam->setNearClipDistance(0.1);
	m_pCutsceneCam->setFarClipDistance (100);


}


void CutsceneState::exit()
{
	m_bQuit = false;
	m_bTransition = false;
	m_timeLenght = 0;

	AudioManager::getSingletonPtr()->unloadMainTrack();
	ParticlesEffect::destroyAllParticles();	
}


bool CutsceneState::keyPressed(const OIS::KeyEvent &keyEventRef)
{

	CEGUI::System::getSingleton().injectKeyDown(keyEventRef.key);

    OgreFramework::getSingletonPtr()->keyPressed(keyEventRef);

    return true;
}

bool CutsceneState::keyReleased(const OIS::KeyEvent &keyEventRef)
{
	CEGUI::System::getSingleton().injectKeyUp(keyEventRef.key);

    OgreFramework::getSingletonPtr()->keyReleased(keyEventRef);

    return true;
}


bool CutsceneState::mouseMoved(const OIS::MouseEvent &evt)
{
	
	CEGUI::System::getSingleton().injectMouseMove(evt.state.X.rel, evt.state.Y.rel);  
	
    return true;
}


bool CutsceneState::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{	
	if(id == OIS::MB_Left)
		CEGUI::System::getSingleton().injectMouseButtonDown(CEGUI::LeftButton);
	
    return true;
}


bool CutsceneState::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
	if(id == OIS::MB_Left)
		CEGUI::System::getSingleton().injectMouseButtonUp(CEGUI::LeftButton);
  
    return true;
}

void CutsceneState::update(double timeSinceLastFrame)
{
    m_FrameEvent.timeSinceLastFrame = timeSinceLastFrame;
	m_timeLenght += timeSinceLastFrame/1000;

	Player* player = GameManager::getSingletonPtr()->getPlayer();

	//todo el control de la cutscene esta hardoceado
	if(	m_timeLenght >= 3 && !m_bTransition){

		Fader::getSingletonPtr()->startFadeOut(3);
		m_bTransition = true;
		//a los 3 segundos de la cutscene ejecutamos el efecto de particulas
		new ParticlesEffect(4.0f, "teleport", 
			Ogre::Vector3(m_pSceneMgr->getSceneNode("portal")->_getDerivedPosition()) );

		AudioManager::getSingletonPtr()->playFX("Teleporting", 0, SECONDARYFX_CHANNEL);
		
	}
	
	//a partir de los 3 segundos empezamos el efecto de escalar al personaje
	//para hacer como si estuviese introduciendose en el portal y siendo
	//teletransportado
	if(m_timeLenght >= 3){
		player = GameManager::getSingletonPtr()->getPlayer();
		player->m_pNode->scale(1 - 2.5*timeSinceLastFrame/1000,
								1 - 2.5*timeSinceLastFrame/1000,
								1 - 2.5*timeSinceLastFrame/1000);
		
		Ogre::Vector3 pos = player->m_pNode->getPosition();
		pos.y -= 1.2*timeSinceLastFrame/1000;
		player->teleport(pos);
	}
	//por ultimo ocultamos al personaje
	if(m_timeLenght >= 6){
		player->m_pNode->setVisible(false);
	}
	// y terminamos la cutscene pasando al menu de continuar
	if(	m_timeLenght >= 8.5){
		OgreFramework::getSingletonPtr()->m_pAppStateManager->pushAppState
			( OgreFramework::getSingletonPtr()->m_pAppStateManager->
			findByName("GameOverState") );
	}

    if(m_bQuit == true)
    {
        popAppState();
        return;
    }

	CEGUI::System::getSingleton().injectTimePulse
		( m_FrameEvent.timeSinceLastFrame);


	GameManager::getSingletonPtr()->getPlayer()->update(timeSinceLastFrame);
	Animations::updateEach(timeSinceLastFrame);
	Trap::updateEach(timeSinceLastFrame);
	MovingPlatform::updateEach(timeSinceLastFrame);
	Effect::updateEach(timeSinceLastFrame);
	ParticlesEffect::updateEach(timeSinceLastFrame);
}