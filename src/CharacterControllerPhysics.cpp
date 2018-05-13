#include <stdio.h>

#include <Ogre.h>
#include "BtOgrePG.h"
#include "BtOgreGP.h"
#include "BtOgreExtras.h"
#include "CharacterControllerPhysics.h"

CharacterControllerPhysics::~CharacterControllerPhysics(){
	mGhostObject = 0;
	mConvexShape = 0;
	mStandingConvexShape = 0;
	mDuckingConvexShape = 0;
	mCollisionWorld = 0;
}

btVector3 CharacterControllerPhysics::computeReflectionDirection(const btVector3 & direction, const btVector3 & normal)
{
	return direction - (btScalar(2) * direction.dot(normal)) * normal;
}

btVector3 CharacterControllerPhysics::parallelComponent(const btVector3 & direction, const btVector3 & normal)
{
	btScalar magnitude = direction.dot(normal);
	return normal * magnitude;
}

btVector3 CharacterControllerPhysics::perpindicularComponent(const btVector3 & direction, const btVector3 & normal)
{
	return direction - parallelComponent(direction, normal);
}

btPairCachingGhostObject * CharacterControllerPhysics::getGhostObject()
{
	return mGhostObject;
}

CharacterControllerPhysics::CharacterControllerPhysics(btPairCachingGhostObject * ghostObject, btConvexShape * convexShape, btScalar stepHeight,
														 btCollisionWorld * collisionWorld, int upAxis)
{
	mUpAxis = upAxis;
	mAddedMargin = 0.02;
	mWalkDirection.setValue(0, 0, 0);
	mUseGhostObjectSweepTest = true;
	mGhostObject = ghostObject;
	mStepHeight = stepHeight;
	mTurnAngle = 0;
	mConvexShape = mStandingConvexShape = convexShape;
	mUseWalkDirection = true;
	mVelocityTimeInterval = 0;
	mVerticalOffset = 0;
	mGrounded = false;
	mVerticalVelocity = 0;
	mGravity = 10 * 2;
	mFallSpeed = 45.0; // Terminal velocity of a sky diver in m/s.
	mJumpSpeed = 12;
	mWasOnGround = false;
	mWasJumping = false;
	setMaxSlope(btRadians(40));
	mCollisionWorld = collisionWorld;
	mCanStand = true;
	mCurrentPosition.setValue(0, 0, 0);
	mMass = 20;

	//fix para la velocidad de caida
	m_interpolateUp = true;
	mCurrentStepOffset = 0;
	full_drop = false;
	bounce_fix = false;
}

void CharacterControllerPhysics::setDuckingConvexShape(btConvexShape * shape)
{
	mDuckingConvexShape = shape;
}

bool CharacterControllerPhysics::recoverFromPenetration(btCollisionWorld * collisionWorld)
{

	//////////////////////////////////////////////////////
	//PROBAR EL PARCHE A VER SI HACE ALGO
	//////////////////////////////////////////////////////
	// Here we must refresh the overlapping paircache as the penetrating movement itself or the
	// previous recovery iteration might have used setWorldTransform and pushed us into an object
	// that is not in the previous cache contents from the last timestep, as will happen if we
	// are pushed into a new AABB overlap. Unhandled this means the next convex sweep gets stuck.
	//
	// Do this by calling the broadphase's setAabb with the moved AABB, this will update the broadphase
	// paircache and the ghostobject's internal paircache at the same time.    /BW

	btVector3 minAabb, maxAabb;
	mConvexShape->getAabb(mGhostObject->getWorldTransform(), minAabb,maxAabb);
	collisionWorld->getBroadphase()->setAabb(mGhostObject->getBroadphaseHandle(),
						 minAabb,
						 maxAabb,
						 collisionWorld->getDispatcher());

	/////////////////////////////////////////////////////
	bool penetration = false;

	collisionWorld->getDispatcher()->dispatchAllCollisionPairs(mGhostObject->getOverlappingPairCache(), collisionWorld->getDispatchInfo(), collisionWorld->getDispatcher());

	mCurrentPosition = mGhostObject->getWorldTransform().getOrigin();

	btScalar maxPen = 0;

	for (int i = 0; i < mGhostObject->getOverlappingPairCache()->getNumOverlappingPairs(); i++)
	{
		mManifoldArray.resize(0);

		btBroadphasePair * collisionPair = &mGhostObject->getOverlappingPairCache()->getOverlappingPairArray()[i];


		///////////////////////////////////////////////////
		////////////OTROPARCHE MAS/////////////////////////
		//creo que es lo mismo que abajo de filtering trigger
		btCollisionObject* obj0 = static_cast<btCollisionObject*>
			(collisionPair->m_pProxy0->m_clientObject);
        btCollisionObject* obj1 = static_cast<btCollisionObject*>
			(collisionPair->m_pProxy1->m_clientObject);

		if ((obj0 && !obj0->hasContactResponse()) || (obj1 && !obj1->hasContactResponse()))
			continue;

		///////////////////////////////////////////////////



		//for trigger filtering
		if (!static_cast<btCollisionObject*>(collisionPair->m_pProxy0->m_clientObject)->hasContactResponse() ||
		!static_cast<btCollisionObject*>(collisionPair->m_pProxy1->m_clientObject)->hasContactResponse())
			continue;

		if (collisionPair->m_algorithm)
			collisionPair->m_algorithm->getAllContactManifolds(mManifoldArray);

		for (int j = 0; j < mManifoldArray.size(); j++)
		{
			btPersistentManifold * manifold = mManifoldArray[j];
			btScalar directionSign = manifold->getBody0() == mGhostObject ? btScalar(-1) : btScalar(1);

			for (int p = 0; p < manifold->getNumContacts(); p++)
			{
				const btManifoldPoint & pt = manifold->getContactPoint(p);

				btScalar dist = pt.getDistance();

				if (dist < 0)
				{
					maxPen = dist;
					mTouchingNormal = pt.m_normalWorldOnB * directionSign;
					penetration = true;
				}

				mCurrentPosition += pt.m_normalWorldOnB * directionSign * dist * btScalar(0.2);


			}
		}
	}

	btTransform newTrans = mGhostObject->getWorldTransform();
	newTrans.setOrigin(mCurrentPosition);
	mGhostObject->setWorldTransform(newTrans);

	return penetration;
}

void CharacterControllerPhysics::setRBForceImpulseBasedOnCollision()
{

	/*
	if (!mWalkDirection.isZero())
	{

		for (int i = 0; i < mGhostObject->getOverlappingPairCache()->getNumOverlappingPairs(); i++)
		{
			btBroadphasePair * collisionPair = &mGhostObject->getOverlappingPairCache()->getOverlappingPairArray()[i];

			btRigidBody * rb = (btRigidBody*)collisionPair->m_pProxy1->m_clientObject;
		
			if (mMass > rb->getInvMass())
			{
				btScalar resultMass = mMass - rb->getInvMass();
				btVector3 reflection = computeReflectionDirection(mWalkDirection * resultMass, getNormalizedVector(mWalkDirection));
				rb->applyCentralImpulse(reflection * -1);
			}
		}

	}*/ 
}

void CharacterControllerPhysics::stepUp(btCollisionWorld * collisionWorld)
{
	btTransform start, end;
	mTargetPosition = mCurrentPosition + getUpAxisDirections()[mUpAxis] * (mStepHeight + (mVerticalOffset > 0 ? mVerticalOffset : 0));

		
	start.setIdentity();
	end.setIdentity();

	start.setOrigin(mCurrentPosition);
	end.setOrigin(mTargetPosition);

	ClosestNotMeConvexResultCallback callback(mGhostObject, getUpAxisDirections()[mUpAxis], btScalar(0.7071));
	callback.m_collisionFilterGroup = getGhostObject()->getBroadphaseHandle()->m_collisionFilterGroup;
	callback.m_collisionFilterMask = getGhostObject()->getBroadphaseHandle()->m_collisionFilterMask;

	if (mUseGhostObjectSweepTest)
		mGhostObject->convexSweepTest(mConvexShape, start, end, callback, collisionWorld->getDispatchInfo().m_allowedCcdPenetration);
	else
		collisionWorld->convexSweepTest(mConvexShape, start, end, callback);

	if (callback.hasHit())
	{
		// Only modify the position if the hit was a slope and not a wall or ceiling.
		if (callback.m_hitNormalWorld.dot(getUpAxisDirections()[mUpAxis]) > 0)
		{
			mCurrentStepOffset = mStepHeight * callback.m_closestHitFraction;
					
			////////////////////////////////////////
			//PARCHE/////////////////////////////////
			if (m_interpolateUp == true)
				mCurrentPosition.setInterpolate3 (mCurrentPosition, mTargetPosition, callback.m_closestHitFraction);
			else
				mCurrentPosition = mTargetPosition;

			mCurrentPosition.setInterpolate3(mCurrentPosition, mTargetPosition, callback.m_closestHitFraction);
		}
		//////////////////////////////////////////////
		mVerticalOffset = mVerticalVelocity = 0;
		mGrounded = true;
	}
	else
	{
		mCurrentStepOffset = mStepHeight;
		mCurrentPosition = mTargetPosition;
	}
}

void CharacterControllerPhysics::updateTargetPositionBasedOnCollision(const btVector3 & hitNormal, btScalar tangentMag, btScalar normalMag)
{
	btVector3 movementDirection = mTargetPosition - mCurrentPosition;
	btScalar movementLenght = movementDirection.length();

	if (movementLenght > SIMD_EPSILON)
	{
		movementDirection.normalize();

		btVector3 reflectDir = computeReflectionDirection(movementDirection, hitNormal);

		btVector3 parallelDir, perpindicularDir;

		parallelDir = parallelComponent(reflectDir, hitNormal);
		perpindicularDir = perpindicularComponent(reflectDir, hitNormal);

		mTargetPosition = mCurrentPosition;

		if (normalMag != 0)
		{
			btVector3 perpComponent = perpindicularDir * btScalar(normalMag * movementLenght);
			mTargetPosition += perpComponent;
		}
	}
}

void CharacterControllerPhysics::stepForwardAndStrafe(btCollisionWorld * collisionWorld, const btVector3 & walkMove)
{
	btTransform start, end;
	mTargetPosition = mCurrentPosition + walkMove;

	start.setIdentity();
	end.setIdentity();

	btScalar fraction = 1.0;
	btScalar distance2 = (mCurrentPosition - mTargetPosition).length2();

	if (mTouchingContact)
	{
		if (mNormalizedDirection.dot(mTouchingNormal) > 0)

			//interferes with step movement
			updateTargetPositionBasedOnCollision(mTouchingNormal);
			//updateTargetPositionBasedOnCollision(mTouchingNormal);
	}

	int maxIter = 10;

	while (fraction > 0.01 && maxIter-- > 0)
	{
		start.setOrigin(mCurrentPosition);
		end.setOrigin(mTargetPosition);

		btVector3 sweepDirNegative(mCurrentPosition - mTargetPosition);

		ClosestNotMeConvexResultCallback callback(mGhostObject, sweepDirNegative, 0);
		callback.m_collisionFilterGroup = getGhostObject()->getBroadphaseHandle()->m_collisionFilterGroup;
		callback.m_collisionFilterMask = getGhostObject()->getBroadphaseHandle()->m_collisionFilterMask;

		btScalar margin = mConvexShape->getMargin();
		mConvexShape->setMargin(margin + mAddedMargin);

		if (mUseGhostObjectSweepTest)
			mGhostObject->convexSweepTest(mConvexShape, start, end, callback, collisionWorld->getDispatchInfo().m_allowedCcdPenetration);
		else
			collisionWorld->convexSweepTest(mConvexShape, start, end, callback, collisionWorld->getDispatchInfo().m_allowedCcdPenetration);

		mConvexShape->setMargin(margin);

		fraction -= callback.m_closestHitFraction;

		if (callback.hasHit())
		{
			btScalar hitDistance = (callback.m_hitPointWorld - mCurrentPosition).length();

			updateTargetPositionBasedOnCollision(callback.m_hitNormalWorld);
			btVector3 currentDir = mTargetPosition - mCurrentPosition;
			distance2 = currentDir.length2();

			if (distance2 > SIMD_EPSILON)
			{
				currentDir.normalize();

				if (currentDir.dot(mNormalizedDirection) <= 0)
					break;
			}
			else
				break;
		}
		else
			mCurrentPosition = mTargetPosition;
	}
}

////	PARCHEANDO PROFUNDAMENTE//////////////////////////////
void CharacterControllerPhysics::stepDown ( btCollisionWorld* collisionWorld, btScalar dt)
{
        btTransform start, end, end_double;
        bool runonce = false;

        // phase 3: down
        /*btScalar additionalDownStep = (mWasOnGround && !onGround()) ? mStepHeight : 0.0;
        btVector3 step_drop = getUpAxisDirections()[mUpAxis] * (mCurrentStepOffset + additionalDownStep);
        btScalar downVelocity = (additionalDownStep == 0.0 && mVerticalVelocity<0.0?-mVerticalVelocity:0.0) * dt;
        btVector3 gravity_drop = getUpAxisDirections()[mUpAxis] * downVelocity; 
        mTargetPosition -= (step_drop + gravity_drop);*/

        btVector3 orig_position = mTargetPosition;
        
        btScalar downVelocity = (mVerticalVelocity<0.f?-mVerticalVelocity:0.f) * dt;

        if(downVelocity > 0.0 && downVelocity > mFallSpeed
                && (mWasOnGround || !mWasJumping))
                downVelocity = mFallSpeed;

        btVector3 step_drop = getUpAxisDirections()[mUpAxis] * (mCurrentStepOffset + downVelocity);
        mTargetPosition -= step_drop;

        ClosestNotMeConvexResultCallback callback (mGhostObject, getUpAxisDirections()[mUpAxis], mMaxSlopeCosine);
        callback.m_collisionFilterGroup = getGhostObject()->getBroadphaseHandle()->m_collisionFilterGroup;
        callback.m_collisionFilterMask = getGhostObject()->getBroadphaseHandle()->m_collisionFilterMask;

        ClosestNotMeConvexResultCallback callback2 (mGhostObject, getUpAxisDirections()[mUpAxis], mMaxSlopeCosine);
        callback2.m_collisionFilterGroup = getGhostObject()->getBroadphaseHandle()->m_collisionFilterGroup;
        callback2.m_collisionFilterMask = getGhostObject()->getBroadphaseHandle()->m_collisionFilterMask;

        while (1)
        {
            start.setIdentity ();
            end.setIdentity ();

            end_double.setIdentity ();

            start.setOrigin (mCurrentPosition);
            end.setOrigin (mTargetPosition);

            //set double test for 2x the step drop, to check for a large drop vs small drop
            end_double.setOrigin (mTargetPosition - step_drop);

            if (mUseGhostObjectSweepTest)
            {
                    mGhostObject->convexSweepTest (mConvexShape, start, end, callback, collisionWorld->getDispatchInfo().m_allowedCcdPenetration);

                    if (!callback.hasHit())
                    {
                            //test a double fall height, to see if the character should interpolate it's fall (full) or not (partial)
                            mGhostObject->convexSweepTest (mConvexShape, start, end_double, callback2, collisionWorld->getDispatchInfo().m_allowedCcdPenetration);
                    }
            } else
            {
                    collisionWorld->convexSweepTest (mConvexShape, start, end, callback, collisionWorld->getDispatchInfo().m_allowedCcdPenetration);

                    if (!callback.hasHit())
                                    {
                                                    //test a double fall height, to see if the character should interpolate it's fall (large) or not (small)
                                                    collisionWorld->convexSweepTest (mConvexShape, start, end_double, callback2, collisionWorld->getDispatchInfo().m_allowedCcdPenetration);
                                    }
            }
    
            btScalar downVelocity2 = (mVerticalVelocity<0.f?-mVerticalVelocity:0.f) * dt;
            bool has_hit = false;
            if (bounce_fix == true)
                    has_hit = callback.hasHit() || callback2.hasHit();
            else
                    has_hit = callback2.hasHit();

            if(downVelocity2 > 0.0 && downVelocity2 < mStepHeight && has_hit == true && runonce == false
                                    && (mWasOnGround || !mWasJumping))
            {
                    //redo the velocity calculation when falling a small amount, for fast stairs motion
                    //for larger falls, use the smoother/slower interpolated movement by not touching the target position

                    mTargetPosition = orig_position;
                                    downVelocity = mStepHeight;

                            btVector3 step_drop = getUpAxisDirections()[mUpAxis] * (mCurrentStepOffset + downVelocity);
                    mTargetPosition -= step_drop;
                    runonce = true;
                    continue; //re-run previous tests
            }
            break;
        }

        if (callback.hasHit() || runonce == true)
        {
                // we dropped a fraction of the height -> hit floor

                btScalar fraction = (mCurrentPosition.getY() - callback.m_hitPointWorld.getY()) / 2;

                //printf("hitpoint: %g - pos %g\n", callback.m_hitPointWorld.getY(), mCurrentPosition.getY());

                if (bounce_fix == true)
                {
                        if (full_drop == true)
                                mCurrentPosition.setInterpolate3 (mCurrentPosition, mTargetPosition, callback.m_closestHitFraction);
                        else
                                //due to errors in the closestHitFraction variable when used with large polygons, calculate the hit fraction manually
                                mCurrentPosition.setInterpolate3 (mCurrentPosition, mTargetPosition, fraction);
                }
                else
                        mCurrentPosition.setInterpolate3 (mCurrentPosition, mTargetPosition, callback.m_closestHitFraction);

                full_drop = false;

                mVerticalVelocity = 0.0;
                mVerticalOffset = 0.0;
								mGrounded = true;
                mWasJumping = false;
        } else {
                // we dropped the full height
                
                full_drop = true;

                if (bounce_fix == true)
                {
                        downVelocity = (mVerticalVelocity<0.f?-mVerticalVelocity:0.f) * dt;
                        if (downVelocity > mFallSpeed && (mWasOnGround || !mWasJumping))
                        {
                                mTargetPosition += step_drop; //undo previous target change
                                downVelocity = mFallSpeed;
                                step_drop = getUpAxisDirections()[mUpAxis] * (mCurrentStepOffset + downVelocity);
                                mTargetPosition -= step_drop;
                        }
                }
                //printf("full drop - %g, %g\n", mCurrentPosition.getY(), mTargetPosition.getY());

                mCurrentPosition = mTargetPosition;
        }
}



void CharacterControllerPhysics::setVelocityForTimeInterval(const btVector3 & velocity, btScalar timeInterval)
{
	mUseWalkDirection = false;
	mWalkDirection = velocity;
	mNormalizedDirection = getNormalizedVector(mWalkDirection);
	mVelocityTimeInterval = timeInterval;
}

void CharacterControllerPhysics::warp(const btVector3 & origin)
{
	btTransform xform;
	xform.setIdentity();
	xform.setOrigin(origin);
	mGhostObject->setWorldTransform(xform);
}

void CharacterControllerPhysics::preStep(btCollisionWorld * collisionWorld)
{
	int numPenetrationLoops = 0;
	mTouchingContact = false;

	while (recoverFromPenetration(collisionWorld))
	{
		numPenetrationLoops++;
		mTouchingContact = true;

		if (numPenetrationLoops > 4)
			break;
	}

	mCurrentPosition = mGhostObject->getWorldTransform().getOrigin();
	mTargetPosition = mCurrentPosition;

}

void CharacterControllerPhysics::playerStep(btCollisionWorld * collisionWorld, btScalar dt)
{
	if (!mUseWalkDirection && mVelocityTimeInterval <= 0)
		return;

	mGrounded = false;

	mWasOnGround = onGround();

	//setRBForceImpulseBasedOnCollision();

	mVerticalVelocity -= mGravity * dt;

	if (mVerticalVelocity > 0 && mVerticalVelocity > mJumpSpeed)
		mVerticalVelocity = mJumpSpeed;

	if (mVerticalVelocity < 0 && btFabs(mVerticalVelocity) > btFabs(mFallSpeed))
		mVerticalVelocity = -btFabs(mFallSpeed);

	mVerticalOffset = mVerticalVelocity * dt;

	btTransform xform;
	xform = mGhostObject->getWorldTransform();

	stepUp(collisionWorld);

	if (mUseWalkDirection)
		stepForwardAndStrafe(collisionWorld, mWalkDirection);
	else
	{
		// still have some time left for moving!
		btScalar dtMoving = (dt < mVelocityTimeInterval) ? dt : mVelocityTimeInterval;
		mVelocityTimeInterval -= dt;

		// how far will we move while we are moving?
		btVector3 move = mWalkDirection * dtMoving;

		// okay, step
		stepForwardAndStrafe(collisionWorld, move);
	}

	stepDown(collisionWorld, dt);

	xform.setOrigin(mCurrentPosition);
	mGhostObject->setWorldTransform(xform);

	if (!mCanStand)
		stand(); // check if we can stand
}

void CharacterControllerPhysics::setFallSpeed(btScalar fallSpeed)
{
	mFallSpeed = fallSpeed;
}

void CharacterControllerPhysics::setJumpSpeed(btScalar jumpSpeed)
{
	mJumpSpeed = jumpSpeed;
}

void CharacterControllerPhysics::setMaxJumpHeight(btScalar maxJumpHeight)
{
	mMaxJumpHeight = maxJumpHeight;
}

bool CharacterControllerPhysics::canJump() const
{
	return onGround();
}

void CharacterControllerPhysics::jump()
{
	if (!canJump())
		return;

	mVerticalVelocity = mJumpSpeed;
	mWasJumping = true;
}

void CharacterControllerPhysics::duck()
{
	mConvexShape = mDuckingConvexShape;
	mGhostObject->setCollisionShape(mDuckingConvexShape);

	btTransform xform;
	xform.setIdentity();
	xform.setOrigin(mCurrentPosition + btVector3(0, 0.1, 0));
	mGhostObject->setWorldTransform(xform);
}

void CharacterControllerPhysics::stand()
{
	if (!canStand())
		return;

	mConvexShape = mStandingConvexShape;
	mGhostObject->setCollisionShape(mStandingConvexShape);
}

bool CharacterControllerPhysics::canStand()
{
	btTransform start, end;

	start.setIdentity();
	end.setIdentity();

	start.setOrigin(mCurrentPosition);
	end.setOrigin(mCurrentPosition + btVector3(0,
												static_cast<btCapsuleShape*>(mStandingConvexShape)->getHalfHeight() * 2 - static_cast<btCapsuleShape*>(mDuckingConvexShape)->getHalfHeight() * 2,
												0));

	ClosestNotMeConvexResultCallback callback(mGhostObject, -getUpAxisDirections()[mUpAxis], 0);
	callback.m_collisionFilterGroup = getGhostObject()->getBroadphaseHandle()->m_collisionFilterGroup;
	callback.m_collisionFilterMask = getGhostObject()->getBroadphaseHandle()->m_collisionFilterMask;

	if (mUseGhostObjectSweepTest)
		mGhostObject->convexSweepTest(mConvexShape, start, end, callback, mCollisionWorld->getDispatchInfo().m_allowedCcdPenetration);
	else
		mCollisionWorld->convexSweepTest(mConvexShape, start, end, callback);

	if (callback.hasHit())
	{
		mCanStand = false;
		return false;
	}
	else
	{
		mCanStand = true;
		return true;
	}
}

void CharacterControllerPhysics::move(bool forward, bool backward, bool left, bool right)
{
	btVector3 walkDirection(0, 0, 0);
	btVector3 frontSpeed(0, 0, .15);
	btVector3 sideSpeed(.13, 0, 0);

	if (forward)
		walkDirection += frontSpeed;

	if (backward)
		walkDirection -= frontSpeed;

	if (left)
		walkDirection -= sideSpeed;

	if (right)
		walkDirection += sideSpeed;



	mLinearVelocity = walkDirection;
}

void CharacterControllerPhysics::setGravity(const btScalar gravity)
{
	mGravity = gravity;
}

btScalar CharacterControllerPhysics::getGravity() const
{
	return mGravity;
}

void CharacterControllerPhysics::setMaxSlope(btScalar slopeRadians)
{
	mMaxSlopeRadians = slopeRadians;
	mMaxSlopeCosine = btCos(slopeRadians);
}

btScalar CharacterControllerPhysics::getMaxSlope() const
{
	return mMaxSlopeRadians;
}

bool CharacterControllerPhysics::onGround() const
{
	
	//en linux esto no funciona bien, en windows si pero no tiene demasiada logica, 
	//probar cambiandolo
	//return(mVerticalVelocity == 0 && mVerticalOffset == 0);
	return mGrounded;	

}

void CharacterControllerPhysics::setWalkDirection(const btVector3 & walkDirection)
{
	mUseWalkDirection = true;
	mWalkDirection = walkDirection;
	mNormalizedDirection = getNormalizedVector(mWalkDirection);
}

void CharacterControllerPhysics::setWalkDirection(const btScalar x, const btScalar y, const btScalar z)
{
	mUseWalkDirection = true;
	mWalkDirection.setValue(x, y, z);
	mNormalizedDirection = getNormalizedVector(mWalkDirection);
}

btVector3 CharacterControllerPhysics::getWalkDirection() const
{
	return mWalkDirection;
}

btVector3 CharacterControllerPhysics::getPosition() const
{
	return mCurrentPosition;
}

btScalar CharacterControllerPhysics::getVerticalVelocity() const{
	return mVerticalVelocity;
}
void CharacterControllerPhysics::setOrientation(const btQuaternion & orientation)
{
	btTransform xform;
	xform = mGhostObject->getWorldTransform();
	xform.setRotation(orientation);
	mGhostObject->setWorldTransform(xform);
}

void CharacterControllerPhysics::updateAction(btCollisionWorld * collisionWorld, btScalar dt)
{
	preStep(collisionWorld);
	playerStep(collisionWorld, dt);
}

void CharacterControllerPhysics::reset(){

	mGrounded = false;
	mVerticalVelocity = 0.0;
    mVerticalOffset = 0.0;
    mWasOnGround = false;
    mWasJumping = false;
    mWalkDirection.setValue(0,0,0);
    mVelocityTimeInterval = 0.0;

    //clear pair cache
    btHashedOverlappingPairCache *cache = mGhostObject->getOverlappingPairCache();
    while (cache->getOverlappingPairArray().size() > 0)
    {
            cache->removeOverlappingPair(cache->getOverlappingPairArray()[0].m_pProxy0, cache->getOverlappingPairArray()[0].m_pProxy1, 
				mCollisionWorld->getDispatcher());
    }
}
