#include "PhysicsManager.h"
#include "AdvancedOgreFramework.h"
#include "AppStateManager.h"
#include "BtOgrePG.h"
#include "BtOgreGP.h"
#include "BtOgreExtras.h"
#include "btBulletDynamicsCommon.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"
#include "Constants.h"
#include "GameObject.h"
#include "GameManager.h"
#include "HUD.h"
#include "Player.h"
#include "Skystone.h"
#include "Portal.h"
#include "CheckPoint.h"
#include "Proyectile.h"
#include "Switch.h"
#include "Boss.h"
#include "LevelsManager.h"

template<> PhysicsManager* Ogre::Singleton<PhysicsManager>::msSingleton = 0;

PhysicsManager& PhysicsManager::getSingleton(){
	assert (msSingleton);
	return (*msSingleton);
}

PhysicsManager* PhysicsManager::getSingletonPtr(){
	assert(msSingleton);
	return msSingleton;
}


PhysicsManager::PhysicsManager(){

	m_pWorld			= 0;	
	m_pBroadPhase		= 0;
	mDispatcher			= 0;
	mSolver				= 0;
	mColConfig			= 0;
	m_pSceneMgr			= 0;
}


PhysicsManager::~PhysicsManager(){
	m_pairsLastUpdate.clear();
	
	// delete - RigidBodies
	std::deque<btRigidBody *>::iterator itBody 
		= m_Bodies.begin();
	while (m_Bodies.end() != itBody)
	{   
		delete *itBody;
		++itBody;
	}   
	// delete - Shapes
	std::deque<btCollisionShape *>::iterator itShape = m_Shapes.begin();
	while (m_Shapes.end() != itShape)
	{   
		delete *itShape;
		++itShape;
	}
	
	m_Bodies.clear();
	m_Shapes.clear();

	
	if(m_pWorld){
		//delete m_pWorld->getDebugDrawer();
		//m_pWorld->setDebugDrawer(0);
		delete m_pWorld;
	}
	if(mSolver) delete mSolver;
	if(m_pBroadPhase) delete m_pBroadPhase;
	if(mDispatcher) delete mDispatcher;
	if(mColConfig) delete mColConfig;

	m_pSceneMgr	= 0;
 
}


void PhysicsManager::destroyPhysics(){
	 
	m_pairsLastUpdate.clear();
	
	// delete - RigidBodies
	std::deque<btRigidBody *>::iterator itBody 
		= m_Bodies.begin();
	while (m_Bodies.end() != itBody)
	{   
		delete *itBody;
		++itBody;
	}
	// delete - Shapes
	std::deque<btCollisionShape *>::iterator itShape = m_Shapes.begin();
	while (m_Shapes.end() != itShape)
	{   
		delete *itShape;
		++itShape;
	}
	
	m_Bodies.clear();
	m_Shapes.clear();
	
	if(m_pWorld){
		//delete m_pWorld->getDebugDrawer();
		//m_pWorld->setDebugDrawer(0);
		delete m_pWorld;
	}
 
	
	if(mSolver) delete mSolver;
	if(m_pBroadPhase) delete m_pBroadPhase;
	if(mDispatcher) delete mDispatcher;
	if(mColConfig) delete mColConfig;

	mColConfig = 0;
	mDispatcher = 0;
	m_pBroadPhase = 0;
	mSolver = 0;
	m_pWorld = 0;

	m_GravityVector = Ogre::Vector3::ZERO;
	
	m_pSceneMgr	= 0;
	
	 
}



void PhysicsManager::initPhysics(Ogre::SceneManager* SceneMgr)
{
	m_pSceneMgr			= SceneMgr;

	m_GravityVector = Ogre::Vector3(0,-10,0);

	mColConfig = new btDefaultCollisionConfiguration();
	mDispatcher = new btCollisionDispatcher(mColConfig);
	m_pBroadPhase = new btDbvtBroadphase();
	mSolver = new btSequentialImpulseConstraintSolver();
	m_pWorld = new btDiscreteDynamicsWorld(mDispatcher, 
		m_pBroadPhase, mSolver, mColConfig);

	m_pWorld->setGravity(BtOgre::Convert::toBullet(m_GravityVector) );
	
	m_pWorld->getDispatchInfo().m_allowedCcdPenetration = 0.00001f;

	m_pWorld->getPairCache()->setInternalGhostPairCallback
		(new btGhostPairCallback());
}



void PhysicsManager::update(double deltaTime){
	if(m_pWorld){
		m_pWorld->stepSimulation(deltaTime, 2, btScalar(34*0.001));
		CollisionsDetection();
	}
}



btRigidBody * PhysicsManager::createPhysicObject(Ogre::SceneNode *pNode, 
			int physicType,		//static, kinematic, dynamic
			int colliderType,  //primitive (cube, sphere..) o mesh
			Ogre::Entity* pEnt = 0,     //se pasa en el caso de que haya que convertir la mesh en una collision shape
			Ogre::String meshName = "",		//el nombre de la mesh por si la convertimos a collision shape
            bool ghost = false,
			const float bodyRestitution = 0,
            const float bodyFriction = 0,
            const float bodyMass = 0,
            const Ogre::Vector3 &pos = Ogre::Vector3::ZERO, 
            const Ogre::Quaternion &quat = Ogre::Quaternion::IDENTITY,
			bool useConvexConverter = false){

	
	Ogre::Vector3 position = pNode->_getDerivedPosition();
	Ogre::Vector3 scale = pNode->_getDerivedScale();
	Ogre::Quaternion rotation = pNode->_getDerivedOrientation();

	btCollisionShape* collShape;
	
	if(colliderType == COLLISION_MESH){
		if(pEnt){
		}
		else{
			Ogre::Entity* provisionalEnt = m_pSceneMgr->createEntity
					("provisionalPhysicsEntity", meshName);

			if(useConvexConverter){	
				BtOgre::StaticMeshToShapeConverter converter(provisionalEnt);
				collShape = converter.createConvex();
			}
			else{
				BtOgre::StaticMeshToShapeConverter converter(provisionalEnt);
				collShape = converter.createTrimesh();
				
			}
			//esta entidad solo la usamos para crear la collisionShape
			//una vez creada la entidad se puede eliminar pues no se usa
			m_pSceneMgr->destroyEntity(provisionalEnt);

			//escala la collision shape que conseguimos a partir de la mesh
			//para que tenga la misma escala que la entidad
			collShape->setLocalScaling(BtOgre::Convert::toBullet(scale));
		}
	}
	if(colliderType == PRIMITIVE_BOX){
		/*
		esto esta hardcodeado pues se trata mas bien de una utilidad 
		para crear colliders de limites y triggers. Utilizo las dimensiones
		del cubo por defecto en blender, y asi puedo utilizar el propio blender
		para editar graficamente estos colliders, simplemente lo multiplicare por
		la escala del nodo y tengo la misma representacion de lo que veo en blender.
		Es un workaround mas que algo estable, pero cumple con lo que necesito
		*/ 
		Ogre::Vector3 min(-1,-1,-1);
		Ogre::Vector3 max(1,1,1);
		Ogre::AxisAlignedBox boundingBox(min, max);
		Ogre::Vector3 size = boundingBox.getSize(); 
		size /= 2.0f;
	
		size = Ogre::Vector3(size.x * scale.x, size.y * scale.y, size.z * scale.z);
	
		collShape = new btBoxShape(BtOgre::Convert::toBullet(size));

	}	
	if(colliderType == PRIMITIVE_SPHERE){
		/*
		si tiene como collision shape una esfear simplemente cogemos un valor 
		del escalado del nodo y lo usamos como radio (en blender se usa una 
		icoesfera de tamano unitario que escalamos y depues exportamos, de 
		ahi que se pueda coger el valor del escalado y obtenemos la representacion
		deseada)*/ 
		collShape = new btSphereShape(scale.x);		
	}	

	//se crea el motion state que es el mismo independientemente del tipo de objeto
	BtOgre::RigidBodyState* motionState = new BtOgre::RigidBodyState
		(pNode, btTransform(BtOgre::Convert::toBullet(rotation),
		BtOgre::Convert::toBullet(position)) );

	//declaramos el rigidbody aunque  aunque no lo inicializamos pues depende del tipo de objeto 
	btRigidBody *rigidbody;

	//si es estatico le damos masa = 0
	if(physicType == STATIC){
		//creamos el rigidbody
		btRigidBody::btRigidBodyConstructionInfo rigidbodyCI(
		0,
		motionState,
		collShape,
		btVector3(0,0,0)
		);	
		
		rigidbody = new btRigidBody(rigidbodyCI);
		//rigidbodyRampa->setActivationState(DISABLE_DEACTIVATION);
		//rigidbodyRampa->setRestitution(0);
		//rigidbodyRampa->setFriction(0);
		rigidbody->setCollisionFlags(btCollisionObject::CF_STATIC_OBJECT);	
	}
	//si es kinematico
	if(physicType == KINEMATIC){

		BtOgre::RigidBodyState* motionStateDyn = new BtOgre::RigidBodyState
			(pNode, btTransform(BtOgre::Convert::toBullet(Ogre::Quaternion::IDENTITY),
		BtOgre::Convert::toBullet(position)) );

		btRigidBody::btRigidBodyConstructionInfo rigidbodyCI(
		0,
		motionStateDyn,
		collShape,
		btVector3(0,0,0)
		);	
		
		rigidbody = new btRigidBody(rigidbodyCI);
		rigidbody->setFriction(0);
		rigidbody->setRestitution(0);
		//le ponemos las flags de kinematics
		rigidbody->setActivationState(DISABLE_DEACTIVATION);
		rigidbody->setCollisionFlags(rigidbody->getCollisionFlags() |  btCollisionObject::CF_KINEMATIC_OBJECT);
	}

	if(physicType == DYNAMIC){
		btRigidBody::btRigidBodyConstructionInfo rigidbodyCI(
		bodyMass,
		motionState,
		collShape,
		btVector3(0,0,0)
		);	
		
		rigidbody = new btRigidBody(rigidbodyCI);
		rigidbody->setFriction(bodyFriction);
		rigidbody->setRestitution(bodyRestitution);
	}

	
	
	//si es ghost (un trigger) le ponemos las flags correspondientes
	if(ghost){
		rigidbody->setCollisionFlags(rigidbody->getCollisionFlags() | 
			btCollisionObject::CF_NO_CONTACT_RESPONSE);
		rigidbody->setActivationState(DISABLE_DEACTIVATION);
		
	}
	//anadimos el objeto al mundo de bullet
	m_pWorld->addRigidBody(rigidbody);
	           
	// push the created object to the deque
	m_Shapes.push_back(collShape);
	m_NumEntitiesInstanced++;	

	return rigidbody;
}


//elimina del mundo y destruye el rigidbody
void PhysicsManager::destroyPhysicObject(btRigidBody *rb){
	m_pWorld->removeRigidBody(rb);
	delete rb->getMotionState();
	delete rb;
	
}

void PhysicsManager::CollisionsDetection(){

	// keep a list of the collision pairs we
	// found during the current update
	//CollisionPairs m_pairsThisUpdate;
	m_pairsThisUpdate.clear();

	if(m_pWorld == 0)
		return;

	// iterate through all of the manifolds in the dispatcher
	for (int i = 0; i < m_pWorld->getDispatcher()->getNumManifolds(); ++i) {
		
		// get the manifold
		btPersistentManifold* pManifold = m_pWorld->getDispatcher()->getManifoldByIndexInternal(i);
		
		// ignore manifolds that have 
		// no contact points.
		if (pManifold->getNumContacts() > 0) {
			// get the two rigid bodies involved in the collision
			btCollisionObject* obA = const_cast<btCollisionObject*>(pManifold->getBody0());
			btCollisionObject* obB = const_cast<btCollisionObject*>(pManifold->getBody1());
			//dice que...la parte contratante de la primera parte sera considerada como la parte
			//contratante de la primera parte, y la parte contratante de la segunda parte
			//sera considerada como la parte contratante de la segunda parte
			bool const swapped = obA > obB; // always create the pair in a predictable order (use the pointer value..)
			const btCollisionObject* pSortedObA = swapped ? obB : obA;
			const btCollisionObject* pSortedObB = swapped ? obA : obB;

			// create the pair
			CollisionPair thisPair = std::make_pair(pSortedObA, pSortedObB);
			// insert the pair into the current list
			m_pairsThisUpdate.insert(thisPair);
			std::set<CollisionPair> CollisionPairs2;
			// if this pair doesn't exist in the list
			// from the previous update, it is a new
			// pair and we must send a collision event
			if (m_pairsLastUpdate.find(thisPair) == m_pairsLastUpdate.end()){
				m_pairsLastUpdate.insert(thisPair);
				CollisionEvent((btCollisionObject*)obA, (btCollisionObject*)obB);
			}
		}
	}
	
	// create another list for pairs that
	// were removed this update
	CollisionPairs removedPairs;
	// this handy function gets the difference beween
	// two sets. It takes the difference between
	// collision pairs from the last update, and this 
	// update and pushes them into the removed pairs list
	std::set_difference( m_pairsLastUpdate.begin(), m_pairsLastUpdate.end(),
	m_pairsThisUpdate.begin(), m_pairsThisUpdate.end(),
	std::inserter(removedPairs, removedPairs.begin()));
	// iterate through all of the removed pairs
	// sending separation events for them
	for (CollisionPairs::const_iterator iter = removedPairs.begin(); 
	iter != removedPairs.end(); ++iter) {
		SeparationEvent((btCollisionObject*)iter->first, (btCollisionObject*)iter->second);
	}
	// in the next iteration we'll want to
	// compare against the pairs we found
	// in this iteration
	m_pairsLastUpdate = m_pairsThisUpdate;	
}



void PhysicsManager::CollisionEvent(btCollisionObject* pObA, btCollisionObject* pObB) {
	//Para poder filtrar las colisiones y poder saber que tipo de objetos estan 
	//involucrados y actuar dependiendo de ello se anade un puntero al GameObject que
	//queremos asociar al objeto fisico. Cada GameObject dispone de una etiqueta "tag"
	//que utilizamos para averiguar de que tipo de objeto se trata
	//tambien puede darse el caso de que haya un objeto estatico que no tiene un gameObject
	//asociado, asi que tenemo en cuenta ambos casos
	if(pObA->getUserPointer() || pObB->getUserPointer()){	
		GameObject* goA = 0;
		GameObject* goB = 0;
		if(pObA->getUserPointer())	
			goA = (GameObject*)pObA->getUserPointer();
		if(pObB->getUserPointer())	
			goB = (GameObject*)pObB->getUserPointer();
		
		#pragma region skystones
		//detectar si el player colisiona con las skystones
		if ( (goA && goB) && 
		((goA->getTag() == "skystone") || (goB->getTag() == "skystone")) ){		
			//para tener siempre el puntero al objeto que estamos comprobando y el del objeto 
			//con el que colisiona en un orden establecido para ahorrar
			//problemas de comprobaciones extra para ver que puntero apunta 
			//a uno u otro
			GameObject* sortedGoA;
			GameObject* sortedGoB;
			if(goA->getTag() == "skystone"){
				sortedGoA = goA;
				sortedGoB = goB;
			}
			else{
				sortedGoA = goB;
				sortedGoB = goA;
			}	
			//solo queremos detectar la colision con el player
			if(! (sortedGoB->getTag() == "player") )
				return;
			//este tipo de colision se trata de un caso especial porque cuando se produce
			//la colision se destruye inmediatamente despues el objeto
			//Esto se detecta como un fin de la colision pero pasa el puntero eliminado
			//produciendose violacion de segmento.
			//hay que acceder a la collisionpair guardada en la lista
			// y eliminarla antes de enviar el evento
			bool const swapped = pObA > pObB; 
			const btCollisionObject* pSortedObA = swapped ?  pObB :  pObA;
			const btCollisionObject* pSortedObB = swapped ?  pObA :  pObB;
			
			CollisionPair thisPair = std::make_pair(pSortedObA, pSortedObB);
			//aqui buscamos la pareja de la colision y la eliminamos de la lista para
			//que no produzca un evento de separacion como fin de colision
			CollisionPairs::iterator iter = m_pairsLastUpdate.find(thisPair);
			if(!(iter == m_pairsLastUpdate.end()) ){ 
				m_pairsLastUpdate.erase(iter);
			}
			iter = m_pairsThisUpdate.find(thisPair);
			if(!(iter == m_pairsThisUpdate.end()) ){ 
				m_pairsThisUpdate.erase(iter);
			}
			
			Player* player = (Player*)sortedGoB;
			//recogemos la sksytone y la destruimos
			player->pickUpStone();

			Skystone* skystone = (Skystone*)sortedGoA;	
			skystone->destroy();
			delete skystone;
			
			//importante este return pues al haber sido destruido el collisionObject
			//provacaria errores en las siguientes comprobaciones
			return;
							
		}
		#pragma endregion
		//detectar colisiones de los proyectiles
		#pragma region Proyectiles
		if((goA && goA->getTag() == "proyectile") 
			|| (goB && goB->getTag() == "proyectile") ){
			GameObject* sortedGoA = 0;
			GameObject* sortedGoB = 0;
			if(goA && (goA->getTag() == "proyectile") ){

				sortedGoA = goA;
				sortedGoB = goB;
			}
			else{
				sortedGoA = goB;
				sortedGoB = goA;
			}
			//mismo caso que el anterior con las skystones, 
			//para que no se produzca evento de separacion
			//debo eliminarlo de la lista
			bool const swapped = pObA > pObB;
			const btCollisionObject* pSortedObA = swapped ?  pObB :  pObA;
			const btCollisionObject* pSortedObB = swapped ?  pObA :  pObB;
			
			CollisionPair thisPair = std::make_pair(pSortedObA, pSortedObB);

			CollisionPairs::iterator iter = m_pairsLastUpdate.find(thisPair);
			if(!(iter == m_pairsLastUpdate.end()) ){ 
				m_pairsLastUpdate.erase(iter);
			}
			iter = m_pairsThisUpdate.find(thisPair);
			if(!(iter == m_pairsThisUpdate.end()) ){ 
				m_pairsThisUpdate.erase(iter);
			}

			Proyectile* proyectile;
			//cuando hay colision con el collider del fondo
			//se destruye el proyectil para que no caiga al vacio
			//infinitamente
			if(sortedGoB && (sortedGoB->getTag() == "deathTrigger") ) {				
				proyectile = (Proyectile*)sortedGoA;
				proyectile->destroy();
				//delete proyectile;
				return;
			}
			//que no se destruya al detectar colision con un trigger,
			//simplemente la colision es ignorada, solo la excepcion 
			//del caso de arriba con el deathtrigger
			//(la comprobacion se hace con el puntero del collision object para ver si
			//se trata de un objeto ghost (trigger) pero el propio proyectil es un objeto 
			//ghost, por lo que tenemos eso en cuenta)
			if( (goA && (goA->getTag() == "proyectile" && !pObB->hasContactResponse()))
				|| (goB && (goB->getTag() == "proyectile" && !pObA->hasContactResponse())) )
				return;

			Player* player;
			//si hay colision con el personaje le da un pinazo 
			//que lo deja tieso
			if(sortedGoB && (sortedGoB->getTag() == "player") ){
				player = (Player*)sortedGoB;
				if(!player->isDead())
					player->die();
			}
			//cualquier otra colision con un objeto fisico
			//destruye el proyectil
			proyectile = (Proyectile*)sortedGoA;
			proyectile->destroy();
			//delete proyectile;
			return;
		}
		
#pragma endregion
		#pragma region Boss
		//colision con el collider del boss, la barrera de fuego
		if ( (goA && goB) && ((goA->getTag() == "boss") || (goB->getTag() == "boss")) ){
			GameObject* sortedGoA;
			GameObject* sortedGoB;
			if(goA->getTag() == "boss"){
				sortedGoA = goA;
				sortedGoB = goB;
			}
			else{
				sortedGoA = goB;
				sortedGoB = goA;
			}
			//solo queremos detectar la colision con el player
			if(! (sortedGoB->getTag() == "player") )
				return;
			Boss* boss = (Boss*)sortedGoA;
			if(!(boss->isDead()) ){
				Player* player = (Player*)sortedGoB;
				if(!player->isDead())
					player->die();
			}
			return;
		}
		#pragma endregion

		#pragma region checkpoint
		//colision con el collider de un checkpoint
		if ( (goA && goB) &&  ((goA->getTag() == "checkpoint") || (goB->getTag() == "checkpoint")) ){
			GameObject* sortedGoA;
			GameObject* sortedGoB;
			if(goA->getTag() == "checkpoint"){
				sortedGoA = goA;
				sortedGoB = goB;
			}
			else{
				sortedGoA = goB;
				sortedGoB = goA;
			}

			if( sortedGoB->getTag() != "player")
				return;

			CheckPoint* checkpoint = (CheckPoint*)sortedGoA;
			//cuando entramos en un checkpoint accedemos a su spawnPoint
			//y lo guardamos para ser utilizado en la proxima reaparicion
			GameManager::getSingletonPtr()->setActiveSpawnPoint
				(checkpoint->getSpawnPoint());
			return;
		}
		#pragma endregion
		#pragma region slasher
		//colision con slasher
		//al colisionar contra alguno de los obstaculos o trampas el personaje muere
		if ( (goA && goB) &&  ((goA->getTag() == "slasher") || (goB->getTag() == "slasher")) ){
			GameObject* sortedGoA;
			GameObject* sortedGoB;
			if(goA->getTag() == "slasher"){
				sortedGoA = goA;
				sortedGoB = goB;
			}
			else{
				sortedGoA = goB;
				sortedGoB = goA;
			}

			if(sortedGoB->getTag() != "player")
				return;

			Player* player = (Player*)sortedGoB;
			//mata al personaje
			if(!player->isDead())
				player->die();
			return;
		}
		#pragma endregion

		#pragma region varios
		//colision con masher
		if ( (goA && goB) &&  ((goA->getTag() == "masher") || (goB->getTag() == "masher")) ){
			GameObject* sortedGoA;
			GameObject* sortedGoB;
			if(goA->getTag() == "masher"){
				sortedGoA = goA;
				sortedGoB = goB;
			}
			else{
				sortedGoA = goB;
				sortedGoB = goA;
			}

			if(sortedGoB->getTag() != "player")
				return;

			//mata al personaje
			Player* player = (Player*)sortedGoB;
			if(!player->isDead())
				player->die();
			return;
		}
		//colision con deathmill
		if ( (goA && goB) && ((goA->getTag() == "deathmill") || (goB->getTag() == "deathmill")) ){
			GameObject* sortedGoA;
			GameObject* sortedGoB;
			if(goA->getTag() == "deathmill"){
				sortedGoA = goA;
				sortedGoB = goB;
			}
			else{
				sortedGoA = goB;
				sortedGoB = goA;
			}

			if(sortedGoB->getTag() != "player")
				return;
			
			//mata al personaje
			Player* player = (Player*)sortedGoB;
			if(!player->isDead())
				player->die();
			return;
		}
		
		//colision con spikes
		if ( (goA && goB) &&  ((goA->getTag() == "spikes") || (goB->getTag() == "spikes")) ){
			GameObject* sortedGoA;
			GameObject* sortedGoB;
			if(goA->getTag() == "spikes"){
				sortedGoA = goA;
				sortedGoB = goB;
			}
			else{
				sortedGoA = goB;
				sortedGoB = goA;
			}
			if(sortedGoB->getTag() != "player")
				return;
			//mata al personaje
			Player* player = (Player*)sortedGoB;
			if(!player->isDead())
				player->die();
			return;
		}
		//colision con deathtrigger
		//el deathtrigger es el limite inferior del nivel, cuando cae al vacio el 
		//personaje entra en el lo que indica que ha muerto por caida
		if ( (goA && goB) && ((goA->getTag() == "deathTrigger") || (goB->getTag() == "spikes")) ){
			GameObject* sortedGoA;
			GameObject* sortedGoB;
			if(goA->getTag() == "deathTrigger"){
				sortedGoA = goA;
				sortedGoB = goB;
			}
			else{
				sortedGoA = goB;
				sortedGoB = goA;
			}

			if(sortedGoB->getTag() != "player")
				return;
			//el personaje muere
			Player* player = (Player*)sortedGoB;
			if(!player->isDead())
				player->die();
			return;
		}
		//colision con el portal
		if ((goA && goB) &&  ((goA->getTag() == "portal") || (goB->getTag() == "portal")) ){
			GameObject* sortedGoA;
			GameObject* sortedGoB;
			if(goA->getTag() == "portal"){
				sortedGoA = goA;
				sortedGoB = goB;
			}
			else{
				sortedGoA = goB;
				sortedGoB = goA;
			}

			if(sortedGoB->getTag() != "player")
				return;

			Player* player = (Player*)sortedGoB;
			Portal* portal = (Portal*)sortedGoA;
			
			//si el portal esta activo podemos completar el nivel
			if(portal->isActive() ){
				GameManager::getSingletonPtr()->completeLevel();
			}
			//si no esta activo mostramos el mensaje indicando que esta inactivo
			//para recordar al jugador que aun le faltan por recoger skystones
			else{
				HUD::getSingletonPtr()->showMsg("");
			}
			return;
			
		}
		//colision con el interruptor
		//los interruptores del jefe final que hay que activar para derrotarlo
		if ( (goA && goB) && ((goA->getTag() == "switch") || (goB->getTag() == "switch")) ){
			GameObject* sortedGoA;
			GameObject* sortedGoB;
			if(goA->getTag() == "switch"){
				sortedGoA = goA;
				sortedGoB = goB;
			}
			else{
				sortedGoA = goB;
				sortedGoB = goA;
			}

			if(sortedGoB->getTag() != "player")
				return;

			Switch* pSwitch = (Switch*)sortedGoA;
			//si no esta activado lo activamos. Si ya esta activado no hacemos nada
			if(!pSwitch->isActive() )
				pSwitch->setActive(true);
			return;
			
		}
		#pragma endregion
	}
}

void PhysicsManager::SeparationEvent(btCollisionObject* pObA, btCollisionObject* pObB) {	
	if(!pObA->getUserPointer() || !pObB->getUserPointer())
	{
		return;
	}
		GameObject* goA = (GameObject*)pObA->getUserPointer();
		GameObject* goB = (GameObject*)pObB->getUserPointer();

		//colision con el portal
		//cuando entramos en el portal y no esta activado mostramos el mensaje de aviso
		//al salid del portal y detectar el fin de la colision lo ocultamos
		if ( (goA->getTag() == "portal") && (goB->getTag() == "player")
			|| (goB->getTag() == "portal") && (goA->getTag() == "player")){
			Portal* portal = (Portal*)goA;

			HUD::getSingletonPtr()->hideMsg();

			return;
			
		}
}
