#include "AnimationLoader.h"
#include "Animations.h"
#include "Constants.h"

AnimationLoader::AnimationLoader(){
	m_pSceneMgr = 0;
	m_groupName = "";
	m_interpolationMode = 0;
	m_animLength = 0;
	m_anim = 0;
	m_track = 0;
	m_bInvert = false;
	m_animSpeed = 1;
	m_pAnimation = 0;
}

AnimationLoader::~AnimationLoader(){
	m_pAnimation = 0;
}

Animations* AnimationLoader::parseAnimation(const Ogre::String &animationName, 
								const Ogre::String &groupName, 
								Ogre::SceneNode* node, Ogre::SceneManager* sceneMgr,
								bool invert, float speed, int startFrame){

	m_pSceneMgr = sceneMgr;
	m_groupName = groupName;
	m_pNode = node;

	m_bInvert = invert;
	m_animSpeed = speed;

	m_startFrame = startFrame;

	rapidxml::xml_document<> XMLDoc;
	rapidxml::xml_node<>* XMLRoot;

    Ogre::DataStreamPtr stream = 
		Ogre::ResourceGroupManager::getSingleton().openResource(animationName, groupName );
    
	char* animation = strdup(stream->getAsString().c_str());

    XMLDoc.parse<0>(animation);

	//grab the aniation node
    XMLRoot = XMLDoc.first_node("animation");

    // Validate the File
    if( getAttrib(XMLRoot, "formatVersion", "") == "")
    {
        Ogre::LogManager::getSingleton().logMessage(
			"[AnimationLoader] Error: Invalid .anim File. Missing <animation>" );
        return 0;
    }

	m_animationName = getAttrib(XMLRoot, "name") + "_" + node->getName() ;

	// Process the animation
    processAnimation(XMLRoot);

	return m_pAnimation;
}

void AnimationLoader::processAnimation(rapidxml::xml_node<>* XMLRoot){
	Ogre::String version = getAttrib(XMLRoot, "formatVersion", "unknown");
	Ogre::String message = "[AnimationLoader] Parsing anim file with version " + version;

	Ogre::LogManager::getSingleton().logMessage(message);

	rapidxml::xml_node<>* pElement;

	//process keyframes
	pElement = XMLRoot->first_node("keyframes");
	if(pElement)
		processKeyframes(pElement);
}

void AnimationLoader::processKeyframes(rapidxml::xml_node<>* XMLNode){
	//vemos cuanto dura la animacion
	m_animLength = getAttribReal(XMLNode, "length");
	
	//creamos la animacion
	m_anim = m_pSceneMgr->createAnimation(m_animationName, m_animLength);
	//obtenemos el modo de interpolacion
	Ogre::String interpolationMode = getAttrib(XMLNode, "interpolation");
	if(interpolationMode == "linear"){
		m_anim->setInterpolationMode(Ogre::Animation::IM_LINEAR);
		
	}
	if(interpolationMode == "spline"){
		m_anim->setInterpolationMode(Ogre::Animation::IM_SPLINE);
	}
	
	//obtenemos la velocidad de la animacion 
	//si hemos modificado la velocidad de la animacion individualmente 
	//para este nodo ignoramos la velocidad general de la animacion
	if(m_animSpeed == 1)
		m_animSpeed = getAttribReal(XMLNode, "speed", 1);

	m_track = m_anim->createNodeTrack(0, m_pNode);
	//muy importante llamar a setInitialState para que la animacion se respecto
	//a este estado inicial
	m_pNode->setInitialState();

	rapidxml::xml_node<>* pElement;
	
	//process keyframe
	pElement = XMLNode->first_node("keyframe");
	while(pElement){
		processKeyframe(pElement);
		pElement = pElement->next_sibling("keyframe");
	}

	m_animState = m_pSceneMgr->createAnimationState(m_animationName);
	m_animState->setEnabled(true);
	//creo la animacion
	m_pAnimation = new Animations(m_anim, m_animState, m_animSpeed, m_startFrame);

}

void AnimationLoader::processKeyframe(rapidxml::xml_node<>* XMLNode){
	rapidxml::xml_node<>* pElement;

	Ogre::Vector3 position = Ogre::Vector3::ZERO;
	Ogre::Quaternion rotation = Ogre::Quaternion::IDENTITY;

	int index = getAttribReal(XMLNode, "index");
	//process position
	pElement = XMLNode->first_node("position");
	if(pElement){
		position = parseVector3(pElement);	
		
		//si hemos indicado que se invierta invertimos la direccion
		if(m_bInvert)
			position = position*(-1);
		//esto permite que se mueva respecto a espacio local por la rotacion
		position = m_pNode->_getDerivedOrientation() * position;

	}
	//process rotation
	pElement = XMLNode->first_node("rotation");
	if(pElement){
		rotation = parseQuaternion(pElement);	
		//si hemos indicado que se invierta invertimos la rotacion
		if(m_bInvert)
			rotation = rotation.Inverse();

	}

	Ogre::TransformKeyFrame* key;
	//creamos el keyframe 
	key = m_track->createNodeKeyFrame(index);
	key->setTranslate(position);
	key->setRotation(rotation);


}



Ogre::String AnimationLoader::getAttrib(rapidxml::xml_node<>* XMLNode, const Ogre::String &attrib, const Ogre::String &defaultValue)
{
    if(XMLNode->first_attribute(attrib.c_str()))
        return XMLNode->first_attribute(attrib.c_str())->value();
    else
        return defaultValue;
}

Ogre::Real AnimationLoader::getAttribReal(rapidxml::xml_node<>* XMLNode, const Ogre::String &attrib, Ogre::Real defaultValue)
{
    if(XMLNode->first_attribute(attrib.c_str()))
        return Ogre::StringConverter::parseReal(XMLNode->first_attribute(attrib.c_str())->value());
    else
        return defaultValue;
}

bool AnimationLoader::getAttribBool(rapidxml::xml_node<>* XMLNode, const Ogre::String &attrib, bool defaultValue)
{
    if(!XMLNode->first_attribute(attrib.c_str()))
        return defaultValue;

    if(Ogre::String(XMLNode->first_attribute(attrib.c_str())->value()) == "true")
        return true;

    return false;
}

Ogre::Vector3 AnimationLoader::parseVector3(rapidxml::xml_node<>* XMLNode)
{
    return Ogre::Vector3(
        Ogre::StringConverter::parseReal(XMLNode->first_attribute("x")->value()),
        Ogre::StringConverter::parseReal(XMLNode->first_attribute("y")->value()),
        Ogre::StringConverter::parseReal(XMLNode->first_attribute("z")->value())
        );
}

Ogre::Quaternion AnimationLoader::parseQuaternion(rapidxml::xml_node<>* XMLNode)
{
    //! @todo Fix this crap!
	//pues espera sentado

    Ogre::Quaternion orientation;

    if(XMLNode->first_attribute("qx"))
    {
        orientation.x = Ogre::StringConverter::parseReal(XMLNode->first_attribute("qx")->value());
        orientation.y = Ogre::StringConverter::parseReal(XMLNode->first_attribute("qy")->value());
        orientation.z = Ogre::StringConverter::parseReal(XMLNode->first_attribute("qz")->value());
        orientation.w = Ogre::StringConverter::parseReal(XMLNode->first_attribute("qw")->value());
    }
    if(XMLNode->first_attribute("qw"))
    {
        orientation.w = Ogre::StringConverter::parseReal(XMLNode->first_attribute("qw")->value());
        orientation.x = Ogre::StringConverter::parseReal(XMLNode->first_attribute("qx")->value());
        orientation.y = Ogre::StringConverter::parseReal(XMLNode->first_attribute("qy")->value());
        orientation.z = Ogre::StringConverter::parseReal(XMLNode->first_attribute("qz")->value());
    }
    else if(XMLNode->first_attribute("axisX"))
    {
        Ogre::Vector3 axis;
        axis.x = Ogre::StringConverter::parseReal(XMLNode->first_attribute("axisX")->value());
        axis.y = Ogre::StringConverter::parseReal(XMLNode->first_attribute("axisY")->value());
        axis.z = Ogre::StringConverter::parseReal(XMLNode->first_attribute("axisZ")->value());
        Ogre::Real angle = Ogre::StringConverter::parseReal(XMLNode->first_attribute("angle")->value());;
        orientation.FromAngleAxis(Ogre::Angle(angle), axis);
    }
    else if(XMLNode->first_attribute("angleX"))
    {
        Ogre::Vector3 axis;
        axis.x = Ogre::StringConverter::parseReal(XMLNode->first_attribute("angleX")->value());
        axis.y = Ogre::StringConverter::parseReal(XMLNode->first_attribute("angleY")->value());
        axis.z = Ogre::StringConverter::parseReal(XMLNode->first_attribute("angleZ")->value());
        //orientation.FromAxes(&axis);
        //orientation.F
    }
    else if(XMLNode->first_attribute("x"))
    {
        orientation.x = Ogre::StringConverter::parseReal(XMLNode->first_attribute("x")->value());
        orientation.y = Ogre::StringConverter::parseReal(XMLNode->first_attribute("y")->value());
        orientation.z = Ogre::StringConverter::parseReal(XMLNode->first_attribute("z")->value());
        orientation.w = Ogre::StringConverter::parseReal(XMLNode->first_attribute("w")->value());
    }
    else if(XMLNode->first_attribute("w"))
    {
        orientation.w = Ogre::StringConverter::parseReal(XMLNode->first_attribute("w")->value());
        orientation.x = Ogre::StringConverter::parseReal(XMLNode->first_attribute("x")->value());
        orientation.y = Ogre::StringConverter::parseReal(XMLNode->first_attribute("y")->value());
        orientation.z = Ogre::StringConverter::parseReal(XMLNode->first_attribute("z")->value());
    }

    return orientation;
}
