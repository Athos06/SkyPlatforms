#include "AudioManager.h"

template <> AudioManager* Ogre::Singleton<AudioManager>::msSingleton = 0;

AudioManager& AudioManager::getSingleton(){
	assert (msSingleton);
	return (*msSingleton);
}

AudioManager* AudioManager::getSingletonPtr(){
	assert (msSingleton);
	return msSingleton;
}

AudioManager::AudioManager(){
	m_pTrackMgr = 0;
	m_pSoundFXMgr = 0;	
}

AudioManager::~AudioManager(){
}

bool AudioManager::initAudio()
{
	if(!initSDL())
		return false;
	loadSounds();
	return true;
}

bool AudioManager::initSDL ()
{


  if (SDL_Init (SDL_INIT_AUDIO) < 0)
    return false;
  // Llamar a  SDL_Quit al terminar.
  atexit (SDL_Quit);

  // Inicializando SDL mixer...
  if (Mix_OpenAudio
      (MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS,
       4096) < 0)
    return false;

  m_pTrackMgr = new TrackManager();
  m_pSoundFXMgr = new SoundFXManager();

  // Llamar a Mix_CloseAudio al terminar.
  atexit (Mix_CloseAudio);


  return true;

}

void AudioManager::loadSounds(){
	//cargamos y anadimos los sonidos a la coleccion
	m_SoundsFXmap.insert(std::make_pair("PickedStone", m_pSoundFXMgr->load 
		("PickedStone.ogg") ) );
	m_SoundsFXmap.insert(std::make_pair("PortalActivated", m_pSoundFXMgr->load 
		("PortalActivated.ogg") ) );
	m_SoundsFXmap.insert(std::make_pair("Jump", m_pSoundFXMgr->load 
		("Jump.ogg") ) );
	m_SoundsFXmap.insert(std::make_pair("getHit", m_pSoundFXMgr->load 
		("Hit.ogg") ) );
	m_SoundsFXmap.insert(std::make_pair("Step", m_pSoundFXMgr->load 
		("Step.ogg") ) );
	m_SoundsFXmap.insert(std::make_pair("EnterPortal", m_pSoundFXMgr->load 
		("EnterPortal.ogg") ) );
	m_SoundsFXmap.insert(std::make_pair("SkipSplash", m_pSoundFXMgr->load 
		("Enter.ogg") ) );
	m_SoundsFXmap.insert(std::make_pair("Teleporting", m_pSoundFXMgr->load 
		("Teleporting.ogg") ) );
	m_SoundsFXmap.insert(std::make_pair("ProyectileExplosion", m_pSoundFXMgr->load 
		("ProyectileExplosion.ogg") ) );
	m_SoundsFXmap.insert(std::make_pair("BigExplosion", m_pSoundFXMgr->load 
		("FinalExplosion.ogg") ) );
	m_SoundsFXmap.insert(std::make_pair("BossShoot", m_pSoundFXMgr->load 
		("BossShoot.ogg") ) );
	m_SoundsFXmap.insert(std::make_pair("Hover", m_pSoundFXMgr->load 
		("Hover.ogg") ) );
	m_SoundsFXmap.insert(std::make_pair("Clicked", m_pSoundFXMgr->load 
		("clicked.ogg") ) );
}


void AudioManager::unloadMainTrack()
{
	m_pMainTrack->unload ();
  
}

void AudioManager::preLoadTrack(Ogre::String sound){
	m_pTrackMgr->load(sound);
}

void AudioManager:: playMainTrack(Ogre::String sound, int loop)
{
	m_pMainTrack = m_pTrackMgr->load(sound);
		m_pMainTrack->play(loop);
  
}

void AudioManager::fadeInTrack(Ogre::String sound, int time, int loop){
	m_pMainTrack = m_pTrackMgr->load(sound);
	m_pMainTrack->fadeIn(loop, time*1000 ); //se le pasa el tiempo en ms
}

void AudioManager::fadeOutTrack(int time){
	m_pMainTrack->fadeOut(time*1000); //se le pasa en ms
}

void AudioManager::stopFX(Ogre::String fxName){
	//si no se encuentra el sonido no hacemos nada
	std::map<Ogre::String, SoundFXPtr>::iterator it;
	it = 	m_SoundsFXmap.find(fxName);
	if(it == m_SoundsFXmap.end()) {
	} 
	else {
	  SoundFXPtr sound = it->second;
		sound->stop(); 
	}
}


void
AudioManager::playFX(Ogre::String fxName, int loop, int channel)
{

	//si no se encuentra el sonido no hacemos nada
	std::map<Ogre::String, SoundFXPtr>::iterator it;
	it = 	m_SoundsFXmap.find(fxName);
	if(it == m_SoundsFXmap.end()) {
	} 
	else {
	  SoundFXPtr sound = it->second;
		sound->play(loop, channel); 
	}

}

void AudioManager::musicVolume(int volume){
	//yo le paso un porcentaje de 0 a 100 para que sea mas
	//comprensible, pero sdl usa unos valores de 0 a 128, por
	//lo que hago una regla de 3 basica
	int volumeChange = volume * MIX_MAX_VOLUME / 100;
	m_pMainTrack->volume(volumeChange);
}

void AudioManager::sfxVolume(int volume){
	int volumeChange = volume * MIX_MAX_VOLUME / 100;
	//indico que ese volumen es para todos los canales de efectos
	Mix_Volume(-1, volumeChange);
}