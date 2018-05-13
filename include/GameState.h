//|||||||||||||||||||||||||||||||||||||||||||||||

#ifndef GAME_STATE_h
#define GAME_STATE_h

//|||||||||||||||||||||||||||||||||||||||||||||||

#include "AppState.h"
#include "BtOgrePG.h"
#include "BtOgreGP.h"
#include "BtOgreExtras.h"

class PhysicsManager;
class GameManager;
class Fader;

/**
 *  @author Jorge Rosique Contreras
 *  @date 13-6-2014
 *
 * Clase que contiene todo lo relativo a la 
 * Ejecucion del estado in game
 */
class GameState : public AppState
{
public:
	GameState();
	~GameState();

	DECLARE_APPSTATE_CLASS(GameState)

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
	* @brief llamado cuando apilamos otro estado sobre el GameState
	* haciendo que el GameState se pause
	*/
	bool pause();
	/**
	* @brief llamado cuando desapilamos el estado que habia sobre el GameState
	* haciendo que vuelva a ejecutarse el GameState
	*/
	void resume();
	/**
	* @brief llamado cada frame para obtener la entrada por teclado
	* del jugador para controlar el personaje
	*/
	void getInput();

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

	//BtOgre::DebugDrawer* mDebugDrawer;


private:	
	bool	m_bQuit;
	Fader* m_pFader;
	PhysicsManager* m_pPhysicsMgr;
	//BtOgre::DebugDrawer* mDebugDrawer;
	GameManager* m_pGameMgr;
};
//|||||||||||||||||||||||||||||||||||||||||||||||

#endif

//|||||||||||||||||||||||||||||||||||||||||||||||