//|||||||||||||||||||||||||||||||||||||||||||||||

#include "GameState.h"
#include "PhysicsManager.h"
#include "Constants.h"
#include "GUIManager.h"
#include "GUIStateManager.h"
#include "GameManager.h"
#include "CameraController.h"
#include "Player.h"
#include "Effect.h"
#include "LevelsManager.h"
#include "LoadingScreen.h"


GameState::GameState()
{
    m_bQuit             = false;
	m_pPhysicsMgr		= 0;
}


GameState::~GameState(){
}

void GameState::enter()
{

    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Entering GameState...");
	
	//mostramos la pantalla de loading cada vez que entramos en el gamestate,
	//despues se actualizara dentro del init() de cada Level
	GUIManager::getSingletonPtr()->m_pGUIStateManager->
		changeGUIState(GUIManager::getSingletonPtr()->m_pGUIStateManager
		->findByName("GUILoading") );
	//pantalla de carga
	GUIManager::getSingletonPtr()->m_pLoadingScreen->setCompletePercent(0);
	OgreFramework::getSingletonPtr()->m_pRoot->renderOneFrame();
	//inicializamos el nivel actual
	LevelsManager::getSingletonPtr()->init();
	//el gameManager es creado en cada nivl al inicializarlo, aqui solo
	//lo obtengo a parter del puntero singleton
	m_pGameMgr = GameManager::getSingletonPtr();
	m_pPhysicsMgr = PhysicsManager::getSingletonPtr();
	//DEBUGER
	//	mDebugDrawer = (BtOgre::DebugDrawer*)PhysicsManager::getSingletonPtr()->
	//	getDynamicsWorld()->getDebugDrawer();
	//DEBUGER	
}
//|||||||||||||||||||||||||||||||||||||||||||||||

void GameState::createScene(){
}
//|||||||||||||||||||||||||||||||||||||||||||||||

bool GameState::pause()
{
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Pausing GameState...");
    return true;
}
//|||||||||||||||||||||||||||||||||||||||||||||||

void GameState::resume()
{
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Resuming GameState...");

    m_bQuit = false;
}
//|||||||||||||||||||||||||||||||||||||||||||||||

void GameState::exit()
{

    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Leaving GameState...");
	
	//if(m_pPhysicsMgr)
		//m_pPhysicsMgr->destroyPhysics();		
	LevelsManager::getSingletonPtr()->exit();

	//el physics Manager lo destruyo en cada nivel
	m_pPhysicsMgr = 0;

	//el gameManager lo destruyo en cada nivel
	m_pGameMgr = 0;



}
//|||||||||||||||||||||||||||||||||||||||||||||||

bool GameState::keyPressed(const OIS::KeyEvent &keyEventRef)
{

	CEGUI::System::getSingleton().injectKeyDown(keyEventRef.key);
	
	if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_ESCAPE))
    {
        pushAppState(findByName("PauseState"));
        return true;
    }	
	//tecla de salto
	if(keyEventRef.key == OIS::KC_SPACE){
		m_pGameMgr->getPlayer()->jump();
	}
   
	/*
	if(keyEventRef.key == OIS::KC_C){
		mDebugDrawer->setDebugMode(false);
	}
	if(keyEventRef.key == OIS::KC_V){
		mDebugDrawer->setDebugMode(true);
	}
	*/
	//tecla para hacer trampas y pasar el nivel instantaneamente, para debug
	if(keyEventRef.key == OIS::KC_Z){
		LevelsManager::getSingletonPtr()->getLevelProperties()->increaseCounter();
		LevelsManager::getSingletonPtr()->getLevelProperties()->increaseCounter();
		LevelsManager::getSingletonPtr()->getLevelProperties()->increaseCounter();
		LevelsManager::getSingletonPtr()->getLevelProperties()->increaseCounter();
		
		GameManager::getSingletonPtr()->completeLevel();
		
	}
    return true;
}
//|||||||||||||||||||||||||||||||||||||||||||||||

bool GameState::keyReleased(const OIS::KeyEvent &keyEventRef)
{
	
    return true;
}
//|||||||||||||||||||||||||||||||||||||||||||||||

void GameState::getInput()
{
	float deltaTime = m_FrameEvent.timeSinceLastFrame/1000;
	//pasamos siempre el input de teclado que indica la direccion de movimiento
	//si no se presiona ninguna telca la direccion es nula
	Ogre::Vector3 movementDirection = Ogre::Vector3::ZERO;

	if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_A))
	{
		movementDirection = Ogre::Vector3::NEGATIVE_UNIT_X ;
	}
	
	if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_D))
	{
		movementDirection = Ogre::Vector3::UNIT_X;
	}
	
	if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_W))
	{
		movementDirection += Ogre::Vector3::NEGATIVE_UNIT_Z;
	}
	
	if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_S))
	{
		movementDirection += Ogre::Vector3::UNIT_Z ;
	}
	//siempre se le pasa la direccion de movimiento, es dentro
	//de Player donde ya se encarga de ver si tiene que utilizarla
	//o no, en caso de que este bloqueado por ejemplo
	m_pGameMgr->getPlayer()->setMovementVector(movementDirection);
	
}


bool GameState::mouseMoved(const OIS::MouseEvent &evt)
{
	float deltaTime = m_FrameEvent.timeSinceLastFrame/1000;

	CEGUI::System::getSingleton().injectMouseMove(evt.state.X.rel, evt.state.Y.rel);  
	//pasamos la rotacion de la camara al player
	
	//-1 si esta invertida la camara, sino 1
	int xInvert = GameConfig::getSingletonPtr()->xInverted;
	int yInvert = GameConfig::getSingletonPtr()->yInverted;
	
	float sensitivity = GameConfig::getSingletonPtr()->mouseSensitivity;
	sensitivity = sensitivity/20;

	if(m_pGameMgr){
		m_pGameMgr->getPlayer()->
			rotateCamera( xInvert *	evt.state.X.rel * sensitivity, 
			yInvert * evt.state.Y.rel * sensitivity, 
			-evt.state.Z.rel * 0.01 );
	}
    return true;
}


bool GameState::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
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

bool GameState::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
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

void GameState::update(double timeSinceLastFrame)
{
	//actualizamos las fisicas
	if(m_pPhysicsMgr)
		m_pPhysicsMgr->update(timeSinceLastFrame);

	m_FrameEvent.timeSinceLastFrame = timeSinceLastFrame;
	
	if(m_bQuit == true)
	{
		popAppState();
		return;
	}

	CEGUI::System::getSingleton().injectTimePulse( m_FrameEvent.timeSinceLastFrame);

	//DEBUGER
	//mDebugDrawer->setDebugMode(false);
	//mDebugDrawer->step();
	//DEBUGER

	//actualizamos todos los efectos
	Effect::updateEach(timeSinceLastFrame);
	//obtenemos el input de teclado para el Player
	getInput();
	//actualizamos el menu
	LevelsManager::getSingletonPtr()->update(timeSinceLastFrame);

	
    

}

