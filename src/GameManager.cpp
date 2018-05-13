#include "GameManager.h"

#include "AdvancedOgreFramework.h"
#include "AppStateManager.h"
#include "LevelsManager.h"

#include "Euler.h"

#include "Player.h"
#include "CharacterController.h"
#include "CameraController.h"
#include "HUD.h"
#include "Portal.h"
#include "Animations.h"
#include "Fader.h"
#include "Effect.h"
#include "AudioManager.h"

#include "Constants.h"

template <> GameManager* Ogre::Singleton<GameManager>::msSingleton = 0;

GameManager& GameManager::getSingleton(){
	assert (msSingleton);
	return (*msSingleton);
}
GameManager* GameManager::getSingletonPtr(){
	assert (msSingleton);
	return msSingleton;
}
//|||||||||||||||||||||||||||||||||||||||||||||||


GameManager::GameManager(Ogre::SceneManager* sceneMgr){
	m_pSceneMgr			= sceneMgr;
	m_pPlayer			= 0;
	m_nLives			= 0;
	m_nSkystones		= 0;
	m_passLevel			= false;
	m_isEnding			= false;
	m_isWaiting			= false;
	m_waitForSeconds	= 0;
	m_waitingTime		= 0;
	m_faderActive		= false;
	m_gameComplete		= false;
}
//||||||||||||||||||||||||||||||||||||||||

GameManager::~GameManager(){
}
//||||||||||||||||||||||||||||||||||||||||

void GameManager::update(double timeSinceLastFrame){	
	//si estamos en una transicion y hemos indicado
	//que espere durante un determinado tiempo
	if(m_isWaiting){
		//actualizamos el timer y si hemos sobrepasado el tiempo
		//a esperar actuamos en consecuencia diciendo que se termino
		//la espera
		m_waitingTime += timeSinceLastFrame/1000;
		if(m_waitingTime >= m_waitForSeconds){
			m_waitingTime = 0;
			m_isWaiting = false;
			//si estamos en la transicion de fin del nivel
			//al terminar el tiempo de espera llamamos a 
			//gameOver donde se encarga de la logica
			//de terminar el nivel
			if(m_isEnding){
				gameOver();
			}
		}
	}
	
	getGameOverCondition();
	HUD::getSingletonPtr()->update(timeSinceLastFrame);
	m_pPlayer->update(timeSinceLastFrame);

}

void GameManager::startGame(){
	//al comenzar el nivel bloqueamos el movimiento del jugador
	//durante unos segundos mientras se ahce el fadein 
	m_pPlayer->blockPlayerForTime(2);
}

void GameManager::retryLevel(){
	m_passLevel = false;
	m_isEnding	= false;
	m_isWaiting	= false;
	m_faderActive = false;
	m_gameComplete = false;

	Animations::resetEach();
	//Effect::destroyAllEffects();
	LevelsManager::getSingletonPtr()->reset();
	m_pPlayer->reset();
	
	m_waitingTime = 0;
	m_waitForSeconds = 2;
	m_isWaiting	= true;
	//al comenzar el nivel bloqueamos el movimiento del jugador
	//durante unos segundos mientras se ahce el fadein 
	m_pPlayer->blockPlayerForTime(2);

	HUD::getSingletonPtr()->fadeIn();
	Fader::getSingletonPtr()->startFadeIn(2);
}

void GameManager::destroy(){
	if(m_pPlayer) delete m_pPlayer;
	m_pSceneMgr	= 0;
	m_pPlayer = 0;
	m_isWaiting	= false;
	m_waitingTime = 0;
	m_faderActive = false;
	m_isEnding	= false;
	m_gameComplete = false;

}

void GameManager::initGameManager(){
	m_passLevel = false;
	m_isEnding	= false;
	m_isWaiting	= false;
	m_faderActive = false;
	m_gameComplete = false;
	m_waitingTime = 0;
	m_waitForSeconds = 0;


	Ogre::Entity* enditadCapsula = m_pSceneMgr->createEntity("testP","lowPolyCharacter.mesh");
	Ogre::SceneNode* nodoCharacter = m_pSceneMgr->getRootSceneNode()->
		createChildSceneNode("nodeCharacter");

	nodoCharacter->attachObject(enditadCapsula);
	nodoCharacter->setPosition(m_pDefaultSpawnPoint->_getDerivedPosition());

	m_pPlayer = new Player();
	m_pPlayer->setupPlayer(nodoCharacter);

	CameraController* cameraMgr = new CameraController();

	cameraMgr->setupCamera(m_pSceneMgr->getCamera("Camera#0"), nodoCharacter);
	m_pPlayer->setupCamera(cameraMgr);

	updateLives();
	updateStones();

	Fader::getSingletonPtr()->startFadeIn(3);

	HUD::getSingletonPtr()->showHUD(false);

}
//|||||||||||||||||||||||||||||||||||||||||||||||

void GameManager::getGameOverCondition(){
	//se llama a checkWinningCondition del nivel actual, pues se
	//encarga de ejecutar una logica determinada en cada nivel
	//podemos llamarlo cada frame porque dentro del nivel
	// ya nos encargamos de solo actuar la primera vez
	LevelsManager::getSingletonPtr()->checkWinningCondition();
	//si el personaje ha muerto o hemos pasado el nivel pasamos a 
	//la transicion de fin del nivel dependiendo de un caso
	//u otro, y comprobamos que solo lo ejecutamos una vez
	if( (m_pPlayer->isDead() || m_passLevel) && !m_isEnding ){
		m_isWaiting = true;
		//si hemos muerto se espera 3 segundos de transicion
		m_waitForSeconds = 3;
		//si se entra en portal se espera solo 1 segundo pues la transicion 
		//es mas rapida para pasar a la cutscene
		if(m_passLevel){
			m_waitForSeconds = 1;
			m_pPlayer->blockPlayerForTime(1);
		}
		//indicamos que estamos terminando el nivel, independientemente
		//de si es porque hemos muerto o lo hemos superado
		m_isEnding = true;
		
		//si aun no habiamos activado el fadeOut lo hacemos ahora
		if(!m_faderActive){
			// de nuevo son diferentes si hemos superado el nivel o hemos muerto
			if(m_passLevel){
				AudioManager::getSingletonPtr()->fadeOutTrack(1);
				HUD::getSingletonPtr()->fadeOut();
				Fader::getSingletonPtr()->startFadeOut(1);
			}
			else{
				AudioManager::getSingletonPtr()->fadeOutTrack(2);
				HUD::getSingletonPtr()->fadeOut();
				Fader::getSingletonPtr()->startFadeOut(2);
				m_faderActive = true;
			}
		}
	}
}

void GameManager::completeLevel(){
	//LevelsManager::getSingletonPtr()->completeLevel();
	//ponemos la flag de que hemos pasado el nivel a true
	//lo que hara que pasemos a la transicion de fin de nivel
	m_passLevel = true;
	AudioManager::getSingletonPtr()->
		playFX("EnterPortal", 0, SECONDARYFX_CHANNEL);
}

void GameManager::gameOver(){
	if(m_passLevel){
		//si hemos superado el nivel llamamos a completar el nivel actual	
		LevelsManager::getSingletonPtr()->completeLevel();
		//ocultamos el HUD y pasamos a la cutscene de teletransporte
		//en el portal
		HUD::getSingletonPtr()->showHUD(false);
		OgreFramework::getSingletonPtr()->m_pAppStateManager->pushAppState
		( OgreFramework::getSingletonPtr()->m_pAppStateManager->
		findByName("CutsceneState") );
	}
	//si lo que ocurre es que hemos muerto pasamos al estado de gameOver
	else{
		OgreFramework::getSingletonPtr()->m_pAppStateManager->pushAppState
			( OgreFramework::getSingletonPtr()->m_pAppStateManager->
			findByName("GameOverState") );
	}
}

void GameManager::updateLives(){
	m_nLives = m_pPlayer->getLives();	
}

void GameManager::updateStones(){
	m_nSkystones = m_pPlayer->getSkystones();
	//LevelsManager::getSingletonPtr()->getLevelProperties()->increaseCounter();

}



//|||||||||||||||||||||||||||||||||||||||||||||||

template <> GameConfig* Ogre::Singleton<GameConfig>::msSingleton = 0;

GameConfig& GameConfig::getSingleton(){
	assert (msSingleton);
	return (*msSingleton);
}


GameConfig* GameConfig::getSingletonPtr(){
	assert (msSingleton);
	return msSingleton;
}

GameConfig::GameConfig(){
	volume = 100;
	sfxVolume = 100;

	xInverted = -1;
	yInverted = 1;

	mouseSensitivity = 2;

	levelsLock[0] = false;
	levelsLock[1] = false;
	levelsLock[2] = false;
	levelsLock[3] = false;
};


GameConfig::~GameConfig(){};
