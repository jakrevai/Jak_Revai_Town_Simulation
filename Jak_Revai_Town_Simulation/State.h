////////////////////////////////////////////////////////////
// Author: <Jak Revai>                                                              
// Project: <Town Simulation Assignment with AIE bootstrap>
////////////////////////////////////////////////////////////
#pragma once
#include "StateMachine.h"

class State
{
public:
	State();

	virtual void OnEnter(Agent* agent) = 0;						   //Couldnt think of any other way to access the global variables that each agent pulls from,
																   //so im passing these in from SimulationApp
	virtual void Update(Agent* agent, StateMachine* stateMachine, int& HungerStorage, int& ThirstStorage, float deltaTime) = 0;
	virtual void OnExit(Agent* agent) = 0;

	~State();
};