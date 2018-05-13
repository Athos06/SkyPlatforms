//||||||||||||||||||||||||||||||||||||||||||||||||

#ifndef CHARACTER_ANIMATION_H
#define CHARACTER_ANIMATION_H
//||||||||||||||||||||||||||||||||||||||||||||||||

#include <Ogre.h>

/**
 *  @author Jorge Rosique Contreras
 *  @date 13-6-2014
 *
 *  Clase encargada de las animaciones del personaje
 */
class CharacterAnimation{
public:
	//diferentes animaciones que tiene el Player
	enum AnimID{
		ANIM_IDLE,
		ANIM_RUNNING,
		ANIM_JUMPING,
		ANIM_FALLING,
		ANIM_DIE1,
		ANIM_RESTGAMECOMPLETE,
		ANIM_NONE
	};
	/**
	@brief constructor
	@param entity la entidad del jugador con las animaciones
	*/
	CharacterAnimation(Ogre::Entity* entity);
	~CharacterAnimation();
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
	brief resetea todas las animaciones y variables relativas al CharacterAnimation
	*/
	void reset();
	/**
	@brief actualiza la animacion
	@param deltaTime tiempo transcurrido desde el utlimo frame en ms
	*/
	void update(double deltaTime);
private:
	static const int NUM_ANIMATIONS = 6;
	static const int ANIM_FADE_SPEED = 5;
	//identificador de la animacion ejecutandose actualmente
	//AnimID m_activeAnimation;
	int activeAnimID;
	Ogre::AnimationState* m_pActiveAnimation;
	//array con las animaciones que se ejecutaran
	Ogre::AnimationState *m_pAnimations[NUM_ANIMATIONS];
	Ogre::Entity* m_pEntity; //la entidad que tiene las animaciones


};
//||||||||||||||||||||||||||||||||||||||||||||||||

#endif