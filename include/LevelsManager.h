#ifndef LEVELSMANAGER_H
#define LEVELSMANAGER_H

#include "Level.h"

/**
 * @author Jorge Rosique Contreras
 * @date 13-6-2014
 *
 * Esta clase se encarga de gestionar los diferentes Niveles
 */
class LevelsManager : public LevelListener, 
	public Ogre::Singleton<LevelsManager>{
public:
	typedef struct{
		Ogre::String name;
		Level* level;
	}level_info;

	static LevelsManager& getSingleton();
	static LevelsManager* getSingletonPtr();

	LevelsManager();
	~LevelsManager();
	/**
	@brief llamada cada frame, llama a actualizar el nivel activo
	*/
	void update(double deltaTime);
	/**
	@brief llama a comenzar el nivel activo
	*/
	void startLevel();
	/**
	@brief solo es usada internamente para crear el nivel, el usuario no debe usarla
	*/
	void manageLevels(Ogre::String levelName, Level* level);
	/**
	@brief selecciona cual sera el siguiente nivel
	@param level el siguiente nivel
	*/
	void selectLevel(Level *level);
	/**
	@brief cambia de nivel
	@param level el nivel al que queremos cambiar
	*/
	void changeLevel(Level *level);
	/**
	@brief llama a passLevel() del nivel actual
	*/
	void completeLevel();
	/**
	@brief obtiene las LevelProperties del nivel actual
	@return puntero a las LevelProperties del nivel actual
	*/
	LevelProperties* getLevelProperties();
	/**
	@brief llama a checkWinningCondition del miembro LevelProperties del nivel actual
	@return si se cumple la winning condition o no
	*/
	bool checkWinningCondition();
	/**
	@brief permite obtener un puntero a un Level a partir de su nombre
	@return puntero al Level correspondiente
	*/
	Level* findByName(Ogre::String levelName);
	/**
	@brief llama reset() del nivel actual para resetearlo
	*/
	void reset();
	/**
	@brief llama a la funcion init() del nivel actual
	*/
	void init();
	/**
	@brief llama a la funcion exit() del nivel actual para salir del mismo
	*/
	void exit();

private:
	Level*		m_ActiveLevel;
	std::vector<level_info>		m_Levels;
};

#endif
