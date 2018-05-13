#ifndef ANIMATION_LOADER_H
#define ANIMATION_LOADER_H

#include <Ogre.h>
#include "rapidxml.h"

class Animations;
/**
 *  @author Jorge Rosique Contreras
 *  @date 14-6-2014
 *
 *  Clase encargada de la importacion de los ficheros de las animaciones
 *	de nodos. Utiliza el mismo sistema de importacion que el DotSceneLoader
 *  aunque con modificaciones especificas para ajustarlo a mis necesidades.
 * revisar DotSceneLoader de ogre para mas informacion
 */
class AnimationLoader{
public:
	AnimationLoader();
	~AnimationLoader();
	/**
	@brief Llamado para importar el fichero .anim (xml) con la animacion
	@param animationName el nombre de la animacion
	@param groupName el nombre del grupo de recursos de ogre
	@param node el nodo que recibe la animacion
	@param sceneMgr el scene manager actual
	@param invert si invertimos la animacion (por defecto false)
	@param speed la velocidad de la animacion (por defecto 1)
	@param startFrame el frame de inicio de la animacion (por defecto comienza desde 0)
	@return puntero a la animacion creada por el animationLoader por si se quiere
	guardar para uso futuro
	*/
	Animations* parseAnimation(const Ogre::String &animationName, const Ogre::String &groupName, 
		Ogre::SceneNode* node, Ogre::SceneManager* sceneMgr,
		bool invert = false, float speed = 1, int startFrame = 0);
	Ogre::String getProperty(const Ogre::String &ndNm, const Ogre::String &prop);
	
	Ogre::String getAttrib(rapidxml::xml_node<>* XMLNode, const Ogre::String &parameter, const Ogre::String &defaultValue = "");
    Ogre::Real getAttribReal(rapidxml::xml_node<>* XMLNode, const Ogre::String &parameter, Ogre::Real defaultValue = 0);
    bool getAttribBool(rapidxml::xml_node<>* XMLNode, const Ogre::String &parameter, bool defaultValue = false);

	Ogre::Vector3 parseVector3(rapidxml::xml_node<>* XMLNode);
    Ogre::Quaternion parseQuaternion(rapidxml::xml_node<>* XMLNode);
	
	/**
	@brief se encarga de llamar a processKeyFrames para 
	crear la animacion
	*/
	void processAnimation(rapidxml::xml_node<>* XMLRoot);
	/**
	@brief se encarga de crear la animacion establece los parametros que esta
	usara. Llama a processKeyFrame para inicializar la animacion de ogre
	*/
	void processKeyframes(rapidxml::xml_node<>* XMLNode);
	/**
	@brief crea e inicializa los diferentes keyFrame que forman
	la animacion
	*/
	void processKeyframe(rapidxml::xml_node<>* XMLNode);
private:
	Ogre::String m_groupName;
	Ogre::String m_animationName;
	
	bool m_bInvert;
	int m_startFrame;
	int m_interpolationMode;	
	int m_animLength;
	float m_animSpeed;
	
	Ogre::SceneNode * m_pNode;
	Ogre::SceneManager* m_pSceneMgr;
	Ogre::Animation* m_anim;
	Ogre::NodeAnimationTrack* m_track;
	Ogre::AnimationState* m_animState;

	Animations* m_pAnimation;

};

#endif