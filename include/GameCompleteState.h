//|||||||||||||||||||||||||||||||||||||||||||||||

#ifndef	GAMECOMPLETE_STATE_H
#define GAMECOMPLETE_STATE_H

//|||||||||||||||||||||||||||||||||||||||||||||||

#include "AppState.h"

//|||||||||||||||||||||||||||||||||||||||||||||||

class GUIManager;
class CharacterAnimation;
class Fader;

/**
 *  @author Jorge Rosique Contreras
 *  @date 13-6-2014
 *
 * Clase que contiene todo lo relacinado al 
 * estado de la escena de fin del juego
 */
class GameCompleteState : public AppState
{
public:
    GameCompleteState();

    DECLARE_APPSTATE_CLASS(GameCompleteState)
    
	/**
	* @brief Llamada cada vez que entra en el estado
	*/
	void enter();
	/**
	* @brief Llamado en enter para crear la escena
	*/
	void createScene();
	/**
	* @brief llamado al salir del estado cuando se hace un pop
	*/
    void exit();

    bool keyPressed(const OIS::KeyEvent &keyEventRef);
    bool keyReleased(const OIS::KeyEvent &keyEventRef);

    bool mouseMoved(const OIS::MouseEvent &evt);
    bool mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
    bool mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
  
	void update(double timeSinceLastFrame);

private:
    bool m_bQuit;

	bool m_bFading;
	float m_faderTimer;

	//para poder actualizar la animacion del personaje en la cutscene
	CharacterAnimation* m_pCharAnim;

	Fader* m_pFader;
};

//|||||||||||||||||||||||||||||||||||||||||||||||

#endif

//|||||||||||||||||||||||||||||||||||||||||||||||