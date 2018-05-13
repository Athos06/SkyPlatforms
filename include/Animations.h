#ifndef ANIMATIONS_H
#define ANIMATIONS_H

#include <Ogre.h>
/**
 *  @author Jorge Rosique Contreras
 *  @date 14-6-2014
 *
 *  Clase que se encarga de la gestion de las animaciones de nodo
 *	de los diferentes objetos del juego que las utilizan	
 */
class Animations{
public:
	/**
	@brief Llama a actualizar cada una de las animaciones activas
	@param deltaTime tiempo transcurrido desde el ultimo frame en ms
	*/
	static void updateEach(double deltaTime);
	/**
	@brief destruye todas las animaciones activas
	*/ 
	static void destroyAllAnimations();
	/**
	@brief Llama a resetear todas las animaciones activas
	*/ 
	static void resetEach();
	/**
	@brief llama a modificar la velocidad de todas las animaciones
	@param relativeSpeed cambio relativo de la velocidad respecto de la
	velocidad actual de la reproduccion, siendo 	el tanto por ciento del 
	cambio de la velocidad. Si pasamos 0.1 la velocidad
	aumentara un 10%, si pasamos -0.1 la velocidad disminuira un 10%, etc
	*/
	static void ChangeSpeedAll(float relativeSpeed);
	/**
	@brief constructor
	@param anim puntero a la Ogre::Animation
	@param animState puntero al AnimationState
	@param speed velocidad de la animacion
	@param startFrame frame de inicio de la animacion
	*/
	Animations(Ogre::Animation* anim, Ogre::AnimationState* animState, 
		float speed, int startFrame = 0);
	~Animations();
	
	
	
	/**
	@brief permite modificar la velocidad de reproduccion de la animacion
	@param speed velocidad de reproduccion de la animacion
	@param relativeSpeed cambio relativo de la velocidad respecto de la
	velocidad actual de la reproduccion. Si es distinta de cero el parametro
	speed es ignorado y se realiza el cambio en funcion a relativeSpeed, siendo
	el tanto por ciento del incremento la velocidad. Si pasamos 0.1 la velocidad
	aumentara un 10%, si pasamos -0.1 la velocidad disminuira un 10%, etc
	*/
	void changeSpeed(float speed, float relativeSpeed = 0);
	/**
	@brief Actualizar la animacion
	@param deltaTime tiempo transcurrido desde el ultimo frame en ms
	*/
	void update(double deltaTime);
	/**
	@brief resetea la animacion
	*/
	void reset();
	/**
	@brief se destruye animacion y todos sus componentes. Llama a delete
	*/
	void destroy();
	
private:
	static std::list<Animations*> m_AnimationsList; //la lista con todas las animaciones
	static std::vector<std::list<Animations*>::iterator> m_deletionList;
	
	std::list<Animations*>::iterator it; //un iterador para poder elimnar la animacion de la lista
						
	float m_animSpeed;
	int m_startFrame;

	Ogre::Animation* m_pAnim;
	Ogre::AnimationState* m_pAnimState;
};

#endif