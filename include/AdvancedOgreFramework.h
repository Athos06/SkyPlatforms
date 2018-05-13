//|||||||||||||||||||||||||||||||||||||||||||||||

#ifndef OGRE_FRAMEWORK_h
#define OGRE_FRAMEWORK_h

//|||||||||||||||||||||||||||||||||||||||||||||||

#include <OgreCamera.h>
#include <OgreEntity.h>
#include <OgreLogManager.h>
#include <OgreOverlay.h>
#include <OgreOverlayElement.h>
#include <OgreOverlayManager.h>
#include <OgreRoot.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreParticleSystemManager.h>
#include <OgreConfigFile.h>
#include <RendererModules/Ogre/CEGUIOgreRenderer.h>

#include "GameManager.h"

#include <OIS/OIS.h>
#include <OIS/OISEvents.h>
#include <OIS/OISInputManager.h>
#include <OIS/OISKeyboard.h>
#include <OIS/OISMouse.h>

#include <CEGUI.h>

#include "GUIState.h"

class GUIManager;
class AudioManager;
class AppStateManager;
class PhysicsManager;
class LevelsManager;

//|||||||||||||||||||||||||||||||||||||||||||||||

class OgreFramework : public Ogre::Singleton<OgreFramework>, Ogre::WindowEventListener, OIS::KeyListener, OIS::MouseListener
{
public:
	OgreFramework();
	~OgreFramework();

	bool initOgre(Ogre::String wndTitle, OIS::KeyListener *pKeyListener = 0, OIS::MouseListener *pMouseListener = 0);
	/**
	* @brief carga el savegame
	*/ 
	void loadGameConfig();
	/**
	* @brief inicializa el gestor de estados
	*/ 
	void initAppStateMgr();
	/**
	* @brief inicializa el gestor de niveles
	*/ 
	void initLevelsMgr();
	void updateOgre(double timeSinceLastFrame);
	
	bool keyPressed(const OIS::KeyEvent &keyEventRef);
	bool keyReleased(const OIS::KeyEvent &keyEventRef);

	bool mouseMoved(const OIS::MouseEvent &evt);
	bool mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
	bool mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id);

	// Ogre::WindowEventListener
    void windowResized(Ogre::RenderWindow* rw);
    void windowClosed(Ogre::RenderWindow* rw);

	Ogre::Root*					m_pRoot;
	Ogre::SceneManager*			m_pActiveSceneMgr;
	Ogre::RenderWindow*			m_pRenderWnd;
	Ogre::Viewport*				m_pViewport;
	Ogre::Log*					m_pLog;
	Ogre::Timer*				m_pTimer;
	
	OIS::InputManager*			m_pInputMgr;
	OIS::Keyboard*				m_pKeyboard;
	OIS::Mouse*					m_pMouse;
	
	//encargado de manejar la pila de estados del juego
	AppStateManager* m_pAppStateManager;
	//encargado de inicializar y manejar todo lo referente a cegui
	GUIManager*					m_pGUImgr;
	//manejador de fisicas
	PhysicsManager*				m_pPhysicsManager;
	//manejador de niveles
	LevelsManager*				m_pLevelsManager;
	//audio manager
	AudioManager*				m_pAudioMgr;
	//clases con las opciones y configuraciones del juego
	GameConfig*					m_pGameConfig;

private:
	OgreFramework(const OgreFramework&);
	OgreFramework& operator= (const OgreFramework&);
};

//|||||||||||||||||||||||||||||||||||||||||||||||

#endif

//|||||||||||||||||||||||||||||||||||||||||||||||
