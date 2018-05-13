#ifndef LEVEL1_H
#define LEVEL1_H

#include "Level.h"
#include <Ogre.h>
#include "PhysicsManager.h"

class GameManager;
class Fader;

/**
 *  @author Jorge Rosique Contreras
 *  @date 12-6-2014
 *
 * Clase que contiene todo lo relativo al nivel1
 */
class Level1 : public Level{
public:
	Level1();
	~Level1();

	DECLARE_LEVEL_CLASS(Level1)
	/**
	* @brief resetea el nivel y sus propiedades
	*/
	void reset();
	/**
	* @brief inicializa el nivel
	*/
	void init();
	/**
	* @brief llamado para empezar a jugar el nivel
	*/
	void start();
	/**
	* @brief Llamado cada ver que se se sale del nivel
	*/
	void exit();
	/**
	* @brief Se llama cada segundo y actualiza todo lo 
	* relativo al nivel
	* @param deltaTime tiempo transcurrido desde el frame anterior en ms
	*/
	void update(double deltaTime);
	/**
	* @brief Se llama cada segundo y actualiza todo lo 
	*/
	void passLevel();
	/**
	* @brief acceso a las LevelProperties del nivel
	* @return puntero a las LevelProperties del nivel
	*/
	LevelProperties* getLevelProperties(){return m_pLevelProperties;};
	bool checkWinningCondition(){ return m_pLevelProperties
		->checkWinningCondition();};

	
private:
	/**
	* @brief Llamado al inicializar el nivel y se encarga de cargar y 
	* crear la escena
	*/
	void createScene();

	GameManager* m_pGameMgr;

	Ogre::Camera* m_pCamera;
	Ogre::SceneManager* m_pSceneMgr;	
	PhysicsManager* m_pPhysicsMgr;
	//flag para indicar si se ha pasado el nivel
	bool m_bm_bLevelClear;
	//el fader para los efectos de fadein y fade out de las transiciones
	Fader* m_pFader;
};

/**
 *  @author Jorge Rosique Contreras
 *  @date 12-6-2014
 *
 * Clase que contiene las propiedades del nivel 
 * que representan la condicion que debe cumplirse 
 * para superar el nivel
 */
class Level1Properties : public LevelProperties{
public:
	Level1Properties();
	/**
	* @brief comprueba si se ha cumplido la condicion que indica que se ha superado el nivel
	* @return si se ha cumplido la condicion o no
	*/
	bool checkWinningCondition();
	/**
	* @brief incrementa el contadorde la condicion de victoria
	*/
	void increaseCounter(){m_counterCondition++;};
	/**
	* @brief resetea la condicion del nivel
	*/
	void reset();
};


#endif