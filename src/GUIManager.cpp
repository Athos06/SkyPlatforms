
#include <CEGUI.h>

#include "AdvancedOgreFramework.h"

#include "GUIManager.h"
#include "GUIStateManager.h"
#include "GUIMainMenu.h"
#include "GUICredits.h"
#include "GUIGame.h"
#include "GUIPauseMenu.h"
#include "GUIGameOver.h"
#include "GUIOptions.h"
#include "GUIAudio.h"
#include "GUIControls.h"
#include "GUISelectLevel.h"
#include "GUILevelIntro.h"
#include "GUIGameComplete.h"
#include "GUILevelComplete.h"
#include "GUISplashScreen.h"
#include "GUILoading.h"
#include "HUD.h"
#include "LoadingScreen.h"
#include "AudioManager.h"
#include "Constants.h"

//|||||||||||||||||||||||||||||||||||||||||||||||

template<> GUIManager* Ogre::Singleton<GUIManager>::msSingleton = 0;

//|||||||||||||||||||||||||||||||||||||||||||||||


GUIManager& GUIManager::getSingleton(){
assert (msSingleton); 
return (*msSingleton);
}
 
GUIManager* GUIManager::getSingletonPtr(){
assert (msSingleton);
return msSingleton;
}

GUIManager::GUIManager(){
	m_pGUIRenderer		= 0;
    m_pGUISystem		= 0;
	m_pGUIStateManager	= 0;
	m_pLoadingScreen	= 0;
	m_pHUD				= 0;
}

GUIManager::~GUIManager(){
	if(m_pLoadingScreen) delete m_pLoadingScreen;
	if(m_pHUD) delete m_pHUD;
	
}

void GUIManager::initGUI(){
	m_pGUIRenderer = &CEGUI::OgreRenderer::bootstrapSystem( *(OgreFramework::getSingletonPtr()->m_pRenderWnd) );
    m_pGUISystem = CEGUI::System::getSingletonPtr();
	//creacion del manejador de estados de la GUI (debe crearse antes
	//de crear la GUI puesto que se requiere de los estados creados para
	//suscribir los metodos de los botones de la GUI)
	m_pGUIStateManager = new GUIStateManager();
	
	GUIMainMenu::create(m_pGUIStateManager, "GUIMainMenu");
	GUICredits::create(m_pGUIStateManager, "GUICredits");
	GUIGame::create(m_pGUIStateManager, "GUIGame");
	GUIPauseMenu::create(m_pGUIStateManager, "GUIPauseMenu");
	GUIGameOver::create(m_pGUIStateManager, "GUIGameOver");
	GUIOptions::create(m_pGUIStateManager, "GUIOptions");
	GUIAudio::create(m_pGUIStateManager, "GUIAudio");
	GUIControls::create(m_pGUIStateManager, "GUIControls");
	GUISelectLevel::create(m_pGUIStateManager, "GUISelectLevel");
	GUILevelIntro::create(m_pGUIStateManager, "GUILevelIntro");
	GUILevelComplete::create(m_pGUIStateManager, "GUILevelComplete");
	GUIGameComplete::create(m_pGUIStateManager, "GUIGameComplete");
	GUISplashScreen::create(m_pGUIStateManager, "GUISplashScreen");
	GUILoading::create(m_pGUIStateManager, "GUILoading");
	createGUI();

}

void GUIManager::createGUI(){
	CreateMainMenuGUI();
	CreateInGameGUI();
	CreateCreditsGUI();
	CreatePauseMenuGUI();
	CreateGameOverGUI();
	CreateOptionsGUI();
	CreateAudioGUI();
	CreateControlsGUI();
	CreateSelectLevelGUI();
	CreateLevelIntroGUI();
	CreateLevelCompleteGUI();
	CreateGameCompleteGUI();
	CreateSplashScreenGUI();
	CreateLoadingGUI();
	
	//crea el HUD
	m_pHUD = new HUD();
	//creo la instancia de loading screen que no es mas
	//que una fachada para acceder a la gui de load level
	//y pasarle el tanto por ciento que lleva cargado
	m_pLoadingScreen = new LoadingScreen();

}
//|||||||||||||||||||||||||||||||||||||||||||||||

void GUIManager::CreateMainMenuGUI(){

	CEGUI::Scheme::setDefaultResourceGroup("Schemes");
	CEGUI::Imageset::setDefaultResourceGroup("Imagesets");
	CEGUI::Font::setDefaultResourceGroup("Fonts");
	CEGUI::WindowManager::setDefaultResourceGroup("Layouts");
	CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
	CEGUI::SchemeManager::getSingleton().create("TaharezLook.scheme");
	m_pGUISystem->setDefaultMouseCursor("TaharezLook","MouseArrow"); 
	
	//cargar la GUI sheet del main menu
	m_pGUIsheetMainMenu = CEGUI::WindowManager::getSingleton().loadWindowLayout("MainMenu.layout");

	CEGUI::Window* backButton = CEGUI::WindowManager::getSingleton().getWindow("MainMenuRoot/BackButton");
	backButton->subscribeEvent(CEGUI::PushButton::EventClicked, 
		CEGUI::Event::Subscriber(&GUIMainMenu::onBackButton, 
		GUIMainMenu::getSingletonPtr() ));
	backButton->subscribeEvent(CEGUI::PushButton::EventMouseEnters,
		CEGUI::Event::Subscriber(&GUIManager::OnHoverSound, 
		GUIManager::getSingletonPtr() ));

	//asociar los botones del main menu con las funciones correspondientes a las que llaman
	CEGUI::Window* exitButton = CEGUI::WindowManager::getSingleton().getWindow("MainMenuRoot/ExitButton");
	exitButton->subscribeEvent(CEGUI::PushButton::EventClicked, 
		CEGUI::Event::Subscriber(&GUIMainMenu::onExitButton, 
		GUIMainMenu::getSingletonPtr() ));
	exitButton->subscribeEvent(CEGUI::PushButton::EventMouseEnters,
		CEGUI::Event::Subscriber(&GUIManager::OnHoverSound, 
		GUIManager::getSingletonPtr() ));

	CEGUI::Window* creditsButton = CEGUI::WindowManager::getSingleton().getWindow("MainMenuRoot/CreditsButton");
	creditsButton->subscribeEvent(CEGUI::PushButton::EventClicked, 
		CEGUI::Event::Subscriber(&GUIMainMenu::onCreditsButton, 
		GUIMainMenu::getSingletonPtr() ));
	creditsButton->subscribeEvent(CEGUI::PushButton::EventMouseEnters,
		CEGUI::Event::Subscriber(&GUIManager::OnHoverSound, 
		GUIManager::getSingletonPtr() ));

	CEGUI::Window* newGameButton = CEGUI::WindowManager::getSingleton().getWindow("MainMenuRoot/PlayButton");
	newGameButton->subscribeEvent(CEGUI::PushButton::EventClicked, 
		CEGUI::Event::Subscriber( &GUIMainMenu::onPlayButton, 
		GUIMainMenu::getSingletonPtr() ) );  
	newGameButton->subscribeEvent(CEGUI::PushButton::EventMouseEnters,
		CEGUI::Event::Subscriber(&GUIManager::OnHoverSound, 
		GUIManager::getSingletonPtr() ));

	CEGUI::Window* optionsButton = CEGUI::WindowManager::getSingleton().getWindow("MainMenuRoot/OptionsButton");
	optionsButton->subscribeEvent(CEGUI::PushButton::EventClicked, 
		CEGUI::Event::Subscriber(&GUIMainMenu::onOptionsButton, 
		GUIMainMenu::getSingletonPtr() ));
	optionsButton->subscribeEvent(CEGUI::PushButton::EventMouseEnters,
		CEGUI::Event::Subscriber(&GUIManager::OnHoverSound, 
		GUIManager::getSingletonPtr() ));

}
//|||||||||||||||||||||||||||||||||||||||||||||||

void GUIManager::CreateLoadingGUI(){
	m_pGUIsheetLoading = CEGUI::WindowManager::getSingleton().
		loadWindowLayout("Loading.layout");
}

void GUIManager::CreateInGameGUI(){
	m_pGUIsheetInGame = CEGUI::WindowManager::getSingleton().loadWindowLayout("InGame.layout");
}


void GUIManager::CreateSplashScreenGUI(){
	m_pGUIsheetSplashScreen = 
		CEGUI::WindowManager::getSingleton().loadWindowLayout("SplashScreen.layout");
}
//inicializa la GUI sheet de los creditos
void GUIManager::CreateCreditsGUI(){
	
	//cargar la GUI sheet del main menu
	m_pGUIsheetCredits = CEGUI::WindowManager::getSingleton().loadWindowLayout("Credits.layout");

	/*
	//asociar los botones del main menu con las funciones correspondientes a las que llaman
	CEGUI::Window* backButton = CEGUI::WindowManager::getSingleton().getWindow
		("CreditsRoot/Background/ExitButton");
	backButton->subscribeEvent(CEGUI::PushButton::EventClicked, 
		CEGUI::Event::Subscriber(&GUICredits::onBackButton, 
		GUICredits::getSingletonPtr() ));
	*/	
}

//|||||||||||||||||||||||||||||||||||||||||||||||

//inicializa la GUI sheet del menu de pausa
void GUIManager::CreatePauseMenuGUI(){
	
	//cargar la GUI sheet del menu de pausa
	m_pGUIsheetPauseMenu = CEGUI::WindowManager::getSingleton().loadWindowLayout("PauseMenu.layout");

	//asociar los botones del main menu con las funciones correspondientes a las que llaman
	CEGUI::Window* backButton = CEGUI::WindowManager::getSingleton().getWindow
		("PauseMenuRoot/ResumeButton");
	backButton->subscribeEvent(CEGUI::PushButton::EventClicked, 
		CEGUI::Event::Subscriber(&GUIPauseMenu::onResumeButton, 
		GUIPauseMenu::getSingletonPtr() ));
	backButton->subscribeEvent(CEGUI::PushButton::EventMouseEnters,
		CEGUI::Event::Subscriber(&GUIManager::OnHoverSound, 
		GUIManager::getSingletonPtr() ));
	
	CEGUI::Window* resumeButton = CEGUI::WindowManager::getSingleton().getWindow
		("PauseMenuRoot/PlayButton");
	resumeButton->subscribeEvent(CEGUI::PushButton::EventClicked, 
		CEGUI::Event::Subscriber(&GUIPauseMenu::onResumeButton, 
		GUIPauseMenu::getSingletonPtr() ));
	resumeButton->subscribeEvent(CEGUI::PushButton::EventMouseEnters,
		CEGUI::Event::Subscriber(&GUIManager::OnHoverSound, 
		GUIManager::getSingletonPtr() ));
	
	CEGUI::Window* optionsButton = CEGUI::WindowManager::getSingleton().getWindow
		("PauseMenuRoot/OptionsButton");
	optionsButton->subscribeEvent(CEGUI::PushButton::EventClicked, 
		CEGUI::Event::Subscriber(&GUIPauseMenu::onOptionsButton, 
		GUIPauseMenu::getSingletonPtr() ));
	optionsButton->subscribeEvent(CEGUI::PushButton::EventMouseEnters,
		CEGUI::Event::Subscriber(&GUIManager::OnHoverSound, 
		GUIManager::getSingletonPtr() ));
	
	CEGUI::Window* MainMenuButton = CEGUI::WindowManager::getSingleton().getWindow
		("PauseMenuRoot/BackToMainButton");
	MainMenuButton->subscribeEvent(CEGUI::PushButton::EventClicked,
		CEGUI::Event::Subscriber(&GUIPauseMenu::onMainMenuButton,
		GUIPauseMenu::getSingletonPtr() ));
	MainMenuButton->subscribeEvent(CEGUI::PushButton::EventMouseEnters,
		CEGUI::Event::Subscriber(&GUIManager::OnHoverSound, 
		GUIManager::getSingletonPtr() ));
	
	CEGUI::Window* exitButton = CEGUI::WindowManager::getSingleton().getWindow
		("PauseMenuRoot/ExitButton");
	exitButton->subscribeEvent(CEGUI::PushButton::EventClicked,
		CEGUI::Event::Subscriber(&GUIPauseMenu::onExitButton,
		GUIPauseMenu::getSingletonPtr() ));
	exitButton->subscribeEvent(CEGUI::PushButton::EventMouseEnters,
		CEGUI::Event::Subscriber(&GUIManager::OnHoverSound, 
		GUIManager::getSingletonPtr() ));
	
}


//|||||||||||||||||||||||||||||||||||||||||||||||

void GUIManager::CreateGameOverGUI(){
	
	//cargar la GUI sheet del main menu
	m_pGUIsheetGameOver = CEGUI::WindowManager::getSingleton().loadWindowLayout("GameOver.layout");


	//asociar los botones del main menu con las funciones correspondientes a las que llaman
	CEGUI::Window* BackToMenuButton = CEGUI::WindowManager::getSingleton().getWindow
		("GameOverRoot/ExitButton");
	BackToMenuButton->subscribeEvent(CEGUI::PushButton::EventClicked, 
		CEGUI::Event::Subscriber(&GUIGameOver::onBackButton, 
		GUIGameOver::getSingletonPtr() ));
	BackToMenuButton->subscribeEvent(CEGUI::PushButton::EventMouseEnters,
		CEGUI::Event::Subscriber(&GUIManager::OnHoverSound, 
		GUIManager::getSingletonPtr() ));
	
	CEGUI::Window* playAgainButton = CEGUI::WindowManager::getSingleton().getWindow
		("GameOverRoot/ContinueButton");
	playAgainButton->subscribeEvent(CEGUI::PushButton::EventClicked, 
		CEGUI::Event::Subscriber(&GUIGameOver::onContinueButton, 
		GUIGameOver::getSingletonPtr() ));
	playAgainButton->subscribeEvent(CEGUI::PushButton::EventMouseEnters,
		CEGUI::Event::Subscriber(&GUIManager::OnHoverSound, 
		GUIManager::getSingletonPtr() ));
	
	CEGUI::Window* restartButton = CEGUI::WindowManager::getSingleton().getWindow
		("GameOverRoot/RestartButton");
	restartButton->subscribeEvent(CEGUI::PushButton::EventClicked,
		CEGUI::Event::Subscriber(&GUIGameOver::onRestartButton, 
		GUIGameOver::getSingletonPtr()) );
	restartButton->subscribeEvent(CEGUI::PushButton::EventMouseEnters,
		CEGUI::Event::Subscriber(&GUIManager::OnHoverSound, 
		GUIManager::getSingletonPtr() ));
	
}

void GUIManager::CreateLevelCompleteGUI(){
	m_pGUIsheetLevelComplete = CEGUI::WindowManager::getSingleton().
		loadWindowLayout("LevelComplete.layout");

	//asociar los botones del main menu con las funciones correspondientes a las que llaman
	CEGUI::Window* continueButton = CEGUI::WindowManager::getSingleton().getWindow
		("LevelCompleteRoot/ContinueButton");
	continueButton->subscribeEvent(CEGUI::PushButton::EventClicked, 
		CEGUI::Event::Subscriber(&GUILevelComplete::onContinueButton, 
		GUILevelComplete::getSingletonPtr() ));
	continueButton->subscribeEvent(CEGUI::PushButton::EventMouseEnters,
		CEGUI::Event::Subscriber(&GUIManager::OnHoverSound, 
		GUIManager::getSingletonPtr() ));
	
	CEGUI::Window* exitButton = CEGUI::WindowManager::getSingleton().getWindow
		("LevelCompleteRoot/ExitButton");
	exitButton->subscribeEvent(CEGUI::PushButton::EventClicked, 
		CEGUI::Event::Subscriber(&GUILevelComplete::onExitButton, 
		GUILevelComplete::getSingletonPtr() ));
	exitButton->subscribeEvent(CEGUI::PushButton::EventMouseEnters,
		CEGUI::Event::Subscriber(&GUIManager::OnHoverSound, 
		GUIManager::getSingletonPtr() ));
	
}


//|||||||||||||||||||||||||||||||||||||||||||||||

void GUIManager::CreateOptionsGUI(){
	//cargar la GUI sheet del main menu
	m_pGUIsheetOptions = CEGUI::WindowManager::getSingleton().loadWindowLayout("OptionsMenu.layout");

	CEGUI::Window* AudioButton = CEGUI::WindowManager::getSingleton().getWindow
		("OptionsRoot/AudioButton");
	AudioButton->subscribeEvent(CEGUI::PushButton::EventClicked, 
		CEGUI::Event::Subscriber(&GUIOptions::onAudioButton, 
		GUIOptions::getSingletonPtr() ));
	AudioButton->subscribeEvent(CEGUI::PushButton::EventMouseEnters,
		CEGUI::Event::Subscriber(&GUIManager::OnHoverSound, 
		GUIManager::getSingletonPtr() ));
	


	//asociar los botones del main menu con las funciones correspondientes a las que llaman
	CEGUI::Window* ControlsButton = CEGUI::WindowManager::getSingleton().getWindow
		("OptionsRoot/ControlsButton");
	ControlsButton->subscribeEvent(CEGUI::PushButton::EventClicked, 
		CEGUI::Event::Subscriber(&GUIOptions::onControlsButton, 
		GUIOptions::getSingletonPtr() ));
	ControlsButton->subscribeEvent(CEGUI::PushButton::EventMouseEnters,
		CEGUI::Event::Subscriber(&GUIManager::OnHoverSound, 
		GUIManager::getSingletonPtr() ));

	CEGUI::Window* BackButton = CEGUI::WindowManager::getSingleton().getWindow
		("OptionsRoot/BackButton");
	BackButton->subscribeEvent(CEGUI::PushButton::EventClicked, 
		CEGUI::Event::Subscriber(&GUIOptions::onBackButton, 
		GUIOptions::getSingletonPtr() ));
	BackButton->subscribeEvent(CEGUI::PushButton::EventMouseEnters,
		CEGUI::Event::Subscriber(&GUIManager::OnHoverSound, 
		GUIManager::getSingletonPtr() ));
	
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void GUIManager::CreateAudioGUI(){
	m_pGUIsheetAudio = CEGUI::WindowManager::getSingleton().loadWindowLayout("Audio.layout");


	//el spinner para el control de volumen de musica
	CEGUI::Window* musicVolume = CEGUI::WindowManager::getSingleton().getWindow
		("AudioRoot/bg/MusicVolumeControl");
	musicVolume->subscribeEvent(CEGUI::Spinner::EventValueChanged, 
		CEGUI::Event::Subscriber(&GUIAudio::musicVolumeChange, 
		GUIAudio::getSingletonPtr() ));
	//el spinner para el control de volumen de sonidos
	CEGUI::Window* sfxVolume = CEGUI::WindowManager::getSingleton().getWindow
		("AudioRoot/bg/SFXVolumeControl");
	sfxVolume->subscribeEvent(CEGUI::Spinner::EventValueChanged, 
		CEGUI::Event::Subscriber(&GUIAudio::sfxVolumeChange, 
		GUIAudio::getSingletonPtr() ));

	CEGUI::Window* BackButton = CEGUI::WindowManager::getSingleton().getWindow
		("AudioRoot/BackButton");
	BackButton->subscribeEvent(CEGUI::PushButton::EventClicked, 
		CEGUI::Event::Subscriber(&GUIAudio::onBackButton, 
		GUIAudio::getSingletonPtr() ));
	BackButton->subscribeEvent(CEGUI::PushButton::EventMouseEnters,
		CEGUI::Event::Subscriber(&GUIManager::OnHoverSound, 
		GUIManager::getSingletonPtr() ));
	
}
//|||||||||||||||||||||||||||||||||||||||||||||||

void GUIManager::CreateControlsGUI(){
	//cargar la GUI sheet del main menu
	m_pGUIsheetControls = CEGUI::WindowManager::getSingleton().loadWindowLayout("Controls.layout");


	//asociar los botones del main menu con las funciones correspondi	entes a las que llaman
	CEGUI::Window* BackButton = CEGUI::WindowManager::getSingleton().getWindow
		("ControlsRoot/BackButton");
	BackButton->subscribeEvent(CEGUI::PushButton::EventClicked, 
		CEGUI::Event::Subscriber(&GUIControls::onBackButton, 
		GUIControls::getSingletonPtr() ));
	BackButton->subscribeEvent(CEGUI::PushButton::EventMouseEnters,
		CEGUI::Event::Subscriber(&GUIManager::OnHoverSound, 
		GUIManager::getSingletonPtr() ));

	CEGUI::Window* xInvertCheck = CEGUI::WindowManager::getSingleton().getWindow
		("ControlsRoot/controlsBG/xInvertCheck");
	xInvertCheck->subscribeEvent(CEGUI::Checkbox::EventCheckStateChanged, 
		CEGUI::Event::Subscriber(&GUIControls::onXInvertCheck, 
		GUIControls::getSingletonPtr() ));
	
	CEGUI::Window* yInvertCheck = CEGUI::WindowManager::getSingleton().getWindow
		("ControlsRoot/controlsBG/yInvertCheck");
	yInvertCheck->subscribeEvent(CEGUI::Checkbox::EventCheckStateChanged, 
		CEGUI::Event::Subscriber(&GUIControls::onYInvertCheck, 
		GUIControls::getSingletonPtr() ));

	CEGUI::Window* sentivitySpinner = CEGUI::WindowManager::getSingleton().getWindow
		("ControlsRoot/controlsBG/sensitivity");
	sentivitySpinner->subscribeEvent(CEGUI::Spinner::EventValueChanged, 
		CEGUI::Event::Subscriber(&GUIControls::onSensitivity, 
		GUIControls::getSingletonPtr() ));
}
//|||||||||||||||||||||||||||||||||||||||||||||||

void GUIManager::CreateSelectLevelGUI(){
	//cargar la GUI sheet del main menu
	m_pGUIsheetSelectLevel = CEGUI::WindowManager::getSingleton().loadWindowLayout("SelectLevel.layout");
	//asociar los botones del main menu con las funciones correspondientes a las que llaman
	CEGUI::Window* BackButton = CEGUI::WindowManager::getSingleton().getWindow
		("SelectLevelRoot/BackButton");
	BackButton->subscribeEvent(CEGUI::PushButton::EventClicked, 
		CEGUI::Event::Subscriber(&GUISelectLevel::onBackButton, 
		GUISelectLevel::getSingletonPtr() ));
	BackButton->subscribeEvent(CEGUI::PushButton::EventMouseEnters,
		CEGUI::Event::Subscriber(&GUIManager::OnHoverSound, 
		GUIManager::getSingletonPtr() ));
	
	CEGUI::Window* lvl1Button = CEGUI::WindowManager::getSingleton().getWindow
		("SelectLevelRoot/Lvl1Button");
	lvl1Button ->subscribeEvent(CEGUI::PushButton::EventClicked, 
		CEGUI::Event::Subscriber(&GUISelectLevel::onLvl1Button, 
		GUISelectLevel::getSingletonPtr() ));
	lvl1Button->subscribeEvent(CEGUI::PushButton::EventMouseEnters,
		CEGUI::Event::Subscriber(&GUIManager::OnHoverSound, 
		GUIManager::getSingletonPtr() ));
	
	CEGUI::Window* lvl2Button = CEGUI::WindowManager::getSingleton().getWindow
		("SelectLevelRoot/Lvl2Button");
	lvl2Button ->subscribeEvent(CEGUI::PushButton::EventClicked, 
		CEGUI::Event::Subscriber(&GUISelectLevel::onLvl2Button, 
		GUISelectLevel::getSingletonPtr() ));
	lvl2Button->subscribeEvent(CEGUI::PushButton::EventMouseEnters,
		CEGUI::Event::Subscriber(&GUIManager::OnHoverSound, 
		GUIManager::getSingletonPtr() ));
	
	CEGUI::Window* lvl3Button = CEGUI::WindowManager::getSingleton().getWindow
		("SelectLevelRoot/Lvl3Button");
	lvl3Button ->subscribeEvent(CEGUI::PushButton::EventClicked, 
		CEGUI::Event::Subscriber(&GUISelectLevel::onLvl3Button, 
		GUISelectLevel::getSingletonPtr() ));
	lvl3Button->subscribeEvent(CEGUI::PushButton::EventMouseEnters,
		CEGUI::Event::Subscriber(&GUIManager::OnHoverSound, 
		GUIManager::getSingletonPtr() ));
	
	CEGUI::Window* lvlBossButton = CEGUI::WindowManager::getSingleton().getWindow
		("SelectLevelRoot/LvlBossButton");
	lvlBossButton ->subscribeEvent(CEGUI::PushButton::EventClicked, 
		CEGUI::Event::Subscriber(&GUISelectLevel::onLvlBossButton, 
		GUISelectLevel::getSingletonPtr() ));
	lvlBossButton->subscribeEvent(CEGUI::PushButton::EventMouseEnters,
		CEGUI::Event::Subscriber(&GUIManager::OnHoverSound, 
		GUIManager::getSingletonPtr() ));
}

void GUIManager::CreateLevelIntroGUI(){
	m_pGUIsheetLevelIntro = CEGUI::WindowManager::getSingleton().loadWindowLayout("LevelIntro.layout");
}

void GUIManager::CreateGameCompleteGUI(){
	m_pGUIsheetGameComplete = CEGUI::WindowManager::getSingleton().
		loadWindowLayout("GameComplete.layout");
}

bool GUIManager::OnHoverSound(const CEGUI::EventArgs &args){
	AudioManager::getSingletonPtr()->playFX("Hover", 0, MENU2_CHANNEL);
	return true;
}
