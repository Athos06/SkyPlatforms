//|||||||||||||||||||||||||||||||||||||||||||||||

#include "AdvancedOgreFramework.h"

#include "AppStateManager.h"

#include "MenuState.h"
#include "GameState.h"
#include "PauseState.h"
#include "GameOverState.h"
#include "GameCompleteState.h"
#include "LevelIntroState.h"
#include "SplashScreenState.h"
#include "CreditsState.h"
#include "CutsceneState.h"
#include "GUIManager.h"
#include "PhysicsManager.h"
#include "LevelsManager.h"
#include "AudioManager.h"
#include "Level1.h" 
#include "Level2.h"
#include "Level3.h"
#include "LevelBoss.h"
#include "SavegameLoader.h"

//|||||||||||||||||||||||||||||||||||||||||||||||

using namespace Ogre;

//|||||||||||||||||||||||||||||||||||||||||||||||

template<> OgreFramework* Ogre::Singleton<OgreFramework>::msSingleton = 0;

//|||||||||||||||||||||||||||||||||||||||||||||||

OgreFramework::OgreFramework()
{
    m_pRoot				= 0;
	m_pActiveSceneMgr	= 0;
    m_pRenderWnd		= 0;
    m_pViewport			= 0;
    m_pLog				= 0;
    m_pTimer			= 0;

    m_pInputMgr			= 0;
    m_pKeyboard			= 0;
    m_pMouse			= 0;
	
	m_pAppStateManager	= 0;
	m_pLevelsManager	= 0;

	m_pGUImgr			= 0;
	m_pPhysicsManager	= 0;
	
	m_pAudioMgr			= 0;


}

//|||||||||||||||||||||||||||||||||||||||||||||||

OgreFramework::~OgreFramework()
{
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Shutdown OGRE...");
	if(m_pPhysicsManager) delete m_pPhysicsManager;

	if(m_pInputMgr){
		OIS::InputManager::destroyInputSystem(m_pInputMgr);
		m_pInputMgr = 0;
	}
	if(m_pGUImgr) delete m_pGUImgr;
	if(m_pLevelsManager) delete m_pLevelsManager;
	if(m_pAudioMgr)	delete m_pAudioMgr;

	//Remove ourself as a Window listener
	Ogre::WindowEventUtilities::removeWindowEventListener(m_pRenderWnd, this);
	windowClosed(m_pRenderWnd);
	delete m_pRoot;
}


//evento de escalado de la ventana
void OgreFramework::windowResized(Ogre::RenderWindow* rw)
{
	//obtenemos las dimensiones de la ventan
	unsigned int width, height, depth;
    int left, top;
    rw->getMetrics(width, height, depth, left, top);

   //ajustamos la gui
   CEGUI::Size size;
   size.d_width = static_cast<float>(width);
   size.d_height = static_cast<float>(height);

   CEGUI::System::getSingleton().invalidateAllCachedRendering();
   CEGUI::System::getSingleton().notifyDisplaySizeChanged(size);
	
   //ajustamos la zona del raton
    const OIS::MouseState &ms = m_pMouse->getMouseState();
    ms.width = width;
    ms.height = height;


}
 
//Unattach OIS before window shutdown (very important under Linux)
void OgreFramework::windowClosed(Ogre::RenderWindow* rw)
{
    //Only close for window that created OIS
    if( rw == m_pRenderWnd )
    {
        if( m_pInputMgr )
        {
            m_pInputMgr->destroyInputObject(m_pMouse );
            m_pInputMgr->destroyInputObject(m_pKeyboard );
 
            OIS::InputManager::destroyInputSystem(m_pInputMgr);
            m_pInputMgr = 0;
        }
    }

}
 
bool OgreFramework::initOgre(Ogre::String wndTitle, OIS::KeyListener *pKeyListener, OIS::MouseListener *pMouseListener)
{
    Ogre::LogManager* logMgr = new Ogre::LogManager();

    m_pLog = Ogre::LogManager::getSingleton().createLog("OgreLogfile.log", true, true, false);
    m_pLog->setDebugOutputEnabled(true);

    m_pRoot = new Ogre::Root();

    if(!m_pRoot->showConfigDialog())
        return false;

	m_pRenderWnd = m_pRoot->initialise(true, wndTitle);

	//anado la ventana de la aplicacion al listener para los eventos de ventana
	Ogre::WindowEventUtilities::addWindowEventListener(m_pRenderWnd, this); 

    m_pViewport = m_pRenderWnd->addViewport(0);
    m_pViewport->setBackgroundColour(ColourValue(0.5f, 0.5f, 0.5f, 1.0f));

    m_pViewport->setCamera(0);


    size_t hWnd = 0;
    OIS::ParamList paramList;
    m_pRenderWnd->getCustomAttribute("WINDOW", &hWnd);

    paramList.insert(OIS::ParamList::value_type("WINDOW", Ogre::StringConverter::toString(hWnd)));

	m_pInputMgr = OIS::InputManager::createInputSystem(paramList);

    m_pKeyboard = static_cast<OIS::Keyboard*>(m_pInputMgr->createInputObject(OIS::OISKeyboard, true));
    m_pMouse = static_cast<OIS::Mouse*>(m_pInputMgr->createInputObject(OIS::OISMouse, true));

    m_pMouse->getMouseState().height = m_pRenderWnd->getHeight();
    m_pMouse->getMouseState().width	 = m_pRenderWnd->getWidth();


    if(pKeyListener == 0)
        m_pKeyboard->setEventCallback(this);
    else
        m_pKeyboard->setEventCallback(pKeyListener);

    if(pMouseListener == 0)
        m_pMouse->setEventCallback(this);
    else
        m_pMouse->setEventCallback(pMouseListener);

    Ogre::String secName, typeName, archName;
    Ogre::ConfigFile cf;
    cf.load("resources.cfg");

    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
    while (seci.hasMoreElements())
    {
        secName = seci.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i)
        {
            typeName = i->first;
            archName = i->second;
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
        }
    }
    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    m_pTimer = new Ogre::Timer();
    m_pTimer->reset();
	
	m_pGameConfig = new GameConfig();
	loadGameConfig();

	

	//manejador de la GUI
	m_pGUImgr = new GUIManager();
	m_pGUImgr->initGUI();
	
	//iniciar el audio, si se produce algun error al inciiar sdl
	//se cierra la aplicacion
	m_pAudioMgr = new AudioManager();
	if(!m_pAudioMgr->initAudio()){
		return false;
	}
		
	//crear los niveles
	initLevelsMgr();

	//crear el manejador de fisicas
	m_pPhysicsManager = new PhysicsManager();

    m_pRenderWnd->setActive(true);

    return true;
}
void OgreFramework::loadGameConfig(){
	SavegameLoader loader;
	//leemos el save game para ver que niveles estan desbloqueados
	loader.read();
}

//|||||||||||||||||||||||||||||||||||||||||||||||
void OgreFramework::initLevelsMgr(){
	m_pLevelsManager = new LevelsManager();

	Level1::create(m_pLevelsManager, "Level1");
	Level2::create(m_pLevelsManager, "Level2");
	Level3::create(m_pLevelsManager, "Level3");
	LevelBoss::create(m_pLevelsManager, "LevelBoss");
}

void OgreFramework::initAppStateMgr(){

	//Creacion e inicializacion de estados del juego
	m_pAppStateManager = new AppStateManager();

	MenuState::create(m_pAppStateManager, "MenuState");
	GameState::create(m_pAppStateManager, "GameState");
	PauseState::create(m_pAppStateManager, "PauseState");
	GameOverState::create(m_pAppStateManager, "GameOverState");
	LevelIntroState::create(m_pAppStateManager, "LevelIntroState");
	GameCompleteState::create(m_pAppStateManager, "GameCompleteState");
	SplashScreenState::create(m_pAppStateManager, "SplashScreenState");
	CutsceneState::create(m_pAppStateManager, "CutsceneState");
	CreditsState::create(m_pAppStateManager, "CreditsState");

	m_pAppStateManager->start( m_pAppStateManager->
		findByName("SplashScreenState"));//("CreditsState"));//("SplashScreenState") );

}
//|||||||||||||||||||||||||||||||||||||||||||||||

bool OgreFramework::keyPressed(const OIS::KeyEvent &keyEventRef)
{
    if(m_pKeyboard->isKeyDown(OIS::KC_SYSRQ))
    {
        m_pRenderWnd->writeContentsToTimestampedFile("Screenshot_", ".jpg");
        return true;
    }

    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool OgreFramework::keyReleased(const OIS::KeyEvent &keyEventRef)
{
    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool OgreFramework::mouseMoved(const OIS::MouseEvent &evt)
{
    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool OgreFramework::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool OgreFramework::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void OgreFramework::updateOgre(double timeSinceLastFrame)
{
}

//|||||||||||||||||||||||||||||||||||||||||||||||