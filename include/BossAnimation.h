//||||||||||||||||||||||||||||||||||||||||||||||||

#ifndef BOSS_ANIMATION_H
#define BOSS_ANIMATION_H
//||||||||||||||||||||||||||||||||||||||||||||||||

#include <Ogre.h>


/**
 *  @author Jorge Rosique Contreras
 *  @date 13-6-2014
 *
 *  Clase encargada de las animaciones del boss
 */
class BossAnimation{
public:
	//diferentes animaciones que tiene el boss
	enum AnimID{
		ANIM_IDLE,
		ANIM_ATTACKING,
		ANIM_DIE,
		ANIM_NONE
	};

	BossAnimation(Ogre::Entity* entity);
	~BossAnimation();
	/**
	@brief inicializa las animaciones
	*/
	void setupAnimations();
	/**
	@brief establece la animacion a ejecutar a continuacion
	@param ANIM_ID identificador de la animacion a ejecutar
	*/
	void setAnimation(AnimID ANIM_ID);
	/**
	@brief resetea todas las animaciones y variables relativas al bossAnimation
	*/
	void reset();
	/**
	@brief actualiza la animacion
	@param deltaTime tiempo transcurrido desde el utlimo frame en ms
	*/
	void update(double deltaTime);
private:
	static const int NUM_ANIMATIONS = 3;
	static const int ANIM_FADE_SPEED = 5;
	//identificador animacion ejecutandose actualmente
	//AnimID m_activeAnimation;
	int activeAnimID;
	//puntero a la animacion actual
	Ogre::AnimationState* m_pActiveAnimation;
	//array con las animaciones que se ejecutaran
	Ogre::AnimationState *m_pAnimations[NUM_ANIMATIONS];
	Ogre::Entity* m_pEntity; //la entidad que tiene las animaciones
};
//||||||||||||||||||||||||||||||||||||||||||||||||

#endif