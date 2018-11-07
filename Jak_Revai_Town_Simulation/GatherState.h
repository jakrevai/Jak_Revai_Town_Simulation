////////////////////////////////////////////////////////////
// Author: <Jak Revai>                                                              
// Project: <Town Simulation Assignment with AIE bootstrap>
////////////////////////////////////////////////////////////
#pragma once
#include "State.h"
class GatherState :
	public State
{
public:
	GatherState(Graph* graph);
	~GatherState();

	// Inherited via State
	virtual void OnEnter(Agent * agent) override;
	virtual void Update(Agent * agent, StateMachine * stateMachine, int &HungerStorage, int &ThirstStorage, float deltaTime) override;
	virtual void OnExit(Agent * agent) override;

	Graph* m_graph;

	Node* m_endNode; //Drop off location for the food
	float timer;
	int foodStored;
	int bushVisited = 0;

	bool atWell; //Check if agent is at the well
	bool atDropOff; //Checkk if agent is at the dropOff location
};

