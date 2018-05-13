#include "SavegameLoader.h"
#include "AdvancedOgreFramework.h"
#include "GameManager.h"
#include <stdio.h>
#include <string.h>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

const char *FILENAME = "savegame";

SavegameLoader::SavegameLoader(){
	m_groupName = "";
}

SavegameLoader::~SavegameLoader(){

}

void SavegameLoader::write (){
	char buffer[10];
	sprintf (buffer, "%d\n", GameConfig::getSingletonPtr()->levelsLock[0]);
	
	ofstream os(FILENAME);
	  
	if (!os.is_open ()) {
		Ogre::LogManager::getSingleton().logMessage(
			"[SavegameLoader] Error leyendo savegame" );
		return;
	}
	  
	else{
		//escribe en savegame el estado de los niveles, si estan bloqueados o no	
		sprintf (buffer, "%d\n", GameConfig::getSingletonPtr()->levelsLock[0]);
		string str(buffer);
		os << str;//os << GameConfig::getSingletonPtr()->levelsLock[0];
		sprintf (buffer, "%d\n", GameConfig::getSingletonPtr()->levelsLock[1]);
		str = buffer; //string str2(buffer);
		os << str;
		sprintf (buffer, "%d\n", GameConfig::getSingletonPtr()->levelsLock[2]);
		str = buffer; //string str3(buffer);
		os << str;
		sprintf (buffer, "%d\n", GameConfig::getSingletonPtr()->levelsLock[3]);
		str = buffer; //string str3(buffer);
		os << str;
	}

	os.close ();

}


void SavegameLoader::read(){ 
	ifstream is (FILENAME);
	string str = "";
	  
	if (!is.is_open ()){
		 Ogre::LogManager::getSingleton().logMessage(
			"[SavegameLoader] Error guardando savegame" );
		return;
	}
	else{
		int i = 0;
		//obtiene si los niveles estan desbloqueados
		while (getline (is, str) && i<4){  
			if(str == "0"){
				GameConfig::getSingletonPtr()->levelsLock[i] = false;
			}
			else{
				GameConfig::getSingletonPtr()->levelsLock[i] = true;
			}
			i++;

		}
		is.close ();
	}
}

