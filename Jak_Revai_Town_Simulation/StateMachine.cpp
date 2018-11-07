////////////////////////////////////////////////////////////
// Author: <Jak Revai>                                                              
// Project: <Town Simulation Assignment with AIE bootstrap>
////////////////////////////////////////////////////////////
#include "StateMachine.h"
#include "State.h"

StateMachine::StateMachine()
{
	//Setting default pointers
	currentState = nullptr;
	previousState = nullptr;
}


StateMachine::~StateMachine()
{
}

void StateMachine::Update(Agent* pAgent, int& HungerStorage, int& ThirstStorage, float deltaTime)
{
	if (currentState != nullptr)
	{
		currentState->Update(pAgent, this, HungerStorage, ThirstStorage, deltaTime); //Pass in agent and current statemachine
	}
}

void StateMachine::ChangeState(Agent* agent, State* state)
{
	if (currentState != nullptr)
	{
		currentState->OnExit(agent);
	}

	if (state != nullptr)
	{
		state->OnEnter(agent);
	}

	previousState = currentState;
	currentState = state;
}

State* StateMachine::GetCurrentState()
{
	return currentState;
}

State* StateMachine::GetPreviousState()
{
	return previousState;
}
