#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

#include <Ogre.h>

#include "SoundFXManager.h"
#include "TrackManager.h"
#include <map>

class AudioManager : public Ogre::Singleton<AudioManager> {
public:
	static AudioManager& getSingleton();
	static AudioManager* getSingletonPtr();

	AudioManager();
	~AudioManager();	
	
	/**
	* @brief iniciliza todo lo relacionado al audio.
	* Llama a loadSounds() para cargar los sonidos y a initSDL()
	*/
	bool initAudio();
	/**
	* @brief inicializa SDL
	*/
	bool initSDL();
	/**
	* @brief precarga los sonidos del juego
	*/
	void loadSounds();
	/**
	* @brief se explica por si mismo
	*/
	void unloadMainTrack();
	/**
	* @brief precarga del track de audio para que no se produzca un paron
	la primera vez de carga
	* @param name Nombre del track a cargar
	*/
	void preLoadTrack(Ogre::String name);
	/**
	* @brief reproduce como track principal el track con el nombre elegido
	* @param name Nombre del track
	* @param loop indica si se reproduce en loop y el numero de veces.
	* 0 solo se reproduce una vez, -1 se reproduce infinitamente en loop
	*/
	void playMainTrack(Ogre::String name, int loop = -1);
	/**
	* @brief reproduce un sonido
	* @param name nombre del sonido a reproducir
	* @param loop indica si se reproduce en loop y el numero de veces.
	* 0 solo se reproduce una vez, -1 se reproduce infinitamente en loop
	* @param channel indica el canal en el que se reproduce el sonido
	*/
	void playFX(Ogre::String name, int loop = 0, int channel = 0);
	/**
	* @brief deja de reproducir el sonido seleccionado
	* @param  name indica si se reproduque queremos parar
	*/
	void stopFX(Ogre::String name);
	/**
	* @brief llama a reproducir el track principal con efecto de fade in
	* @param name nombre del sonido a reproducir
	* @param time tiempo que dura el fade in
	* @param loop indica si se reproduce en loop y el numero de veces.
	* 0 solo se reproduce una vez, -1 se reproduce infinitamente en loop
	*/
	void fadeInTrack(Ogre::String name, int time, int loop = -1);
	/**
	@brief ejecuta efecto de fade out del main track
	@param time tiempo de duracion del efecto
	*/ 
	void fadeOutTrack(int time);
	/**
	@brief modifica el volumen de reproduccion de la musica
	@param volume volumen (limitado de 0 a 100)
	*/ 
	void musicVolume(int volume);
	/**
	@brief modifica el volumen de reproduccion de los efectos de sonido
	@param volume volumen (limitado de 0 a 100)
	*/ 
	void sfxVolume(int volume);

	//puntero al track principal reproduciendose
	TrackPtr m_pMainTrack;
private:
	TrackManager* m_pTrackMgr;
	SoundFXManager* m_pSoundFXMgr;

	//mapa de los diferentes efectos de sonido. Utiliza un nombre como
	//clave de acceso
	std::map<std::string, SoundFXPtr> m_SoundsFXmap;
	
	
};



#endif