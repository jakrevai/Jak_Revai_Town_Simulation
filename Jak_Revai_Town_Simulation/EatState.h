////////////////////////////////////////////////////////////
// Author: <Jak Revai>                                                              
// Project: <Town Simulation Assignment with AIE bootstrap>
////////////////////////////////////////////////////////////
#pragma once
#include "State.h"
class EatState :
	public State
{
public:
	EatState(Graph* graph);
	~EatState();

	Graph* m_graph;
	Node* m_eatLocation;
	float timer;

	// Inherited via State
	virtual void OnEnter(Agent * agent) override;
	virtual void Update(Agent * agent, StateMachine * stateMachine, int &HungerStorage, int &ThirstStorage, float deltaTime) override;
	virtual void OnExit(Agent * agent) override;
};

