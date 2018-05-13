#ifndef EFFECT_H
#define EFFECT_H

#include <Ogre.h>
#include <list>
/**
 *  @author Jorge Rosique Contreras
 *  @date 14-6-2014
 *
 *  Clase de la que heredan los efectos tales como el fader
 */
class Effect {
public:
	/**
	@brief llamado cada frame, actualiza todos los efectos activos
	@param deltaTime tiempo transcurrido desde el ultimo frame en ms
	*/ 
	static void updateEach(double deltaTime);
	/**
	@brief destruye todos los efectos activos
	*/
	static void destroyAllEffects();

	Effect();
	virtual ~Effect();

	virtual void update(double deltaTime) = 0;

protected:
	//lista de efectos activos
	static std::list<Effect*> m_effectsList;
	//lista de efectos a eliminar
	static std::vector<std::list<Effect*>::iterator> m_deletionList;

	std::list<Effect*>::iterator it; //para eliminarlo facilmente
};

#endif