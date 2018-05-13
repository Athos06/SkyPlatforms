#include "LevelsManager.h"


template<> LevelsManager* Ogre::Singleton<LevelsManager>::msSingleton = 0;


LevelsManager& LevelsManager::getSingleton(){
	assert (msSingleton); 
	return (*msSingleton);
}

 
LevelsManager* LevelsManager::getSingletonPtr(){
	assert (msSingleton);
	return msSingleton;
}

LevelsManager::LevelsManager(){
	m_ActiveLevel	= 0;
}

LevelsManager::~LevelsManager(){
}

//solo se usa internamente para crear el nivel
void LevelsManager::manageLevels(Ogre::String levelName, Level *level){
	
	try
	{
		level_info new_level_info;
		new_level_info.name = levelName;
		new_level_info.level = level;
		m_Levels.push_back(new_level_info);
	}
	catch(std::exception& e)
	{
		delete level;
		throw Ogre::Exception(Ogre::Exception::ERR_INTERNAL_ERROR,
			"Error while trying to manage a new Level\n" + 
			Ogre::String(e.what()), "LevelsManager.cpp");
	}
}

void LevelsManager::reset(){
	m_ActiveLevel->reset();
}

void LevelsManager::update(double deltaTime){
	m_ActiveLevel->update(deltaTime);
}

void LevelsManager::startLevel(){
	m_ActiveLevel->start();
}
void LevelsManager::exit(){
	m_ActiveLevel->exit();
}

Level* LevelsManager::findByName(Ogre::String levelName){

	std::vector<level_info>::iterator itr;

	for(itr = m_Levels.begin(); itr != m_Levels.end(); itr++){
		if(itr->name == levelName)
			return itr->level;
	}
	return 0;
}

void LevelsManager::init(){
	if(m_ActiveLevel)
		m_ActiveLevel->init();
}

void LevelsManager::selectLevel(Level *level){
	m_ActiveLevel = level;
}

void LevelsManager::changeLevel(Level *level){
	//if(m_ActiveLevel)
	//	m_ActiveLevel->exit();

	m_ActiveLevel = level;
	//m_ActiveLevel->init();
}

void LevelsManager::completeLevel(){
	m_ActiveLevel->passLevel();
}

LevelProperties* LevelsManager::getLevelProperties(){
	return m_ActiveLevel->getLevelProperties();
}

bool LevelsManager::checkWinningCondition(){
	return m_ActiveLevel->getLevelProperties()->checkWinningCondition();
}