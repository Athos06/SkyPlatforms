#ifndef HUD_H
#define HUD_H

#include <Ogre.h>
#include <CEGUI.h>

/**
 *  @author Jorge Rosique Contreras
 *  @date 11-6-2014
 *
 *  La clase HUD se encarga de gestionar todo lo relacionado
 *	con el head up Display dentro del juego asi como de la intro de
 *	los niveles
 */
class HUD : public Ogre::Singleton<HUD> {

public:
	static HUD& getSingleton();
	static HUD* getSingletonPtr();
	
	HUD();
	~HUD();
	/**
	* @brief Es llamado cada Frame y ejecuta el efecto de fade in y 
	* fade out cuando esta activo, asi como actualizar los indicadores 
	* del HUD dentro del juego
	* @param deltaTime tiempo transcurrido desde el ultimo frame en ms
	*/ 
	void update(double deltaTime);
	/**
	* @brief Es llamada al final de cada nivel para resetear todas las 
	* variables del HUD a su estado inicial
	*/ 
	void reset();
	/**
	* @brief Inicia el efecto de fade in (aparece gradualmente) del HUD
	*/ 
	void fadeIn();
	/**
	* @brief Inicia el efecto de fade out (desvanecimiento) del HUD
	*/ 
	void fadeOut();
	/**
	* @brief selecciona la visibilidad del HUD
	* @param show indica si se muestra o no el HUD
	*/ 
	void showHUD(bool show);	
	/**
	* @brief selecciona la visibilidad del mensaje de portal abierto
	* @param show indica si se muestra o no el mensaje de portal abierto
	*/ 
	void showOpenMsg(bool show);
	
	/**
	* @brief Muestra el mensaje de advertencia indicando que el portal
	*esta inactivo y que hay que recoger las skystones
	* @message no hace nada, se incluye por si hubiese necesidad de elegir
	* entre diferentes mensajes, pero solo hay uno actualmente asi que no
	* tiene uso
	*/ 
	void showMsg(Ogre::String message);
	/**
	* @brief oculta el mensaje de advertencia del portal
	*/ 
	void hideMsg();
	/**
	* @brief Indica que se muestre el HUD especifico del jefe final
	* Se llama al comienzo del nivel del jefe final y es un caso 
	* especial solo para este nivel
	*/ 
	void showBossHUD();
	/**
	* @brief Selecciona la GUI de la intro del nivel
	* @param level Indica el nivel actual
	*/ 
	void selectIntro(int level);
	/**
	* @brief Inicia el efecto de fade out (desvanecimiento) de la GUI
	* de Intro del nivel
	*/ 
	void fadeOutIntro();
	/**
	* @brief Inicia el efecto de fade in (aparece gradualmente) de la GUI
	* de Intro del nivel
	*/ 
	void fadeInIntro();

private:
	bool m_bIntroFadeIn;
	bool m_bIntroFadeOut;

	bool m_bFadeIn;
	bool m_bFadeOut;
	
	float m_IntroAlphaValue;
	float m_alphaValue;
	float m_fadeTime;

	CEGUI::Window*	m_pHUDsheet;
	
	CEGUI::Window * m_nLives;
	CEGUI::Window * m_nStones; 
	
	CEGUI::Window * m_innactiveMsg;
	CEGUI::Window * m_openMsg;

};

#endif