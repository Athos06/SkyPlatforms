#ifndef GUI_MANAGER_H
#define GUI_MANAGER_H

#include <CEGUI.h>
#include <RendererModules/Ogre/CEGUIOgreRenderer.h>

#include <Ogre.h>

class GUIStateManager;
class LoadingScreen;
class HUD;
/**
 * @author Jorge Rosique Contreras
 * @date 13-6-2014
 *
 * Esta clase se encarga de gestionar todo lo relacionado con la creacion
 * e inicializacion de la GUI del juego
 */
class GUIManager : public Ogre::Singleton<GUIManager>{
public:
	GUIManager();
	~GUIManager();
	
	static GUIManager& getSingleton();
	static GUIManager* getSingletonPtr();

		
	/**
	@brief inicializa CEGUI y la GUI del juego y  anade a la pila
	los diferentes estados de la GUI
	*/
	void initGUI();
	/**
	@brief Llama a las funciones de carga de las GUI sheet de CEGUI
	*/
	void createGUI();
	/**
	@brief carga la GUI sheet del menu principal
	*/
	void CreateMainMenuGUI();
	/**
	@brief carga la GUI sheet de los creditos
	*/
	void CreateCreditsGUI();
	/**
	@brief carga la GUI sheet del menu de pausa
	*/
	void CreatePauseMenuGUI();
	/**
	@brief carga la GUI sheet del menu de game over
	*/
	void CreateGameOverGUI();
	/**
	@brief carga la GUI sheet del menu de opciones
	*/
	void CreateOptionsGUI();
	/**
	@brief carga la GUI sheet del menu de audio
	*/
	void CreateAudioGUI();
	/**
	@brief carga la GUI sheet del menu de controles
	*/
	void CreateControlsGUI();
	/**
	@brief carga la GUI sheet in game
	*/
	void CreateInGameGUI();
	/**
	@brief carga la GUI sheet del menu de seleccion de nivel
	*/
	void CreateSelectLevelGUI();
	/**
	@brief carga la GUI sheet de la intro de los niveles
	*/
	void CreateLevelIntroGUI();
	/**
	@brief carga la GUI sheet del menu de nivel superado
	*/
	void CreateLevelCompleteGUI();
	/**
	@brief carga la GUI sheet de la escena final del juego
	*/
	void CreateGameCompleteGUI();
	/**
	@brief carga la GUI sheet de la splash screen
	*/
	void CreateSplashScreenGUI();
	/**
	@brief carga la GUI sheet de la pantalla de carga
	*/
	void CreateLoadingGUI();
	
	/**
	@brief Llamado cuando se produce un evento de el raton 
	pasando por encima de un boton en CEGUI
	*/
	bool OnHoverSound(const CEGUI::EventArgs &args);

	CEGUI::OgreRenderer*		m_pGUIRenderer;
	CEGUI::System*				m_pGUISystem;

	//punteros a cada una de las cegui Window con la GUI correspondiente
	CEGUI::Window*				m_pGUIsheetMainMenu;
	CEGUI::Window*				m_pGUIsheetInGame;
	CEGUI::Window*				m_pGUIsheetCredits;
	CEGUI::Window*				m_pGUIsheetPauseMenu;
	CEGUI::Window*				m_pGUIsheetGameOver;
	CEGUI::Window*				m_pGUIsheetLevelComplete;
	CEGUI::Window*				m_pGUIsheetOptions;
	CEGUI::Window*				m_pGUIsheetAudio;
	CEGUI::Window*				m_pGUIsheetControls;
	CEGUI::Window*				m_pGUIsheetSelectLevel;
	CEGUI::Window*				m_pGUIsheetLevelIntro;
	CEGUI::Window*				m_pGUIsheetGameComplete;
	CEGUI::Window*				m_pGUIsheetSplashScreen;
	CEGUI::Window*				m_pGUIsheetLoading;

	GUIStateManager* m_pGUIStateManager;
	LoadingScreen* m_pLoadingScreen;
	HUD* m_pHUD;
private:
	
};

#endif