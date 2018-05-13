#include "Collectible.h"

std::list<Collectible*> Collectible::m_collectiblesList;

Collectible::Collectible(){
	m_pNode = 0;
	tag = "Collectible";	
	m_pRigidbody = 0;
}

Collectible::~Collectible(){

}

void Collectible::destroyAll(){
	if(m_collectiblesList.empty())
		return;

	std::list<Collectible*>::iterator aux;
	for(aux = m_collectiblesList.begin();
		aux != m_collectiblesList.end(); aux++){
		delete (*aux);
	}
	m_collectiblesList.clear();
	//m_deletionList.clear();
}
