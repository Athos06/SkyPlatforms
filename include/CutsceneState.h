//|||||||||||||||||||||||||||||||||||||||||||||||

#ifndef	CUTSCENE_STATE_H
#define CUTSCENE_STATE_H

//|||||||||||||||||||||||||||||||||||||||||||||||

#include "AppState.h"

//|||||||||||||||||||||||||||||||||||||||||||||||

class GUIManager;
/**
 *  @author Jorge Rosique Contreras
 *  @date 13-6-2014
 *
 * Clase que contiene todo lo relacinado al 
 * estado de la cutscene que se muestra al entrar
 * en el portal y superar el nivel
 */
class CutsceneState : public AppState
{
public:
    CutsceneState();

    DECLARE_APPSTATE_CLASS(CutsceneState)

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
    /**
	* @brief llamado cada frame y actualiza el estado
	*@param timeSinceLastFrame tiempo pasado desde el ultimo frame en ms
	*/
	void update(double timeSinceLastFrame);

private:
	float m_timeLenght;

	bool m_bTransition;
    bool m_bQuit;

	Ogre::SceneNode* m_pCameraNode;
	Ogre::Camera* m_pCutsceneCam;
};

//|||||||||||||||||||||||||||||||||||||||||||||||

#endif

//|||||||||||||||||||||||||||||||||||||||||||||||