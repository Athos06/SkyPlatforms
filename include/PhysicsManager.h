#ifndef	PHYSICS_MANAGER_H
#define PHYSICS_MANAGER_H

#include <Ogre.h>		 
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

#include <set>
#include <iterator>
#include <algorithm>

// convenient typedefs for collision events
typedef std::pair<const btCollisionObject*, 
		const btCollisionObject*> CollisionPair;
typedef std::set<CollisionPair> CollisionPairs;

/**
 *  @author Jorge Rosique Contreras
 *  @date 11-6-2014
 *
 * La clase PhysicsManager se encarga de gestionar todo lo 
 * relacionado con las fisicas del juego, desde la creacion y destruccion
 * del mundo y objetos fisicos hasta la deteccion y gestion de colisiones
 */
class PhysicsManager :  public Ogre::Singleton<PhysicsManager>{
public:
	static PhysicsManager& getSingleton();
	static PhysicsManager* getSingletonPtr();

	PhysicsManager();
	~PhysicsManager();
	/**
	* @brief Inicializa un nuevo mundo fisico.
	* Llamado cada vez que se comienza un nuevo nivel
	* @param SceneMgr El scene manager de ogre que utilizamos en el nivel
	*/
	void initPhysics(Ogre::SceneManager* SceneMgr);
	/**
	 * @brief Finaliza y destruye el mundo fisico actual pero no destruye el manejador
	 * llamado cada vez que salimos de un nivel
	 */
	void destroyPhysics();
	/**
	* @brief llamado una vez en cada frame. Actualiza el mundo de Bullet
	* @param deltaTime tiempo transcurrido entre frames en milisegundos
	*/
	void update(double deltaTime);
	/**
	* @brief Es llamado una vez cada frame y se encarga de detectar y procesar las colisiones. 
	*/
	void CollisionsDetection();
	/**
	 * @brief Actua como factoria para todos los elementos fisicos
	 *del juego
	 * @param pNode node de ogre que utilizara el objeto fisico
	 * @param physicType tipo de objeto fisico (puede ser static, kinematic, dynamic)
	 * @param colliderType tipo de collider que utilizara el objeto fisico
	 * (puede ser una primitiva (cube, sphere) o utilizar una mesh que indiquemos)
	 * @param pEnt parametro opcional por si pasamos un entidad que contiene la 
	 * mesh que queremos utilizar para transformarla en maya de colision (por defecto no se usa)
	 * @param meshName podemos indicar el nombre de la mesh para cargar y usar como maya de colision
	 *(por defecto no se usa)
	 * @param ghost indica si se trata de un objeto ghost o no. Un objeto ghost es utilizado como trigger
	 * @param bodyRestitution
	 * @param bodyFriction
	 * @param bodyMass
	 * @param pos la posicion en la que se creara el objeto
	 * @param quat orientacion del objeto
	 * @param useConvexConverter indica que tipo de conversor utilizar para transformar un maya en 
	 * una collision shape. Si true utiliza el convex converter de btOgre, sino utiliza el trimesh converter.
	 * Ambos conversores tienen diferentes caracteristicas y el character controller se comporta de diferente manera
	 * sobre las collision shapes creadas por uno y otro,  se utiliza dependiendo de las necesidades del objeto.
	 * Por defecto false, utiliza el trimesh converter
	 * @return devuelve un puntero a el btRigidBody creado
	 */
	btRigidBody * createPhysicObject(Ogre::SceneNode *pNode, 
			int physicType,	
			int colliderType,
			Ogre::Entity* pEnt,    
			Ogre::String meshName,
			bool ghost,
            const float bodyRestitution,
            const float bodyFriction,
            const float bodyMass,
            const Ogre::Vector3 &pos, 
			const Ogre::Quaternion &quat,
			bool useOgreBulletConverter);
	/**
	* @brief se encarga de eliminar el rigidbody del mundo fisico de bullet
	* @param rb rigidbody a eliminar del mundo
	*/		
	void destroyPhysicObject(btRigidBody* rb);	
	/**
	 * @brief Es llamado por cada colision nueva que se detecta en el frame actual. Solo se consideran las 
	 * nuevas colisiones, si dos objetos ya estaban en contacto en el frame anterior no es llamado
	 * @param pObA puntero al primer btCollisionObject involucrado en la colision
	 * @param pObB puntero a el otro btCollisionObject involucrado en la colision
	 */
	virtual void CollisionEvent(btCollisionObject* pBody0, 
							btCollisionObject* pBody1);
	/**
	 * @brief Es llamado por cada evento de separacion que se detecta en el frame actual. 
	 * cuando dos CollisionObject dejan de estar en contacto se produce un evento de separacion.
	 * @param pObA puntero al primer btCollisionObject involucrado en la colision
	 * @param pObB puntero a el otro btCollisionObject involucrado en la colision
	 */		
	virtual void SeparationEvent(btCollisionObject* pBody0, 
										 btCollisionObject* pBody1);

	/**
	* @brief acceso al btDiscreteDynamicsWorld 
	* @return puntero al btDiscreteDynamicWorld en uso
	*/	
	btDiscreteDynamicsWorld * getDynamicsWorld() const{
		return m_pWorld;
	}
	/**
	* @brief acceso al btCollisionWorld
	* @return puntero al btCollisionWorld en uso
	*/	
	btCollisionWorld * getCollisionWorld() const{
		return m_pWorld->getCollisionWorld();
	}
	/**
	* @brief acceso a la btBroadphaseInterface  
	* @return puntero a la btBroadphaseInterface  en uso
	*/	
	btBroadphaseInterface * getBroadphase() const{
		return m_pBroadPhase;
	}
	
	//colas que albergan punteros a las collision shapes y rigidbodies 
	//utlizados, necesaris para poder eliminarlos ya que no se destruyen automaticamente 
	//al destruir el mundo de bullet
	std::deque<btRigidBody *>       m_Bodies;
 	std::deque<btCollisionShape *>  m_Shapes;
	
	int m_NumEntitiesInstanced; //para crear nombres unicos sin conflicto


private:
	btDiscreteDynamicsWorld *m_pWorld;	// Bullet World
	btBroadphaseInterface* m_pBroadPhase;
	btCollisionDispatcher *mDispatcher;
	btConstraintSolver *mSolver;
	btDefaultCollisionConfiguration *mColConfig;

	Ogre::SceneManager* m_pSceneMgr;
	Ogre::AxisAlignedBox m_WorldBounds; 
	Ogre::Vector3 m_GravityVector;

	// collision event variables
	CollisionPairs m_pairsLastUpdate;
	CollisionPairs m_pairsThisUpdate;


};
//|||||||||||||||||||||||||||||||||||||||||||||||

#endif
//|||||||||||||||||||||||||||||||||||||||||||||||