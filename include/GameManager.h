#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <Ogre.h>

class Player;
class CharacterController;
class HUD;
class Portal;

/**
 *  @author Jorge Rosique Contreras
 *  @date 11-6-2014
 *
 * La clase GameManager se trata de una clase auxiliar que 
 * se encarga de albergar los elementos relacionados con 
 * el funcionamiento fundamental del juego. En el podemos obtener acceso
 * a diferentes elementos importantes del juego como el Player y el Portal,
 * gestionar la logica de comienzo y fin de nivel, asi com llamar a actualizar 
 * otros elementos como el HUD
 */
class GameManager : public Ogre::Singleton<GameManager>{
public:
	static GameManager& getSingleton();
	static GameManager* getSingletonPtr();
	/**
	@brief constructor
	@param sceneMgr puntero al scene manager actual
	*/ 
	GameManager(Ogre::SceneManager* sceneMgr);
	~GameManager();
	
	/**
	* @brief Llamado cada frame, se encarga de llamar a diferentes funciones
	* relacionadas con el funcionamiento del juego. Llama a update del
	* player, del HUD, y comprueba si se cumple la condicion de fin del nivel
	* @param timeSinceLastFrame tiempo pasado ddesde el ultimo frame en ms
	*/
	void update(double timeSinceLastFrame);
	/**
	@brief inicializa el GameManager
	*/ 
	void initGameManager();
	/**
	@brief destruye el GameManager
	*/ 
	void destroy();
	/**
	* @brief llamada una vez se pasa el nivel y activa la flag m_passLevel
	* que indica que lo hemos superado
	*/
	void completeLevel();

	/**
	* @brief es llamada cada frame para ver si hemos cumplido
	* la condicion para superar el nivel o si hemos muerto, y comenzar 
	* la logica de fin del nivel en consecuencia
	*/
	void getGameOverCondition();
	/**
	* @brief llamada una vez se termino la transicion de finalizar nivel y
	* se encarga de la logica para finalizar este, teniendo en cuenta si 
	* lo hemos superado o hemos muerto
	*/
	void gameOver();
	/**
	* @brief empieza el nivel
	*/
	void startGame();
	/**
	* @brief permite continuar el nivel una vez hemos muerto
	* pero aun nos quedan vidas
	*/
	void retryLevel();
	/**
	* @brief actualiza el numero de vidas
	*/
	void updateLives();
	/**
	* @brief actualiza el numero de skystones
	*/
	void updateStones();
	/**
	* @brief  establece cual es el spawnpoint por defecto del nivel.
	*/
	void setDefaultSpawnPoint(Ogre::SceneNode* spawnpoint){
		m_pDefaultSpawnPoint = spawnpoint;
		m_pActiveSpawnPoint	= spawnpoint;
	};
	/**
	* @brief  establece cual es el spawnpoint actual para la proxima
	* vez que reaparezcamos
	*/
	void setActiveSpawnPoint(Ogre::SceneNode* spawnpoint){
		m_pActiveSpawnPoint	= spawnpoint;
	};
	/**
	* @brief  acceso al sceneManager actual
	* @return puntero al SceneManager actual
	*/
	Ogre::SceneManager* getSceneMgr() const {
		return m_pSceneMgr;
	}
		/**
	* @brief  acceso al objeto Player, que es la clase
	* que alberga todo lo relacionado con el jugador
	* @return puntero al Player
	*/
	Player* getPlayer() const {
		return m_pPlayer;
	}
	/**
	* @brief  comprobacion para ver si hay alguna transicion
	* en curso y estamos esperando
	* @return si estamos esperando o no
	*/
	bool isWaiting(){return m_isWaiting;};
	/**
	* @brief  comprobacion para ver si hemos superado el nivel
	* @return si hemos superado el nivel o no
	*/
	bool isPassLevel(){return m_passLevel;};
	/**
	* @brief  comprobacion para ver si hemos superado el ultimo
	* nivel y hemos completado el juego
	* @return si hemos completado el juego o no
	*/
	bool isGameComplete(){return m_gameComplete;};
	/**
	* @brief  Indicamos que hemos superado el ultimo nivel
	* y completado el juego
	*/
	void setGameComplete(bool set){m_gameComplete = set;};
	/**
	* @return  numero de skystones recogidas
	*/
	int getSkystones() const{return m_nSkystones;};
	/**
	* @return  numero de vidas del jugador
	*/
	int getLives() const{return m_nLives;};
	
	Ogre::SceneNode* m_pDefaultSpawnPoint;
	Ogre::SceneNode* m_pActiveSpawnPoint;

	Portal* m_pPortal;	
	
private:
	//numero de skystones recogidas
	int m_nSkystones;
	//numero de vidas restantes
	int  m_nLives;
	//indica si hemos superado el nivel
	bool m_passLevel;
	//indica si hemos completado el juego
	bool m_gameComplete;
	//indica si estamos esperando en una transicion
	bool m_isWaiting; 
	//timer del tiempo de espera
	float m_waitingTime;
	//el tiempo por el que estara bloqueado
	float m_waitForSeconds;
	//flag que indica que estamos en la transicion de terminar el nivel
	bool m_isEnding;
	//flag que indica cuando el efecto de fadein o fadeout esta
	//activo
	bool m_faderActive;

	Ogre::SceneManager* m_pSceneMgr;
	Player* m_pPlayer;
	
};

class GameConfig : public Ogre::Singleton<GameConfig>{
public:
	static GameConfig& getSingleton();
	static GameConfig* getSingletonPtr();

	GameConfig();
	~GameConfig();
	
	int volume;
	int sfxVolume;

	int xInverted;
	int yInverted;

	int mouseSensitivity;

	bool levelsLock[4];
};

#endif