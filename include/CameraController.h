//|||||||||||||||||||||||||||||||||||||||||||||||||
#ifndef CAMERA_MGR_H
#define CAMERA_MGR_H
//|||||||||||||||||||||||||||||||||||||||||||||||||

#include <Ogre.h>
//|||||||||||||||||||||||||||||||||||||||||||||||||

/**
 *  @author Jorge Rosique Contreras
 *  @date 14-6-2014
 *
 *  Clase que se encarga de gestionar el movimiento
 *	de la camara del jugador
 */
class CameraController{
public:
	CameraController();
	~CameraController();
	/**
	@brief inicializa y configura la camara
	@param camera la camara que utilizamos
	@param target el objetivo al que la camara seguira
	*/ 
	void setupCamera(Ogre::Camera* camera, Ogre::SceneNode* target);
	/**
	@brief llamado cada frame para actualizar la camara
	@param deltaTime tiempo en ms transcurrido desde el ultimo frame
	*/ 
	void update(double deltaTime);
	/**
	@param estable la posicion deseada para la camara
	@deltaYaw la rotacion deseada en el eje X
	@deltaPitch la rotacion deseada en el eje Y
	@deltaZoom la distancia deseada al jugador
	*/
	void updateCameraGoal(float deltaYaw, float deltaPitch, float deltaZoom);
	/** 
	@brief resetea la camara
	*/ 
	void reset();

	float targetHeight; //1
	float currentDistance;		
	float xSpeed;		
	float ySpeed;
	
	//limite en el que puede rotar la camara en el eje vertical, min y max
	float yMinLimit;	//-10	
	float yMaxLimit;	//45
	
	float initialRotation;

	float minZoom;
	float maxZoom;

	float initialZoom;
	
	float yRotGoal;
	float xRotGoal;
	float zoomGoal;

	float xRotSpeed;
	float yRotSpeed;
	float zoomSpeed;

	//nodo sobre el que rota la camara
	Ogre::SceneNode* m_pCameraPivot;
	//utilizado para la rotacion en el eje y sin producil gimbal lock
	Ogre::SceneNode* m_pCameraPitchPivot;
	//nodo en el que se encuentra la camara
	Ogre::SceneNode* m_pCameraNode;
	
	void setFollowPlayer(bool follow){
		m_bFollowPlayer = follow;
	};

private:
	float m_InitialPitch;
	float m_PivotPitch;
	
	bool m_bFollowPlayer;

	Ogre::SceneNode* m_pTargetToFollow;

};
//|||||||||||||||||||||||||||||||||||||||||||||||||

#endif