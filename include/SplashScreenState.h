//|||||||||||||||||||||||||||||||||||||||||||||||

#ifndef SPLASHSCREEN_STATE_H
#define SPLASHSCREEN_STATE_H

//|||||||||||||||||||||||||||||||||||||||||||||||

#include "AppState.h"

//|||||||||||||||||||||||||||||||||||||||||||||||

class SplashScreenState : public AppState
{
public:
    SplashScreenState();

	DECLARE_APPSTATE_CLASS(SplashScreenState)

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
	/**
	* @brief llamado al volver al estado tras desapilar el estado que habia
	* sobre el (el mainState del main menu)
	*/
	void resume();

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
	Ogre::SceneNode* m_pCamNode;
};


#endif
