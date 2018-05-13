//|||||||||||||||||||||||||||||||||||||||||||||||

#ifndef CREDITS_STATE_h
#define CREDITS_STATE_h

//|||||||||||||||||||||||||||||||||||||||||||||||


#include <OgreSubEntity.h>
#include <OgreMaterialManager.h>

#include "AppState.h"
#include "DotSceneLoader.h"

#include "BtOgrePG.h"
#include "BtOgreGP.h"
#include "BtOgreExtras.h"

class CharacterController;
class GameManager;

/**
 *  @author Jorge Rosique Contreras
 *  @date 12-6-2014
 *
 * Clase que contiene todo lo relativo a la 
 * ejecucion del estado de los creditos
 */
class CreditsState : public AppState
{
public:
	CreditsState();
	~CreditsState();

	DECLARE_APPSTATE_CLASS(CreditsState)
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
	* @brief pausa el estado cuando se hace un push
	* de otro estado sobre el 
	*/
	bool pause();
	/**
	* @brief llamado cuando se desapila el estado que
	* habia sobre el y se retorna a este estado
	*/
	void resume();

	bool keyPressed(const OIS::KeyEvent &keyEventRef);
	bool keyReleased(const OIS::KeyEvent &keyEventRef);

	bool mouseMoved(const OIS::MouseEvent &arg);
	bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	/**
	* @brief Llamado cada frame para actualizar el estado
	* @param tiempo pasado desde el ultimo frame en ms
	*/
	void update(double timeSinceLastFrame);

private:	
	bool m_fading;
	bool m_bQuit;
		

};
//|||||||||||||||||||||||||||||||||||||||||||||||

#endif

//|||||||||||||||||||||||||||||||||||||||||||||||