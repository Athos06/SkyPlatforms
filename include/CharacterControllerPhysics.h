#ifndef CC_PHYSICS_H
#define CC_PHYSICS_H

#include <stdio.h>

#include "LinearMath/btVector3.h"

#include "BulletCollision/CollisionDispatch/btGhostObject.h"
#include "BulletCollision/CollisionShapes/btMultiSphereShape.h"
#include "BulletCollision/CollisionShapes/btCapsuleShape.h"
#include "BulletCollision/BroadphaseCollision/btOverlappingPairCache.h"
#include "BulletCollision/BroadphaseCollision/btCollisionAlgorithm.h"btCollisionObject.h 
#include "BulletCollision/CollisionDispatch/btCollisionWorld.h"
#include "LinearMath/btDefaultMotionState.h"
#include "BulletDynamics/Character/btCharacterControllerInterface.h"


class btCharacterControllerInterface;
class btDynamicCharacterController;
class btCollisionShape;
class btPairCachingGhostObject;
class btBroadphaseInterface;
class btCollisionDispatcher;
class btConstraintSolver;
class btDefaultCollisionConfiguration;
class btTriangleIndexVertexArray;


/**
 * Clase que se encarga de las fisicas del personaje
 * Basado en el btKinematiCcharactercontroller de Bullet, con mejoras de
 * diversas fuentes. Consultar la documentacion de bullet para
 * obtener mas informacion sobre el
 */
class CharacterControllerPhysics : public btCharacterControllerInterface
{
	private:
		btScalar mHalfHeight;

		btPairCachingGhostObject * mGhostObject;
		btConvexShape * mConvexShape;
		btConvexShape * mStandingConvexShape;
		btConvexShape * mDuckingConvexShape;

		btCollisionWorld * mCollisionWorld;
		
		btScalar mVerticalVelocity;
		btScalar mVerticalOffset;
		btScalar mFallSpeed;
		btScalar mJumpSpeed;
		btScalar mMaxJumpHeight;
		btScalar mMaxSlopeRadians;
		btScalar mMaxSlopeCosine;
		btScalar mGravity;

		btScalar mTurnAngle;

		btScalar mStepHeight;

		btScalar mAddedMargin;

		btVector3 mWalkDirection;
		btVector3 mNormalizedDirection;

		btVector3 mCurrentPosition;
		btScalar mCurrentStepOffset;
		btVector3 mTargetPosition;

		btManifoldArray mManifoldArray;

		bool mTouchingContact;
		btVector3 mTouchingNormal;

		bool mGrounded;
		bool mWasOnGround;
		bool mWasJumping;
		bool mUseGhostObjectSweepTest;
		bool mUseWalkDirection;
		btScalar mVelocityTimeInterval;
		int mUpAxis;

		btVector3 mLinearVelocity;
		btScalar mMass;
		bool mCanStand;

		bool  m_interpolateUp;
		bool  full_drop;
		bool  bounce_fix;

		//metodo que se le pasa como callback a raytest, y que supuestamente se 
		//utiliza para ignorar al collider propio en caso de objetos kinematicos
		class ClosestNotMeRayResultCallback : public btCollisionWorld::ClosestRayResultCallback
		{
			private:
				btCollisionObject * mMe;

			public:
				ClosestNotMeRayResultCallback(btCollisionObject * me) :
					btCollisionWorld::ClosestRayResultCallback(btVector3(0, 0, 0), btVector3(0, 0, 0)),
					mMe(me)
				{}

				btScalar addSingleResult(btCollisionWorld::LocalRayResult & rayResult, bool normalInWorldSpace)
				{
					if (rayResult.m_collisionObject == mMe)
						return 1.0;
					if (!rayResult.m_collisionObject->hasContactResponse())
						return btScalar(1.0);

					return btCollisionWorld::ClosestRayResultCallback::addSingleResult(rayResult, normalInWorldSpace);
				}
		};

		
		//lo mismo que arriba pero para casos de deteccion de colisiones del collider y no raycast
		class ClosestNotMeConvexResultCallback : public btCollisionWorld::ClosestConvexResultCallback
		{
			private:
				btCollisionObject * mMe;
				const btVector3 mUp;
				btScalar mMinSlopeDot;

			public:
				ClosestNotMeConvexResultCallback(btCollisionObject * me, const btVector3 & up, btScalar minSlopeDot) :
					btCollisionWorld::ClosestConvexResultCallback(btVector3(0, 0, 0), btVector3(0, 0, 0)),
					mMe(me),
					mUp(up),
					mMinSlopeDot(minSlopeDot)
				{}

				btScalar addSingleResult(btCollisionWorld::LocalConvexResult & convexResult, bool normalInWorldSpace)
				{
		
					if (convexResult.m_hitCollisionObject == mMe)
						return 1.0;
					
					if (!convexResult.m_hitCollisionObject->hasContactResponse())
						return btScalar(1.0);

					btVector3 hitNormalWorld;

					if (normalInWorldSpace)
						hitNormalWorld = convexResult.m_hitNormalLocal;
					else
						hitNormalWorld = convexResult.m_hitCollisionObject->getWorldTransform().getBasis() * convexResult.m_hitNormalLocal;

					btScalar dotUp = mUp.dot(hitNormalWorld);

					if (dotUp < mMinSlopeDot)
						return 1.0;

					return btCollisionWorld::ClosestConvexResultCallback::addSingleResult(convexResult, normalInWorldSpace);
				}
		};


		static btVector3 * getUpAxisDirections()
		{
			static btVector3 sUpAxisDirection[3] = {
													btVector3(1, 0, 0),
													btVector3(0, 1, 0),
													btVector3(0, 0, 1)
													};

			return sUpAxisDirection;
		}

		static btVector3 getNormalizedVector(const btVector3 & v)
		{
			btVector3 n = v.normalized();

			if (n.length() < SIMD_EPSILON)
				n.setValue(0, 0, 0);

			return n;
		}

		btVector3 computeReflectionDirection(const btVector3 & direction, const btVector3 & normal);
		btVector3 parallelComponent(const btVector3 & direction, const btVector3 & normal);
		btVector3 perpindicularComponent(const btVector3 & direction, const btVector3 & normal);
		void setPlayerMode();

	public:
		btPairCachingGhostObject * getGhostObject();

		CharacterControllerPhysics(btPairCachingGhostObject * ghostObject, btConvexShape * convexShape, btScalar stepHeight,
									btCollisionWorld * collisionWorld, int upAxis = 1);
		~CharacterControllerPhysics();
		void setDuckingConvexShape(btConvexShape * shape);
		bool recoverFromPenetration(btCollisionWorld * collisionWorld);
		void stepUp(btCollisionWorld * collisionWorld);
		void setRBForceImpulseBasedOnCollision();
		void updateTargetPositionBasedOnCollision(const btVector3 & hitNormal, btScalar tangentMag = 0, btScalar normalMag = 1);
		void stepForwardAndStrafe(btCollisionWorld * collisionWorld, const btVector3 & walkMove);
		void stepDown(btCollisionWorld * collisionWorld, btScalar dt);
		void setVelocityForTimeInterval(const btVector3 & velocity, btScalar timeInterval);

		void reset();
		//void reset ( btCollisionWorld* collisionWorld ){};

		void warp(const btVector3 & origin);
		void preStep(btCollisionWorld * collisionWorld);
		void playerStep(btCollisionWorld * collisionWorld, btScalar dt);
		void setFallSpeed(btScalar fallSpeed);
		void setJumpSpeed(btScalar jumpSpeed);
		void setMaxJumpHeight(btScalar maxJumpHeight);
		bool canJump() const;
		void jump();
		void duck();
		void stand();
		bool canStand();
		void move(bool forward, bool backward, bool left, bool right); /*, bool up, bool down*/
		void setGravity(const btScalar gravity);
		btScalar getGravity() const;
		void setMaxSlope(btScalar slopeRadians);
		btScalar getMaxSlope() const;
		bool onGround() const;
		void setWalkDirection(const btVector3 & walkDirection);
		void setWalkDirection(const btScalar x, const btScalar y, const btScalar z);
		void setOrientation(const btQuaternion & orientation);
		btVector3 getWalkDirection() const;
		btVector3 getPosition() const;
		btScalar getVerticalVelocity() const;
		void debugDraw(btIDebugDraw * debugDrawer)
		{
		}
		
		void setUpInterpolate (bool value);

		void updateAction(btCollisionWorld * collisionWorld, btScalar dt);
};

#endif // CC_PHYSICS_H
