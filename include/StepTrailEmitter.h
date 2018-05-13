#ifndef STEP_TRAIL_EMITTER_H
#define STEP_TRAIL_EMITTER_H

#include <Ogre.h>

/**
 *  @author Jorge Rosique Contreras
 *  @date 14-6-2014
 *
 * Clase encargada de gestionar el efecto
 * de esa especia de polvo que se levanta cuando el personaje
 * comienza a andar, salta, o atteriza
 */
class StepTrailEmitter{
public:
	/**
	@brief constructor
	@param node nodo asociado a la posicion donde se crea el efecto
	*/
	StepTrailEmitter(Ogre::SceneNode* node);
	~StepTrailEmitter();
	/**
	@brief reproducimos el efecto
	@param continuous si true el efecto se reproduce infinitamente hasta
	que manualmente indicamos que pare con stop
	@param duration si no es continuous el efecto se reproduce durante el tiempo pasado
	*/
	void start(bool continuous, float duration);
	/**
	@brief dejamos de reproducir el efecto de particulas
	*/
	void stop();
	/**
	@brief llamado cada frame y se encarga de actualizar la logica del efecto
	y comprobar si ha pasado el tiempo de duracion del efecto para pararlo
	*/
	void update(double deltaTime);

private:
	//variables  de control de la duracion
	float m_timer;
	float m_duration;
	//si se reproduce infinitamente
	bool m_bContinuous;
	//si esta activo
	bool m_bActive;
	//efecto de particulas
	Ogre::SceneNode* m_pNode;
	Ogre::ParticleSystem* m_pParticle;

};

#endif