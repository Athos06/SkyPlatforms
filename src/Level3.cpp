#include "Level3.h"
#include "LevelsManager.h"
#include "AdvancedOgreFramework.h"
#include "AppStateManager.h"
#include "DotSceneLoader.h"
#include "GameManager.h"
#include "SavegameLoader.h"
#include "CameraController.h"
#include "Collectible.h"
#include "Player.h"
#include "Animations.h"
#include "Trap.h"
#include "MovingPlatform.h"
#include "Effect.h"
#include "AudioManager.h"
#include "ParticlesEffect.h"
#include "GUIManager.h"
#include "GUIStateManager.h"
#include "LoadingScreen.h"
#include "Fader.h"
#include "Portal.h"
#include "HUD.h"
#include "Constants.h"

Level3::Level3(){
	m_pCamera = 0;
	m_pSceneMgr = 0;	
	m_pPhysicsMgr = 0;
	m_pFader	= 0;
	m_bLevelClear = false;
	m_pLevelProperties = new Level3Properties();
}

Level3::~Level3(){
}

void Level3::reset(){
	//cuando morimos y seleccionamos continuar al resetear el nivel
	//dependiendo de si el portal esta abierto reproducimos un track u otro
	//u otro
	if(m_pLevelProperties->checkWinningCondition())
		AudioManager::getSingletonPtr()->playMainTrack("Level3return.ogg");
	else
		AudioManager::getSingletonPtr()->playMainTrack("Level3.ogg");

}

void Level3::init(){

	m_pLevelProperties->reset();

	m_bLevelClear = false;

	OgreFramework::getSingletonPtr()->m_pLog->logMessage("Creando Level3...");

	m_pSceneMgr = OgreFramework::getSingletonPtr()->
	m_pRoot->createSceneManager(Ogre::ST_GENERIC, "gameSceneMgr");
	OgreFramework::getSingletonPtr()->m_pActiveSceneMgr = m_pSceneMgr;

	m_pGameMgr =  new GameManager(m_pSceneMgr);

	m_pFader = new Fader("Overlays/FadeInOut", "Materials/OverlayMaterial");

	//iniciar la simulacion de fisicas
	m_pPhysicsMgr = PhysicsManager::getSingletonPtr();
	m_pPhysicsMgr->initPhysics(m_pSceneMgr);
	/*
	BtOgre::DebugDrawer* mDebugDrawer = new BtOgre::DebugDrawer(m_pSceneMgr->getRootSceneNode(), 
		PhysicsManager::getSingletonPtr()->getDynamicsWorld());
	PhysicsManager::getSingletonPtr()->getDynamicsWorld()->setDebugDrawer(mDebugDrawer); 
	*/
	//pantalla de carga
	GUIManager::getSingletonPtr()->m_pLoadingScreen->setCompletePercent(25);
	OgreFramework::getSingletonPtr()->m_pRoot->renderOneFrame();

    createScene();
	//pantalla de carga
	GUIManager::getSingletonPtr()->m_pLoadingScreen->setCompletePercent(50);
	OgreFramework::getSingletonPtr()->m_pRoot->renderOneFrame();
	
	AudioManager::getSingletonPtr()->preLoadTrack("Level3.ogg");
	AudioManager::getSingletonPtr()->preLoadTrack("Level3return.ogg");
	//pantalla de carga
	GUIManager::getSingletonPtr()->m_pLoadingScreen->setCompletePercent(70);
	OgreFramework::getSingletonPtr()->m_pRoot->renderOneFrame();

	AudioManager::getSingletonPtr()->playMainTrack("IntroLevel.ogg");	
	
	//pantalla de carga
	GUIManager::getSingletonPtr()->m_pLoadingScreen->setCompletePercent(90);
	OgreFramework::getSingletonPtr()->m_pRoot->renderOneFrame();
	
	m_pGameMgr->initGameManager();
	
	//pantalla de carga
	GUIManager::getSingletonPtr()->m_pLoadingScreen->setCompletePercent(100);
	OgreFramework::getSingletonPtr()->m_pRoot->renderOneFrame();

	GUIManager::getSingletonPtr()->m_pGUIStateManager->popAllAndPushGUIState
		(GUIManager::getSingletonPtr()->m_pGUIStateManager->findByName("GUIGame") );

	//seleccionamos la interfaz de la intro del nivel3
	HUD::getSingletonPtr()->selectIntro(INTRO_LEVEL3);

	OgreFramework::getSingletonPtr()->m_pAppStateManager->
		pushAppState(OgreFramework::getSingletonPtr()->
		m_pAppStateManager->findByName("LevelIntroState") );

}

void Level3::start(){
	AudioManager::getSingletonPtr()->playMainTrack("Level3.ogg");
	HUD::getSingletonPtr()->fadeIn();
	Fader::getSingletonPtr()->startFadeIn(2);
	m_pGameMgr->startGame();
}


void Level3::exit(){
	
	AudioManager::getSingletonPtr()->unloadMainTrack();

	if(m_bLevelClear){
		LevelsManager::getSingletonPtr()->selectLevel(
			LevelsManager::getSingletonPtr()->findByName("LevelBoss"));
	}

	if(m_pGameMgr){
		m_pGameMgr->destroy();
		delete m_pGameMgr;
		m_pGameMgr = 0;
	}
	
	//el fader se elimna junto con el resto de effectos
	//en destroyAllEffects()
	m_pFader = 0;

	if(m_pPhysicsMgr){
		m_pPhysicsMgr->destroyPhysics();	
		m_pPhysicsMgr = 0;	
	}
	m_pLevelProperties->reset();
	
	HUD::getSingletonPtr()->reset();
	//eliminar todas las animaciones
	Animations::destroyAllAnimations();
	//destruir las trampas
	Trap::destroyAllTraps();
	//destruir las plataformas
	MovingPlatform::destroyAllPlatforms();
	//destruir todos los efectos
	Effect::destroyAllEffects();
	//destruir todas los particulas
	ParticlesEffect::destroyAllParticles();
	Collectible::destroyAll();
	//eliminar el scene manager lo ultimo porque si no elimnia todos los nodos
	//y da error al intentar eliminar los nodos en los destructores de los otros objetos
	m_pSceneMgr->destroyCamera(m_pCamera);
	if(m_pSceneMgr){
        OgreFramework::getSingletonPtr()->m_pRoot->destroySceneManager(m_pSceneMgr);
		OgreFramework::getSingletonPtr()->m_pActiveSceneMgr = 0;
	}
	

}

void Level3::createScene(){	
	//hay que seleccionar el modo de las sombras antes de cargar la escena
	//porque utilizo logica estatica que de otro modo no tendria sombras
	m_pSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

	DotSceneLoader loader;
	loader.parseDotScene("Level3.scene", "General", m_pSceneMgr, m_pPhysicsMgr);
	//la camara se la pasamos en el .scene, aqui la recuperamos por su nombre
	//Obviamente es indispensable nombrarla con ese nombre especifico en el .scene
	m_pCamera = m_pSceneMgr->getCamera("Camera#0");
	m_pCamera->setAspectRatio(Ogre::Real(OgreFramework::getSingletonPtr()->m_pViewport->getActualWidth()) /
        Ogre::Real(OgreFramework::getSingletonPtr()->m_pViewport->getActualHeight()));	
	OgreFramework::getSingletonPtr()->m_pViewport->setCamera(m_pCamera);

	//precarga de efecto de particulas
	new ParticlesEffect(0, "PickUpStone", Ogre::Vector3::ZERO);

}

void Level3::update(double deltaTime){
	
	m_pGameMgr->update(deltaTime);

	Animations::updateEach(deltaTime);
	Trap::updateEach(deltaTime);
	MovingPlatform::updateEach(deltaTime);
	ParticlesEffect::updateEach(deltaTime);
}



void Level3::passLevel(){
	m_bLevelClear = true;
	GameConfig::getSingletonPtr()->levelsLock[3] = true;
	SavegameLoader loader;
	loader.write();
	/*LevelsManager::getSingletonPtr()->selectLevel(
	LevelsManager::getSingletonPtr()->findByName("LevelBoss"));*/

}

///////////////////////////////////////////////////////

Level3Properties::Level3Properties(){
	m_bWin = false;
	m_counterCondition = 0;
	m_conditionValue = 4;
}

bool Level3Properties::checkWinningCondition(){
	if(m_counterCondition >= m_conditionValue){
		m_bWin = true;
		if(!(GameManager::getSingletonPtr()
		->m_pPortal->isActive()) ){
			GameManager::getSingletonPtr()
			->m_pPortal->setActive(true);
			AudioManager::getSingletonPtr()->playFX("PortalActivated", 
			0, SECONDARYFX_CHANNEL);
			AudioManager::getSingletonPtr()->playMainTrack("Level3return.ogg");
			HUD::getSingletonPtr()->showOpenMsg(true);
			//aumento la velocidad de todas las animaciones un 15%
			Animations::ChangeSpeedAll(0.15);
		}		
		return true;
	}
	return false;
}

void Level3Properties::reset(){
	m_bWin = false;
	m_counterCondition = 0;
	m_conditionValue = 4;
}
