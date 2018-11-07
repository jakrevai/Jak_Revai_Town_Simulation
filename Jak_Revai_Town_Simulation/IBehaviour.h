////////////////////////////////////////////////////////////
// Author: <Jak Revai>                                                              
// Project: <Town Simulation Assignment with AIE bootstrap>
////////////////////////////////////////////////////////////
#pragma once
//#include "Agent.h"

class Agent;

class IBehaviour
{
public:
	IBehaviour() {};//Constructor
	virtual ~IBehaviour() {} //Destructor

	virtual void Update(Agent* pAgent, int &HungerStorage, int &ThirstStorage, float deltaTime) = 0;
};