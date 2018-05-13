#ifndef CONSTANTS_H 
#define CONSTANTS_H

#include "btBulletDynamicsCommon.h"

/*
//para filtrado de colisiones en bullet
#define BIT(x) (1<<(x))

enum collisiontypes {
    COL_NOTHING = 0, //<Collide with nothing
    COL_DEFAULT = BIT(0),
	COL_PROYECTILE = BIT(1), 
	COL_TRIGGERS = BIT(2), 
	COL_PLAYER = BIT(3), 
};

const int proyectileCollidesWith = COL_DEFAULT | COL_PLAYER;
const int triggerCollidesWith = COL_DEFAULT;
const int defaultCollidesWith = COL_DEFAULT | COL_PROYECTILE | COL_TRIGGERS;
//const int playerCollidesWith = COL_PROYECTILE;
//const int obstaclesCollidesWith = COL_PROYECTILE;
*/

//|||||||||||||||||||||||||||||||||||||||||||||||||||||

const float PI = 3.14159265;
//|||||||||||||||||||||||||||||||||||||||||||||||||||||

//tipos de objectos en bullet
const int STATIC	= 0;
const int KINEMATIC = 1;
const int DYNAMIC	= 2;

//tipos de collider en bullet
const int PRIMITIVE_BOX = 0;
const int PRIMITIVE_SPHERE = 1;
const int COLLISION_MESH = 2;
//|||||||||||||||||||||||||||||||||||||||||||||||||||||

//canales de sonido para reproducion de soundsFX 
const int FOOTSTEP_CHANNEL = 1;
const int MAINFX_CHANNEL = 2;
const int SECONDARYFX_CHANNEL = 3;
const int HIT_CHANNEL = 4;
const int PICKUP_CHANNEL = 5;
const int SHOT_CHANNEL = 6;
const int EXPLOSION_CHANNEL = 7;
const int MENU1_CHANNEL = 8;
const int MENU2_CHANNEL = 9;

//niveles para seleccionar intro
const int INTRO_LEVEL1 = 1;
const int INTRO_LEVEL2 = 2;
const int INTRO_LEVEL3 = 3;
const int INTRO_LEVELBOSS = 4;
#endif