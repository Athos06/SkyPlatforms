#include "Fader.h"

template <> Fader* Ogre::Singleton<Fader>::msSingleton = 0;

Fader& Fader::getSingleton(){
	assert (msSingleton);
	return (*msSingleton);
}

Fader* Fader::getSingletonPtr(){
	assert (msSingleton);
	return msSingleton;
}

Fader::Fader(const char *OverlayName, const char *MaterialName, FaderCallback *instance){
     _fadeop = FADE_NONE;
     _alpha = 0.0;
     _inst = instance;

     // Get the material by name
     Ogre::ResourcePtr resptr = Ogre::MaterialManager::getSingleton().getByName(MaterialName);
     Ogre::Material * mat = dynamic_cast<Ogre::Material*>(resptr.getPointer());

     Ogre::Technique *tech = mat->getTechnique(0);    // Get the technique
     Ogre::Pass *pass = tech->getPass(0);            // Get the pass
     _tex_unit = pass->getTextureUnitState(0);        // Get the texture_unit state

     // Get the _overlay
     _overlay = Ogre::OverlayManager::getSingleton().getByName(OverlayName);
     _overlay->hide();
	
	 m_effectsList.push_front(this);
	//y guardamos el iterador al elemento que acabamos de crear
	it = m_effectsList.begin();
}

Fader::~Fader(){
	//destruir la overlay en el destructor...mala idea
	_overlay->hide();
	//m_effectsList.erase(it);
	
}


 void Fader::startFadeIn(double duration )
 {
     if( duration < 0 )
         duration = -duration;
     if( duration < 0.000001 )
         duration = 1.0;
	//lo paso a milisegundos que es la misma unidad
	//en la que funciona el timer del juego
	duration *= 1000;
	_alpha = 1.0;
	_total_dur = duration;
	_current_dur = duration;
	_fadeop = FADE_IN;
	_tex_unit->setAlphaOperation(Ogre::LBX_MODULATE, Ogre::LBS_MANUAL, Ogre::LBS_TEXTURE, _alpha);
	_overlay->show();
 }
 
 void Fader::startFadeOut(double duration )
 {
     if( duration < 0 )
         duration = -duration;
     if( duration < 0.000001 )
         duration = 1.0;
 
	 duration *= 1000;
     _alpha = 0.0;
     _total_dur = duration;
     _current_dur = 0.0;
     _fadeop = FADE_OUT;

	 _tex_unit->setAlphaOperation(Ogre::LBX_MODULATE, Ogre::LBS_MANUAL, Ogre::LBS_TEXTURE, _alpha);
     _overlay->show();
 }

void Fader::update(double deltaTime){
	if( _fadeop != FADE_NONE && _tex_unit ){
		// Set the _alpha value of the _overlay
		_tex_unit->setAlphaOperation(Ogre::LBX_MODULATE, Ogre::LBS_MANUAL, Ogre::LBS_TEXTURE, _alpha);    // Change the _alpha operation

		// If fading in, decrease the _alpha until it reaches 0.0
		if( _fadeop == FADE_IN ){
			_current_dur -= deltaTime;
			_alpha = _current_dur / _total_dur;
			if( _alpha < 0.0 ){
			 _overlay->hide();
			 _fadeop = FADE_NONE;
			 if( _inst )
				 _inst->fadeInCallback();
			}
		}
		// If fading out, increase the _alpha until it reaches 1.0
		else if( _fadeop == FADE_OUT ){
			_current_dur += deltaTime;
			_alpha = _current_dur / _total_dur;
			if( _alpha > 1.0 ){
				_fadeop = FADE_NONE;
				if( _inst )
				_inst->fadeOutCallback();
			}
		}
	}
}
	
