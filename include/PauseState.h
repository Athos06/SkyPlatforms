//|||||||||||||||||||||||||||||||||||||||||||||||

#ifndef PAUSE_STATE_h
#define PAUSE_STATE_h

//|||||||||||||||||||||||||||||||||||||||||||||||

#include "AppState.h"

//|||||||||||||||||||||||||||||||||||||||||||||||

class GUIManager;
/**
 *  @author Jorge Rosique Contreras
 *  @date 13-6-2014
 *
 * Clase que contiene todo lo relativo a la 
 * Ejecucion del estado de pausa del juego
 */
class PauseState : public AppState
{
public:
    PauseState();

    DECLARE_APPSTATE_CLASS(PauseState)

	/**
	* @brief Llamada cada vez que entra en el estado
	*/
	void enter();
	/**
	* @brief Llamado en enter para crear el fondo del pause
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

	/**
	* @brief Llamado cada frame para actualizar el estado
	* @param tiempo pasado desde el ultimo frame en ms
	*/
	void update(double timeSinceLastFrame);

private:
	//una textura semitransparente que se pondra sobre la escena al pausarlo
	Ogre::Rectangle2D* m_pBackground;
	//el nodo al que esta asociada esta entidad
	Ogre::SceneNode* m_pNodeBg;
    bool m_bQuit;
};

//|||||||||||||||||||||||||||||||||||||||||||||||

#endif

//|||||||||||||||||||||||||||||||||||||||||||||||