//|||||||||||||||||||||||||||||||||||||||||||||||

#ifndef	GAMEOVER_STATE_H
#define GAMEOVER_STATE_H

//|||||||||||||||||||||||||||||||||||||||||||||||

#include "AppState.h"

//|||||||||||||||||||||||||||||||||||||||||||||||

class GUIManager;

/**
 *  @author Jorge Rosique Contreras
 *  @date 13-6-2014
 *
 * Clase que contiene todo lo relativo a la 
 * ejecucion del estado de GameOver , que se ejecuta
 * tras morir o haber superado el nivelp
 */
class GameOverState : public AppState
{
public:
    GameOverState();

    DECLARE_APPSTATE_CLASS(GameOverState)
	/**
	* @brief Llamada cada vez que entra en el estado
	*/
	void enter();
	/**
	* @brief llamado al salir del estado cuando se hace un pop
	*/
	void exit();
	/**
	* @brief pausa el estado cuando se hace un push
	* de otro estado sobre el 
	*/

    bool keyPressed(const OIS::KeyEvent &keyEventRef);
    bool keyReleased(const OIS::KeyEvent &keyEventRef);

    bool mouseMoved(const OIS::MouseEvent &evt);
    bool mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
    bool mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
    /**
	* @brief Llamado cada frame para actualizar el estado
	* @param tiempo pasado desde el ultimo frame en ms
	*/
	void update(double timeSinceLastFrame);

private:
    bool m_bQuit;

};

//|||||||||||||||||||||||||||||||||||||||||||||||

#endif

//|||||||||||||||||||||||||||||||||||||||||||||||