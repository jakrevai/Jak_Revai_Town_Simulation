////////////////////////////////////////////////////////////
// Author: <Jak Revai>                                                              
// Project: <Town Simulation Assignment with AIE bootstrap>
////////////////////////////////////////////////////////////
#pragma once
#include "Agent.h"
#include "IBehaviour.h"

class State;
class Agent;

class StateMachine : public IBehaviour
{
public:
	StateMachine();

	// Inherited via IBehaviour
	virtual void Update(Agent* pAgent, int& HungerStorage, int& ThirstStorage, float deltaTime);
	void ChangeState(Agent* agent, State* state);
	State* GetCurrentState();
	State* GetPreviousState();

	~StateMachine();

private:
	State* currentState;
	State* previousState;
};

