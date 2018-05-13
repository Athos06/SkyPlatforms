//|||||||||||||||||||||||||||||||||||||||||||||||

#ifndef LEVELINTRO_STATE_H
#define LEVELINTRO_STATE_H

//|||||||||||||||||||||||||||||||||||||||||||||||

#include "AppState.h"

//|||||||||||||||||||||||||||||||||||||||||||||||

class GUIManager;

/**
 *  @author Jorge Rosique Contreras
 *  @date 13-6-2014
 *
 * Clase que contiene todo lo relativo a la 
 * Ejecucion del estado de la intro de los niveles
 */
class LevelIntroState : public AppState
{
public:
    LevelIntroState();

    DECLARE_APPSTATE_CLASS(LevelIntroState)
	/**
	* @brief Llamada cada vez que entra en el estado
	*/
	void enter();
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
	bool m_bFading;
	float m_faderTimer;
    bool m_bQuit;

	PhysicsManager* m_pPhysicsMgr;
};

//|||||||||||||||||||||||||||||||||||||||||||||||

#endif

//|||||||||||||||||||||||||||||||||||||||||||||||