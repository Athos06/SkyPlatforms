#include "LevelBoss.h"
#include "LevelsManager.h"
#include "AdvancedOgreFramework.h"
#include "AppStateManager.h"
#include "DotSceneLoader.h"
#include "BtOgrePG.h"
#include "BtOgreGP.h"
#include "BtOgreExtras.h"
#include "GameManager.h"

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
#include "Boss.h"
#include "Portal.h"
#include "Constants.h"
#include "HUD.h"
#include "Fader.h"

LevelBoss::LevelBoss(){
	m_pCamera = 0;
	m_pSceneMgr = 0;	
	m_pPhysicsMgr = 0;
	m_pBossMgr = 0;
	m_pFader = 0;
	m_bLevelClear = false;
	m_sceneTimer = 0;
	m_bKingSceneActive = false;
	m_bKingSceneEnded = false;
	m_pLevelProperties = new LevelBossProperties();
}

LevelBoss::~LevelBoss(){
}

void LevelBoss::reset(){
	//cuando morimos y seleccionamos continuar al resetear el nivel
	//dependiendo de si el jefe final esta muerto o reproducimos un track
	//u otro
	if(m_pLevelProperties->checkWinningCondition())
		AudioManager::getSingletonPtr()->playMainTrack("KingDead.ogg");
	else
		AudioManager::getSingletonPtr()->playMainTrack("LevelBoss.ogg");
	m_pBossMgr->resetBoss();
}

void LevelBoss::init(){

	m_sceneTimer = 0;
	m_bKingSceneActive = false;
	m_bKingSceneEnded = false;
	m_bLevelClear = false;
	
	OgreFramework::getSingletonPtr()->m_pLog->logMessage("Creando LevelBoss...");

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
	
	AudioManager::getSingletonPtr()->preLoadTrack("KingDead.ogg");
	//pantalla de carga
	GUIManager::getSingletonPtr()->m_pLoadingScreen->setCompletePercent(70);
	OgreFramework::getSingletonPtr()->m_pRoot->renderOneFrame();

	AudioManager::getSingletonPtr()->playMainTrack("IntroBoss.ogg");
	
	
	//pantalla de carga
	GUIManager::getSingletonPtr()->m_pLoadingScreen->setCompletePercent(90);
	OgreFramework::getSingletonPtr()->m_pRoot->renderOneFrame();
	
	m_pGameMgr->initGameManager();
	//inicializa el boss
	m_pBossMgr->initBoss();
	

	//pantalla de carga
	GUIManager::getSingletonPtr()->m_pLoadingScreen->setCompletePercent(100);
	OgreFramework::getSingletonPtr()->m_pRoot->renderOneFrame();
	
	GUIManager::getSingletonPtr()->m_pGUIStateManager->popAllAndPushGUIState
		(GUIManager::getSingletonPtr()->m_pGUIStateManager->findByName("GUIGame") );

	//mostramos el HUD especifico del nivel del boss
	HUD::getSingletonPtr()->selectIntro(INTRO_LEVELBOSS);
	HUD::getSingletonPtr()->showBossHUD();

	//pasamos 
	OgreFramework::getSingletonPtr()->m_pAppStateManager->
		pushAppState(OgreFramework::getSingletonPtr()->
		m_pAppStateManager->findByName("LevelIntroState") );

}


void LevelBoss::start(){
	AudioManager::getSingletonPtr()->playMainTrack("LevelBoss.ogg");
	HUD::getSingletonPtr()->fadeIn();
	Fader::getSingletonPtr()->startFadeIn(2);
	m_pBossMgr->setBossActive(true);
	m_pBossMgr->blockBossForTime(2);
	m_pGameMgr->startGame();
}


void LevelBoss::exit(){
	m_sceneTimer = 0;
	m_bKingSceneActive = false;
	m_bKingSceneEnded = false;
	m_bLevelClear = false;

	AudioManager::getSingletonPtr()->unloadMainTrack();
	
	m_pLevelProperties->reset();

	if(m_bLevelClear){
		LevelsManager::getSingletonPtr()->selectLevel(
			LevelsManager::getSingletonPtr()->findByName("Level1"));
	}


	if(m_pGameMgr){
		m_pGameMgr->destroy();
		delete m_pGameMgr;
		m_pGameMgr = 0;
	}
	
	if(m_pBossMgr){
		m_pBossMgr->destroy();
		delete m_pBossMgr;
	}
	
	//el fader se elimna junto con el resto de effectos
	//en destroyAllEffects()
	m_pFader = 0;

	if(m_pPhysicsMgr)
		m_pPhysicsMgr->destroyPhysics();	
	
	m_pLevelProperties->reset();
	
	m_pPhysicsMgr = 0;
	HUD::getSingletonPtr()->reset();
	//eliminar todas las animaciones
	Animations::destroyAllAnimations();
	//destruir las trampas
	Trap::destroyAllTraps();
	//destruir las plataformas
	MovingPlatform::destroyAllPlatforms();
	Effect::destroyAllEffects();
	ParticlesEffect::destroyAllParticles();
	//eliminar el scene manager lo ultimo porque si no elimnia todos los nodos
	//y da error al intentar eliminar los nodos en los destructores de los otros objetos
	m_pSceneMgr->destroyCamera(m_pCamera);
	if(m_pSceneMgr){
        OgreFramework::getSingletonPtr()->m_pRoot->destroySceneManager(m_pSceneMgr);
		OgreFramework::getSingletonPtr()->m_pActiveSceneMgr = 0;
	}
	

}

void LevelBoss::createScene(){	
	//para poder crear el Boss desde el importador
	m_pSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

	m_pBossMgr = new BossManager();

	DotSceneLoader loader;
	loader.parseDotScene("LevelBoss.scene", "General", m_pSceneMgr, m_pPhysicsMgr);
	
	m_pCamera = m_pSceneMgr->getCamera("Camera#0");
	m_pCamera->setAspectRatio(Ogre::Real(OgreFramework::getSingletonPtr()->m_pViewport->getActualWidth()) /
        Ogre::Real(OgreFramework::getSingletonPtr()->m_pViewport->getActualHeight()));	
	OgreFramework::getSingletonPtr()->m_pViewport->setCamera(m_pCamera);

	//en este nivel el portal esta activo desde el principio
	m_pGameMgr->m_pPortal->setActive(true);

	//precarga de particulas
	new ParticlesEffect(0, "SmallExplosionSmoke", Ogre::Vector3::ZERO);
	new ParticlesEffect(0, "SmallExplosionChunks", Ogre::Vector3::ZERO);
	new ParticlesEffect(0, "trapSpiral", Ogre::Vector3::ZERO);
	new ParticlesEffect(0, "ExplosionSmoke", Ogre::Vector3::ZERO);
	new ParticlesEffect(0, "ExplosionChunks", Ogre::Vector3::ZERO);
}

void LevelBoss::update(double deltaTime){
	
	//if(m_pGameMgr)
	m_pGameMgr->update(deltaTime);

	Animations::updateEach(deltaTime);
	Trap::updateEach(deltaTime);
	MovingPlatform::updateEach(deltaTime);
	ParticlesEffect::updateEach(deltaTime);
	m_pBossMgr->update(deltaTime);

	//si hemos activado los 3 interruptores ejecutamos
	//la logica de la escena de la explosion del boss
	if(m_pLevelProperties->checkWinningCondition()){
		m_bKingSceneActive = true;	
	}
	if(m_bKingSceneActive && !m_bKingSceneEnded){
		m_sceneTimer += deltaTime/1000;		
		if(m_sceneTimer >= 7){
			m_bKingSceneActive = false;
			m_bKingSceneEnded = true;
			//cuando el boss ha muerto cambiamos la musica
			AudioManager::getSingletonPtr()->fadeInTrack("KingDead.ogg", 2);
			OgreFramework::getSingletonPtr()->m_pViewport->
				setCamera(m_pCamera);
		}
	}
}



void LevelBoss::passLevel(){
	m_bLevelClear = true;
	m_pGameMgr->setGameComplete(true);
	//en el ultimo nivel no hace falta seleccionar el siguiente nivel
	//porque pasamos a la escena de fin del juego y luego se vuelve al
	//menu, desde donde ya seleccionamos el nivel
	//LevelsManager::getSingletonPtr()->selectLevel(
	//LevelsManager::getSingletonPtr()->findByName("Level1"));
}

///////////////////////////////////////////////////////

LevelBossProperties::LevelBossProperties(){
	m_bWin = false;
	m_counterCondition = 0;
	m_conditionValue = 3;
}

bool LevelBossProperties::checkWinningCondition(){
	//este nivel es diferente y en lugar de activar el portal
	//cuando hemos cumplido la condicion de victoria lo que hacemos
	//es iniciar la escena de destruccion del boss
	if(m_counterCondition >= m_conditionValue){
		if(!m_bWin){
			m_bWin = true;
			//paramos el track principal
			AudioManager::getSingletonPtr()->fadeOutTrack(2);
			AudioManager::getSingletonPtr()->playFX("PortalActivated", 
				0, SECONDARYFX_CHANNEL);
			BossManager::getSingletonPtr()->killBoss();
			//cambiamos la camara a la de la escena final del boss
			Ogre::Camera* cam = OgreFramework::getSingletonPtr()->m_pActiveSceneMgr
				->getCamera("KingEndCamera");

			OgreFramework::getSingletonPtr()->m_pViewport->setCamera(cam);
			cam->setAspectRatio(Ogre::Real(OgreFramework::getSingletonPtr()->
				m_pViewport->getActualWidth()) /Ogre::Real(OgreFramework::
				getSingletonPtr()->m_pViewport->getActualHeight()));	
			GameManager::getSingletonPtr()->getPlayer()->blockPlayerForTime(7);
		}
		return true;
	}
	return false;
}

void LevelBossProperties::reset(){
	m_bWin = false;
	m_counterCondition = 0;
	m_conditionValue = 3;
	
}
