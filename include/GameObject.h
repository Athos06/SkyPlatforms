//|||||||||||||||||||||||||||||||||||||||||||||||
#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H
//|||||||||||||||||||||||||||||||||||||||||||||||

#include <Ogre.h>
//|||||||||||||||||||||||||||||||||||||||||||||||

/**
 *  @author Jorge Rosique Contreras
 *  @date 14-6-2014
 *
 *  Clase base de todos los objetos del juego
 *	utilizada especialmente como userPointer en los rigidbody
 *	de bullet y asi poder comprobar colisiones sabiendo que tipo
 *	de objetos estan implicados
 */
class GameObject{
public:
	/**
	@brief obtiene el tag asociado a este objeto
	@return el tag asociado al objeto
	*/ 
	virtual Ogre::String getTag(){return tag;};
	Ogre::SceneNode* m_pNode;
	/** 
	@brief establece el tag de este objeto
	@param goTag el tag que queremos asociar al objeto
	*/ 
	void setTag(Ogre::String goTag){tag = goTag;};

protected:
	//tag del gameObject, utilizada en las comprobaciones de
	//las colisiones
	Ogre::String tag;
};
//|||||||||||||||||||||||||||||||||||||||||||||||

#endif
//|||||||||||||||||||||||||||||||||||||||||||||||