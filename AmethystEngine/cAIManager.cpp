#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>
#include <memory>

#ifdef _DEBUG
#define DEBUG_NEW new (_NORMAL_BLOCK , __FILE__ , __LINE__)
#else
#define DBG_NEW
#endif

#include "cAIManager.hpp"
#include "nAI.hpp"
#include <iostream>



#pragma region SINGLETON
cAIManager cAIManager::stonAIMngr;
cAIManager* cAIManager::GetAIManager() { return &(cAIManager::stonAIMngr); }
cAIManager::cAIManager()
	: ai_behaviours(), pMediator(nullptr)
{ std::cout << "AI Manager Created" << std::endl; }
#pragma endregion



#pragma region MEDIATOR SETUP
void cAIManager::setMediatorPointer(iMediatorInterface* pMediator) { this->pMediator = pMediator; }
#pragma endregion



#pragma region MEDIATOR COMMUNICATIONS
sData cAIManager::RecieveMessage(sData& data) {
	data.setResult(OK);
	return data;
}
#pragma endregion



#pragma region AI BEHAVIOURS UPDATE
void cAIManager::update(sData& data) {
	// change this such that it passes through only what is needed to the AI Behaviours
	// before this switch sends data to the AI Behaviours it needs to determine if the behaviour in question can be enacted...
	switch (data.getSourceGameObj()->GetActor()->getActiveBehaviour()) {
	case nAI::eAIBehviours::IDLE:
		// check physics to make sure the AI can be enacted...
		ai_behaviours.idle(data);
		break;

	case nAI::eAIBehviours::SEEK:
		ai_behaviours.seek(data);
		break;

	case nAI::eAIBehviours::APPROACH:
		ai_behaviours.approach(data);
		break;

	case nAI::eAIBehviours::PURSURE:
		ai_behaviours.pursure(data);
		break;

	case nAI::eAIBehviours::FLEE:
		ai_behaviours.flee(data);
		break;

	case nAI::eAIBehviours::EVADE:
		ai_behaviours.evade(data);
		break;

	case nAI::eAIBehviours::WANDER:
		ai_behaviours.wander(data);
		break;

	case nAI::eAIBehviours::FOLLOW_PATH:
		// TODO: Implement for second project
		break;

	default:
		break;
	}
}
#pragma endregion
