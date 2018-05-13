#include <Ogre.h>
/**
 *  @author Jorge Rosique Contreras
 *  @date 1-7-2014
 *
 *  Factoria encargada de la creacion de trampas
 */
class Trap;

class TrapFactory{
public:
	TrapFactory();
	~TrapFactory();
	/**
	@brief crea una trampa del tipo DeathMill
	@param nodo de la trampa
	@return puntero a la trampa recien creada
	*/
	Trap* createDeathMill(Ogre::SceneNode* node);
	/**
	@brief crea una trampa del tipo Mahser
	@param nodo de la trampa
	@return puntero a la trampa recien creada
	*/
	Trap* createMasher(Ogre::SceneNode* node);
	/**
	@brief crea una trampa del tipo Slasher
	@param nodo de la trampa
	@return puntero a la trampa recien creada
	*/
	Trap* createSlasher(Ogre::SceneNode* node);
	/**
	@brief crea una trampa del tipo Spikes
	@param nodo de la trampa
	@return puntero a la trampa recien creada
	*/
	Trap* createSpikes(Ogre::SceneNode* node);
};