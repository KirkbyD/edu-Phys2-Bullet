#pragma once
#include "cError.hpp"
#include "iMediatorInterface.hpp"
#include "cAIBehaviours.hpp"

class cAIManager {
	cAIManager();
	static cAIManager stonAIMngr;
	iMediatorInterface* pMediator;

	cAIBehaviours ai_behaviours;
	cError error;


public:
	~cAIManager() { }
	
	// SINGLETON FUNCTIONS
	static cAIManager* GetAIManager();

	// MEDIATOR FUNCTIONS
	void setMediatorPointer(iMediatorInterface* pMediator);
	virtual sData RecieveMessage(sData& data);

	// FUNCTIONS
	void update(sData& data);
};
