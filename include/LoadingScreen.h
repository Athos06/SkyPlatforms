#ifndef LOADING_SCREEN_H
#define LOADING_SCREEN_H

#include <Ogre.h>
#include <CEGUI.h>

/**
 *  @author Jorge Rosique Contreras
 *  @date 11-6-2014
 *
 *  se encarga de gestionar y actualizar la pantalla de carga
 */
class LoadingScreen : public Ogre::Singleton<LoadingScreen>{
public:
	static LoadingScreen& getSingleton();
	static LoadingScreen* getSingletonPtr();

	LoadingScreen();
	~LoadingScreen();
	/**
	@brief establece el tanto por ciento de carga a mostrar
	@param indicar del tanto por ciento de carga
	*/
	void setCompletePercent(int total);

private:
	CEGUI::Window*	m_pLoadingSheet;
	//puntero a la window que contine el texto con el tanto por ciento de carga
	CEGUI::Window * m_pLoadingText;
};

#endif 