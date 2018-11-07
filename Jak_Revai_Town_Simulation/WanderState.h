////////////////////////////////////////////////////////////
// Author: <Jak Revai>                                                              
// Project: <Town Simulation Assignment with AIE bootstrap>
////////////////////////////////////////////////////////////
#pragma once
#include "State.h"
class WanderState :	public State
{
public:
	WanderState();
	~WanderState();

	// Inherited via State
	virtual void OnEnter(Agent * agent);
	virtual void Update(Agent* agent, StateMachine* stateMachine, int &HungerStorage, int &ThirstStorage, float deltaTime);
	virtual void OnExit(Agent * agent);
};

