#include "Effect.h"

std::list<Effect*> Effect::m_effectsList;
std::vector<std::list<Effect*>::iterator>  Effect::m_deletionList;


Effect::Effect(){
}

Effect::~Effect(){
}

void Effect::updateEach(double deltaTime){

	
	//elimina de la lista los efectos destruidos en el anterior update
	if(!m_deletionList.empty()){
		for(int i = 0; i < m_deletionList.size(); i++){
			m_effectsList.erase(m_deletionList[i]);
		}
		m_deletionList.clear();
	}

	if(m_effectsList.empty())
		return;
	
	std::list<Effect*>::iterator aux;
	for(aux = m_effectsList.begin(); 
		aux != m_effectsList.end();	aux++){
			(*aux)->update(deltaTime);
	}
	
}

void Effect::destroyAllEffects(){
	
	if(m_effectsList.empty())
		return;
	//destruye todos los efectos activos
	std::list<Effect*>::iterator aux;
	for(aux = m_effectsList.begin(); 
		aux != m_effectsList.end();	aux++){
		delete (*aux);
	}

	m_effectsList.clear();
	m_deletionList.clear();
	 
}
