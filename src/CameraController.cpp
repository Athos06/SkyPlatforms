//|||||||||||||||||||||||||||||||||||||||||||||||||

#include "CameraController.h"
#include "GameManager.h"
#include "Euler.h"
#include "Utils.h"

//|||||||||||||||||||||||||||||||||||||||||||||||||

CameraController::CameraController(){
	yMinLimit = 10;
	yMaxLimit = -30;

	initialRotation = -5;

	maxZoom = 10;
	minZoom = 4;

	initialZoom = 6;

	xRotSpeed = 8;
	yRotSpeed = 8;
	zoomSpeed = 10;

	yRotGoal = 0;
	xRotGoal = 0;
	zoomGoal = 0;

	m_pCameraPivot = 0;
	m_pCameraPitchPivot = 0;
	m_pCameraNode = 0;

}
//|||||||||||||||||||||||||||||||||||||||||||||||||

CameraController::~CameraController(){
}
//|||||||||||||||||||||||||||||||||||||||||||||||||

void CameraController::setupCamera(Ogre::Camera *camera, Ogre::SceneNode* target){
	m_bFollowPlayer = true;

	Ogre::SceneManager* sceneMgr = GameManager::getSingletonPtr()->getSceneMgr();
	//el nodo que movemos para seguir al jugador
	m_pCameraPivot = sceneMgr->getRootSceneNode()->createChildSceneNode
		("NodeCameraPivot");
	//nodo para rotar en el ejeX sin gimbal lock
	m_pCameraPitchPivot = m_pCameraPivot->createChildSceneNode
		("NodeCameraPitch", Ogre::Vector3(0, 0, 0) );
	//el nodo que incluye a la camara, y que es hijo del camerapivot
	//por lo que se mueve con este junto al jugador
	//Solo se mueve en el eje z local para ajustar la distancia del zoom
	m_pCameraNode = m_pCameraPitchPivot->createChildSceneNode
		("NodeCameraNode", Ogre::Vector3(0, 0, 5) );

	camera->setPosition(0,0,0);
	m_pCameraNode->attachObject(camera);

	m_pTargetToFollow = target;

	
	targetHeight = 1;
	currentDistance = initialZoom;
	zoomGoal = currentDistance;
	xRotGoal = 0;
	yRotGoal = initialRotation;

	Ogre::Vector3 goalPosition = target->getPosition() + 
		Ogre::Vector3(0, targetHeight, currentDistance);


	m_pCameraPivot->setPosition(target->getPosition());
	m_pCameraNode->setPosition(0, targetHeight, currentDistance);
	
	//la inclinacion inicial de la camara
	m_pCameraPitchPivot->pitch(Ogre::Degree(initialRotation));
}

void CameraController::update(double timeSinceLastFrame){
	float deltaTime = timeSinceLastFrame/1000;

	if(m_bFollowPlayer){
		//actualizamos la posicion de la camara cada frame respecto al jugador
		Ogre::Vector3 goalPosition = m_pTargetToFollow->getPosition(); //+ 
			//Ogre::Vector3(0, targetHeight, currentDistance);
		Ogre::Vector3 goalOffset = goalPosition - m_pCameraPivot->getPosition();
		m_pCameraPivot->translate(goalOffset * 10 * deltaTime);  //* deltaTime * 9.0f
	}

	
	//actualizamos la rotacion de la camara (si se ha rotado)
	//eje horizontal
	float xRotOffset = xRotGoal;
	xRotOffset =  xRotOffset * xRotSpeed * deltaTime;
	m_pCameraPivot->yaw(Ogre::Degree(xRotOffset));
	xRotGoal -= xRotOffset;
	if(xRotGoal <= 0) xRotGoal = 0;
	//eje vertical
	
	Ogre::Euler angle;
	angle.fromQuaternion(m_pCameraPitchPivot->getOrientation());
	float yCurrent = angle.pitch().valueDegrees();
	float yRotOffset = yRotGoal - yCurrent;
	yRotOffset = yRotOffset * yRotSpeed * deltaTime;
	m_pCameraPitchPivot->pitch(Ogre::Degree(yRotOffset));
	

	//actualizamos la distancia de la camara al jugador
	float zoomOffset = zoomGoal - currentDistance;
	zoomOffset = zoomOffset * zoomSpeed * deltaTime;
	m_pCameraNode->translate(Ogre::Vector3(0,0,zoomOffset), Ogre::Node::TS_LOCAL);
	currentDistance = currentDistance + zoomOffset;
	
}

void CameraController::reset(){
	m_bFollowPlayer = true;

	targetHeight = 1;
	currentDistance = initialZoom;
	zoomGoal = currentDistance;
	xRotGoal = 0;
	yRotGoal = initialRotation;

	Ogre::Vector3 goalPosition = m_pTargetToFollow->getPosition() + 
		Ogre::Vector3(0, targetHeight, currentDistance);

	m_pCameraPivot->setPosition(m_pTargetToFollow->getPosition());
	m_pCameraNode->setPosition(0, targetHeight, currentDistance);
	
	m_pCameraPivot->setOrientation(Ogre::Quaternion::IDENTITY);
	m_pCameraPitchPivot->setOrientation(Ogre::Quaternion::IDENTITY);
	//la inclinacion inicial de la camara
	m_pCameraPitchPivot->pitch(Ogre::Degree(initialRotation));
}

void CameraController::updateCameraGoal(float deltaYaw, float deltaPitch, float deltaZoom){
	//actualizamos la rotacion deseada para el eje x
	xRotGoal = deltaYaw;
	//actualizamos la rotacion deseada para el eje y
	Ogre::Euler angle;
	angle.fromQuaternion(m_pCameraPitchPivot->getOrientation());
	float y = angle.pitch().valueDegrees();
	float goalPitch = y + deltaPitch;
	//por como  los ejes hay que invertir el signo
	//en realidad cuando estmos ratando la camara hacia arriba estamos
	//rotando por valores negativos. Por eso en realidad el maximo se
	//pasa como negativo, porque es -45 y el minimo es 10
	yRotGoal = Utils::clampAngle(goalPitch, yMaxLimit, yMinLimit);
	//ajustando la distancia de la camara al jugador
	float goalZoom = currentDistance + deltaZoom;
	float min = 4;
	float max = 10;
	zoomGoal = Ogre::Math::Clamp(goalZoom, min, max);
}
//|||||||||||||||||||||||||||||||||||||||||||||||||
