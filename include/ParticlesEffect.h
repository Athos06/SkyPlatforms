#ifndef PARTICLES_MGR_H
#define PARTICLES_MGR_H

#include <Ogre.h>
#include <vector>
#include <list>
#include "GameObject.h"

/**
 *  @author Jorge Rosique Contreras
 *  @date 14-6-2014
 *
 *  clase encargada de la gestion de los efectos de particulas
 */
class ParticlesEffect : public GameObject{
public:
	/**
	@brief llamado cada frame, actualiza todos los efectos de particulas activos
	@param deltaTime tiempo transcurrido desde el ultimo frame en ms
	*/ 
	static void updateEach(double deltaTime);
	/**
	@brief destruye todos los efectos de particulas activos
	*/
	static void destroyAllParticles();
	/**
	@brief constructor
	@param lifeTime tiempo de vida del efecto de particulas
	@param templateName nombre de la plantilla de las particulas
	@position posicion donde se crea el efecto de particulas
	*/
	ParticlesEffect(float lifeTime, Ogre::String templateName, 
		Ogre::Vector3 position);
	~ParticlesEffect();
	/**
	@brief actualiza el efecto de particulas
	@param deltaTime tiempo transcurrido desde el ultimo frame en ms
	*/
	void update(double deltaTime);
	/**
	@brief se destruye el efecto de particulas. Tambien elimina el efecto de particulas
	llamanado a delete
	*/
	void destroy();
	
private:
	static int m_NumInstanced; //para crear nombres unicos sin conflictos
	static std::list<ParticlesEffect*> m_particlesEffectsList;
	static std::vector<std::list<ParticlesEffect*>::iterator> 
		m_deletionList;
	std::list<ParticlesEffect*>::iterator it;

	float m_AliveTime;
	float m_LifeTime;
	Ogre::ParticleSystem* m_pParticle;
	
	Ogre::SceneManager* m_pSceneMgr;
	
};

#endif