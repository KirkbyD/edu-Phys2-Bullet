#pragma once
#include "sData.hpp"
extern struct sData;

class cAIBehaviours {
public:
	void seek(sData& data);
	void pursure(sData& data);
	void flee(sData& data);
	void approach(sData& data);
	void evade(sData& data);
	void wander(sData& data);
	void idle(sData& data);
	void followPath(void); // needs to take in way points or nodes
};
