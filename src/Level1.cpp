#include "Level1.h"
#include "LevelsManager.h"
#include "AdvancedOgreFramework.h"
#include "AppStateManager.h"
#include "DotSceneLoader.h"
#include "BtOgrePG.h"
#include "BtOgreGP.h"
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


Level1::Level1(){
	m_pCamera = 0;
	m_pSceneMgr = 0;	
	m_pPhysicsMgr = 0;
	m_pFader = 0;
	m_bm_bLevelClear = false;

	m_pLevelProperties = new Level1Properties();
}

Level1::~Level1(){
}

void Level1::reset(){
	//cuando morimos y seleccionamos continuar al resetear el nivel
	//dependiendo de si el portal esta abierto reproducimos un track u otro
	//u otro
	if(m_pLevelProperties->checkWinningCondition())
		AudioManager::getSingletonPtr()->playMainTrack("Level1return.ogg");
	else
		AudioManager::getSingletonPtr()->playMainTrack("Level1.ogg");


}

void Level1::init(){

	m_pLevelProperties->reset();

	m_bm_bLevelClear = false;


	OgreFramework::getSingletonPtr()->m_pLog->logMessage("Creando Level1...");
	//precargar de tracks
	AudioManager::getSingletonPtr()->preLoadTrack("Level1.ogg");
	AudioManager::getSingletonPtr()->preLoadTrack("Level1return.ogg");
		GUIManager::getSingletonPtr()->m_pLoadingScreen->setCompletePercent(25);
	OgreFramework::getSingletonPtr()->m_pRoot->renderOneFrame();
	//reproducir track de intro
	AudioManager::getSingletonPtr()->playMainTrack("IntroLevel.ogg");
	//pantalla de carga
	GUIManager::getSingletonPtr()->m_pLoadingScreen->setCompletePercent(50);
	OgreFramework::getSingletonPtr()->m_pRoot->renderOneFrame();

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

	createScene();
	//pantalla de carga
	GUIManager::getSingletonPtr()->m_pLoadingScreen->setCompletePercent(70);
	OgreFramework::getSingletonPtr()->m_pRoot->renderOneFrame();
	
	m_pGameMgr->initGameManager();
	
	//pantalla de carga
	GUIManager::getSingletonPtr()->m_pLoadingScreen->setCompletePercent(100);
	OgreFramework::getSingletonPtr()->m_pRoot->renderOneFrame();

	GUIManager::getSingletonPtr()->m_pGUIStateManager->popAllAndPushGUIState
		(GUIManager::getSingletonPtr()->m_pGUIStateManager->findByName("GUIGame") );

	//seleccionamos la interfaz de la intro del nivel1
	HUD::getSingletonPtr()->selectIntro(INTRO_LEVEL1);

	OgreFramework::getSingletonPtr()->m_pAppStateManager->
		pushAppState(OgreFramework::getSingletonPtr()->
		m_pAppStateManager->findByName("LevelIntroState") );
	

}


void Level1::start(){
	AudioManager::getSingletonPtr()->playMainTrack("Level1.ogg");
	HUD::getSingletonPtr()->fadeIn();
	Fader::getSingletonPtr()->startFadeIn(2);
	m_pGameMgr->startGame();
}

void Level1::exit(){
	
	AudioManager::getSingletonPtr()->unloadMainTrack();
	//si hemos pasado el nivel seleccionamos el siguiente nivel
	//para cuando seleccionamos continuar al terminar el nivel
	if(m_bm_bLevelClear){
		LevelsManager::getSingletonPtr()->selectLevel(
			LevelsManager::getSingletonPtr()->findByName("Level2"));
	}

	//destruir y resetear todo lo usado en el nivel
	if(m_pGameMgr){
		m_pGameMgr->destroy();
		delete m_pGameMgr;
		m_pGameMgr = 0;
	}
	//el fader se elimna junto con el resto de effectos
	//en destroyAllEffects()
	m_pFader = 0;
	if(m_pPhysicsMgr)
		m_pPhysicsMgr->destroyPhysics();	
	m_pPhysicsMgr = 0;
	m_pLevelProperties->reset();
	HUD::getSingletonPtr()->reset();
	Animations::destroyAllAnimations();
	Trap::destroyAllTraps();
	MovingPlatform::destroyAllPlatforms();
	Effect::destroyAllEffects();
	ParticlesEffect::destroyAllParticles();
	Collectible::destroyAll();

	//eliminar el scene manager lo ultimo porque si no elimina todos los nodos
	//y da error al intentar eliminar los nodos en los destructores de los otros objetos
	m_pSceneMgr->destroyCamera(m_pCamera);
	if(m_pSceneMgr){
        OgreFramework::getSingletonPtr()->m_pRoot->destroySceneManager(m_pSceneMgr);
		OgreFramework::getSingletonPtr()->m_pActiveSceneMgr = 0;
	}
	

}

void Level1::createScene(){	
	//hay que seleccionar el modo de las sombras antes de cargar la escena
	//porque utilizo logica estatica que de otro modo no tendria sombras
	m_pSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

	DotSceneLoader loader;
	loader.parseDotScene("Level1.scene", "General", m_pSceneMgr, m_pPhysicsMgr);
	//la camara se la pasamos en el .scene, aqui la recuperamos por su nombre
	//Obviamente es indispensable nombrarla con ese nombre especifico en el .scene
	m_pCamera = m_pSceneMgr->getCamera("Camera#0");
	m_pCamera->setAspectRatio(Ogre::Real(OgreFramework::getSingletonPtr()->m_pViewport->getActualWidth()) /
        Ogre::Real(OgreFramework::getSingletonPtr()->m_pViewport->getActualHeight()));	
	OgreFramework::getSingletonPtr()->m_pViewport->setCamera(m_pCamera);
	//precarga de efecto de particulas
	new ParticlesEffect(0, "PickUpStone", Ogre::Vector3::ZERO);
	
}

void Level1::update(double deltaTime){
	
	//if(m_pGameMgr)
	m_pGameMgr->update(deltaTime);

	Animations::updateEach(deltaTime);
	Trap::updateEach(deltaTime);
	MovingPlatform::updateEach(deltaTime);
	ParticlesEffect::updateEach(deltaTime);
}


//se llama al superar el nivel
void Level1::passLevel(){
	m_bm_bLevelClear = true;
	//se desbloquea el siguiente nivel y se guarda
	//como desbloqueado en el savegame
	GameConfig::getSingletonPtr()->levelsLock[1] = true;
	SavegameLoader loader;
	loader.write();
}

///////////////////////////////////////////////////////

Level1Properties::Level1Properties(){
	m_bWin = false;
	m_counterCondition = 0;
	m_conditionValue = 4;
}

bool Level1Properties::checkWinningCondition(){
if(m_counterCondition >= m_conditionValue){
	m_bWin = true;
	//si el portal no esta activado lo activamos
	//si esta activado no hacemos nada
	if(!(GameManager::getSingletonPtr()
	->m_pPortal->isActive()) ){
			GameManager::getSingletonPtr()
			->m_pPortal->setActive(true);
			AudioManager::getSingletonPtr()->playFX("PortalActivated", 
			0, SECONDARYFX_CHANNEL);
			AudioManager::getSingletonPtr()->fadeInTrack("Level1return.ogg", 1);
			HUD::getSingletonPtr()->showOpenMsg(true);
			//aumento la velocidad de todas las animaciones un 10%
			Animations::ChangeSpeedAll(0.15);
		}		
		return true;
	}
	return false;
}

void Level1Properties::reset(){
	m_bWin = false;
	m_counterCondition = 0;
	m_conditionValue = 4;
}
