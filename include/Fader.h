#ifndef FADER_H
#define FADER_H

#include "Effect.h"
#include <Ogre.h>

namespace Ogre {
     class TextureUnitState;
     class Overlay;
 }


class FaderCallback{
public:
	virtual void fadeInCallback(void){}
	virtual void fadeOutCallback(void){}
};
 
/**
 *  @author Jorge Rosique Contreras
 *  @date 14-6-2014
 *
 *  Clase encargada de gestionar el efecto de fade in y fade out
 * basada en http://www.ogre3d.org/tikiwiki/tiki-index.php?page=FadeEffectOverlay
 */
class Fader : public Effect, public Ogre::Singleton<Fader> {
public:	
	static Fader& getSingleton();
	static Fader* getSingletonPtr();

	Fader(){};
	Fader(const char *OverlayName,
		const char *MaterialName, FaderCallback *instance = 0);
	~Fader();

	 void startFadeIn(double duration = 1.0f);
     void startFadeOut(double duration = 1.0f);

	void update(double deltaTime);
private:
	 double _alpha;
     double _current_dur;
     double _total_dur;
     FaderCallback *_inst;
     Ogre::TextureUnitState *_tex_unit;
     Ogre::Overlay *_overlay;
 
     enum _fadeop {
         FADE_NONE,
         FADE_IN,
         FADE_OUT,
     } _fadeop;
};

#endif
