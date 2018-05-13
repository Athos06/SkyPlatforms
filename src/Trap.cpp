#include "Trap.h"
#include "PhysicsManager.h"
#include "Constants.h"

std::list<Trap*> Trap::m_trapsList;
std::vector<std::list<Trap*>::iterator> Trap::m_deletionList;

void Trap::updateEach(double deltaTime){
	if(!m_deletionList.empty()){
		for(int i = 0; i < m_deletionList.size(); i++){
			m_trapsList.erase(m_deletionList[i]);
		}
		m_deletionList.clear();
	}
	
	if(m_trapsList.empty())
		return;

	std::list<Trap*>::iterator aux;
	for(aux = m_trapsList.begin();
		aux != m_trapsList.end(); aux++){
		(*aux)->update(deltaTime);
	}
	
}

void Trap::destroyAllTraps(){
	m_deletionList.clear();

	if(m_trapsList.empty())
		return;

	std::list<Trap*>::iterator aux;
	for(aux = m_trapsList.begin();
		aux != m_trapsList.end(); aux++){
		delete (*aux);
	}
	m_trapsList.clear();	
}

Trap::Trap(){
	m_pNode = 0;
	tag = "trap";
	m_pRigidbody = 0;
}


Trap::~Trap(){
	m_pRigidbody = 0;
}


