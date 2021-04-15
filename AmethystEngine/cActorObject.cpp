#include "cActorObject.hpp"

cActorObject::cActorObject(std::vector<cActorObject*>& vActors) {
	this->friendlyIDNumber = 0;
	this->friendlyName = "";

	this->isVisible = false;
	this->isControl = false;

	this->m_uniqueID = next_uniqueID;
	++cActorObject::next_uniqueID;

	this->parentObject = nullptr;

	// DEFAULT = FALSE
	_is_player = false;

	// DEFAULT = IDLE
	_activeBehaviour = nAI::eAIBehviours::IDLE;
	_enemy_type = nDecisionTree::eEnemyType::NONE;

	vActors.push_back(this);
}

cActorObject::cActorObject(std::vector<cActorObject*>& vActors, std::string actorName) {
	this->friendlyIDNumber = 0;
	this->friendlyName = actorName;

	this->isVisible = true;
	this->isControl = false;

	this->m_uniqueID = next_uniqueID;
	++cActorObject::next_uniqueID;

	this->parentObject = nullptr;

	// DEFAULT = entity
	this->_entityName = "entity";

	// DEFAULT = false
	_is_player = false;

	// DEFAULT = IDLE
	_activeBehaviour = nAI::eAIBehviours::IDLE;
	_enemy_type = nDecisionTree::eEnemyType::NONE;

	vActors.push_back(this);
}

void cActorObject::setEntityName(std::string name) {
	this->_entityName = name;
}

std::string cActorObject::getEntityName() {
	return this->_entityName;
}

void cActorObject::setActorName(std::string name) {
	this->_actorName = name;
}


nDecisionTree::eEnemyType cActorObject::getEnemyType() { return _enemy_type; }
void cActorObject::setEnemyType(nDecisionTree::eEnemyType enemyType) { this->_enemy_type = enemyType; }

void cActorObject::switchPlayerControlled() { this->_is_player = !this->_is_player; }
bool cActorObject::isPlayerControlled() { return this->_is_player; }



nAI::eAIBehviours cActorObject::getActiveBehaviour() { return _activeBehaviour; }
void cActorObject::setActiveBehaviour(nAI::eAIBehviours behaviour_to_set) { this->_activeBehaviour = behaviour_to_set; }

std::vector<nDecisionTree::cDecision*> cActorObject::GetDecisionTree() { return _vecActorDecisionTree; }
void cActorObject::AddDecision(nDecisionTree::cDecision* decision) { _vecActorDecisionTree.push_back(decision); }
void cActorObject::SetDecision(nDecisionTree::cDecision decision, int pos) { _vecActorDecisionTree[pos] = &decision; }

void cActorObject::setDecisionTreebehaviour(nDecisionTree::eIdentifier identifier, nDecisionTree::eComparater comparater, unsigned priority, nAI::eAIBehviours behaviour, size_t data) {
	AddDecision(new nDecisionTree::cDecision());
	size_t index = _vecActorDecisionTree.size() - 1;
	_vecActorDecisionTree[index]->behaviour = behaviour;
	_vecActorDecisionTree[index]->identifier = identifier;
	_vecActorDecisionTree[index]->comparater = comparater;
	_vecActorDecisionTree[index]->priority = priority;
	_vecActorDecisionTree[index]->data = data;
}

void cActorObject::setDecisionTreebehaviour(nDecisionTree::eIdentifier identifier, nDecisionTree::eComparater comparater, unsigned priority, nAI::eAIBehviours behaviour, glm::vec4 data) {
	AddDecision(new nDecisionTree::cDecision());
	size_t index = _vecActorDecisionTree.size() - 1;
	_vecActorDecisionTree[index]->behaviour = behaviour;
	_vecActorDecisionTree[index]->identifier = identifier;
	_vecActorDecisionTree[index]->comparater = comparater;
	_vecActorDecisionTree[index]->priority = priority;
	_vecActorDecisionTree[index]->v4Data = data;
}

void cActorObject::setDecisionTreebehaviour(nDecisionTree::eIdentifier identifier, nDecisionTree::eComparater comparater, unsigned priority, nAI::eAIBehviours behaviour, glm::vec3 data) {
	AddDecision(new nDecisionTree::cDecision());
	size_t index = _vecActorDecisionTree.size() - 1;
	_vecActorDecisionTree[index]->behaviour = behaviour;
	_vecActorDecisionTree[index]->identifier = identifier;
	_vecActorDecisionTree[index]->comparater = comparater;
	_vecActorDecisionTree[index]->priority = priority;
	_vecActorDecisionTree[index]->v4Data = glm::vec4(data, 1.0f);
}

void cActorObject::setDecisionTreebehaviour(nDecisionTree::eIdentifier identifier, nDecisionTree::eComparater comparater, unsigned priority, nAI::eAIBehviours behaviour, glm::vec2 data) {
	AddDecision(new nDecisionTree::cDecision());
	size_t index = _vecActorDecisionTree.size() - 1;
	_vecActorDecisionTree[index]->behaviour = behaviour;
	_vecActorDecisionTree[index]->identifier = identifier;
	_vecActorDecisionTree[index]->comparater = comparater;
	_vecActorDecisionTree[index]->priority = priority;
	_vecActorDecisionTree[index]->v4Data = glm::vec4(data, 0.0f, 1.0f);
}

void cActorObject::setDecisionTreebehaviour(nDecisionTree::eIdentifier identifier, nDecisionTree::eComparater comparater, unsigned priority, nAI::eAIBehviours behaviour, float data) {
	AddDecision(new nDecisionTree::cDecision());
	size_t index = _vecActorDecisionTree.size() - 1;
	_vecActorDecisionTree[index]->behaviour = behaviour;
	_vecActorDecisionTree[index]->identifier = identifier;
	_vecActorDecisionTree[index]->comparater = comparater;
	_vecActorDecisionTree[index]->priority = priority;
	_vecActorDecisionTree[index]->fData= data;
}

void cActorObject::setDecisionTreebehaviour(nDecisionTree::eIdentifier identifier, nDecisionTree::eComparater comparater, unsigned priority, nAI::eAIBehviours behaviour, int data) {
	AddDecision(new nDecisionTree::cDecision());
	size_t index = _vecActorDecisionTree.size() - 1;
	_vecActorDecisionTree[index]->behaviour = behaviour;
	_vecActorDecisionTree[index]->identifier = identifier;
	_vecActorDecisionTree[index]->comparater = comparater;
	_vecActorDecisionTree[index]->priority = priority;
	_vecActorDecisionTree[index]->iData = data;
}

void cActorObject::setDecisionTreebehaviour(nDecisionTree::eIdentifier identifier, nDecisionTree::eComparater comparater, unsigned priority, nAI::eAIBehviours behaviour, bool data) {
	AddDecision(new nDecisionTree::cDecision());
	size_t index = _vecActorDecisionTree.size() - 1;
	_vecActorDecisionTree[index]->behaviour = behaviour;
	_vecActorDecisionTree[index]->identifier = identifier;
	_vecActorDecisionTree[index]->comparater = comparater;
	_vecActorDecisionTree[index]->priority = priority;
	_vecActorDecisionTree[index]->bData = data;
}

unsigned int cActorObject::getUniqueID(void) { return this->m_uniqueID; }
glm::vec3 cActorObject::getPosition(void) { return glm::vec3(0.0f, 0.0f, 0.0f); }		// NOT USED...
