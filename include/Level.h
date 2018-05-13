#ifndef LEVEL_H
#define LEVEL_H

#include <Ogre.h>

class Level;
class LevelProperties;


/**
 *  @author Jorge Rosique Contreras
 *  @date 12-6-2014
 *
 * Clase virtual de la que heredara LevelsManager
 */
class LevelListener{
public:
	LevelListener(){};
	virtual ~LevelListener(){};

	virtual void manageLevels(Ogre::String levelName,Level* level) = 0;
	virtual Level* findByName(Ogre::String levelName) = 0;
	virtual void update(double deltaTime) = 0;
	virtual void selectLevel(Level* level) = 0;

};


/**
 *  @author Jorge Rosique Contreras
 *  @date 12-6-2014
 *
 * Clase virtual de la que heredan todos los niveles
 */
class Level
{
public:
	static void create(LevelListener* parent, const Ogre::String name){};
	void destroy(){delete this;}
	virtual void reset() = 0;
	virtual void init() = 0;
	virtual void start() = 0;
	virtual void exit()	= 0;
	virtual void update(double deltaTime) = 0;
	virtual void passLevel() = 0;
	
	virtual LevelProperties* getLevelProperties() = 0;

protected:
	Level(){};
	
	LevelProperties* m_pLevelProperties;
	LevelListener* m_pParent;

};



/**
 *  @author Jorge Rosique Contreras
 *  @date 12-6-2014
 *
 * Clase virtual de la que heredan las propiedades de todos los niveles
 */
class LevelProperties{
public:
	virtual bool checkWinningCondition() = 0;
	virtual void increaseCounter() = 0;
	virtual void reset() = 0;
protected:
	LevelProperties(){};

	bool m_bWin;	
	int m_counterCondition;
	int m_conditionValue;
};

//|||||||||||||||||||||||||||||||||||||||||||||||

#define DECLARE_LEVEL_CLASS(T)										\
static void create(LevelListener* parent, const Ogre::String name)	\
{																	\
	T* myLevel = new T();											\
	myLevel->m_pParent = parent;									\
	parent->manageLevels(name, myLevel);							\
}																	\
	
//|||||||||||||||||||||||||||||||||||||||||||||||


#endif






