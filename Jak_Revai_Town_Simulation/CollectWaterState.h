////////////////////////////////////////////////////////////
// Author: <Jak Revai>                                                              
// Project: <Town Simulation Assignment with AIE bootstrap>
////////////////////////////////////////////////////////////
#pragma once
#include "State.h"
class CollectWaterState :
	public State
{
public:
	CollectWaterState(Graph* graph);
	~CollectWaterState();
	
	Graph* m_graph;
	
	Node* m_well; //Node location for the well
	Node* m_endNode; //Drop off location for the water
	float timer;
	int thirstStored;
	bool atWell; //Check if agent is at the well
	bool atDropOff; //Checkk if agent is at the dropOff location
	//Sleep location - 12, 19
	//Food location - 12, 11
	//Water location - 19, 11

	// Inherited via State
	virtual void OnEnter(Agent* agent) override;
	virtual void Update(Agent* agent, StateMachine* stateMachine, int& HungerStorage, int& ThirstStorage, float deltaTime);
	virtual void OnExit(Agent* agent) override;
};

