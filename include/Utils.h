//||||||||||||||||||||||||||||||||||||||||||||

#ifndef UTILS_H
#define UTILS_H
//||||||||||||||||||||||||||||||||||||||||||||

#include <Ogre.h>
//||||||||||||||||||||||||||||||||||||||||||||

class Utils{
public:
	Utils(){};
	~Utils(){};

	static float clampAngle(float angle, float min, float max){
		if(angle < -360)
			angle += 360;
		if(angle > 360)
			angle -=360;

		return Ogre::Math::Clamp(angle, min, max);
	}
};
//||||||||||||||||||||||||||||||||||||||||||||

#endif
//||||||||||||||||||||||||||||||||||||||||||||
