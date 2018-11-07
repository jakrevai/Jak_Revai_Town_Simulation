////////////////////////////////////////////////////////////
// Author: <Jak Revai>                                                              
// Project: <Town Simulation Assignment with AIE bootstrap>
////////////////////////////////////////////////////////////
#include "WanderState.h"
WanderState::WanderState()
{
}


WanderState::~WanderState()
{
}

void WanderState::OnEnter(Agent* agent)
{
}

void WanderState::Update(Agent* agent, StateMachine* stateMachine, int &HungerStorage, int &ThirstStorage, float deltaTime)
{
	if (agent->initialPathSet == false)
	{
		agent->SetEndNode(agent->m_Graph->m_nodes.at(rand() % agent->m_Graph->m_nodes.size()));
		agent->m_path = agent->m_Graph->AStarSeach(agent->m_StartNode, agent->m_endNode, Node::heuristicDistance);
		agent->SetPath(agent->m_path);
	}

	if (agent->lastNodeReached == true)
	{
		agent->SetEndNode(agent->m_Graph->m_nodes.at(rand() % agent->m_Graph->m_nodes.size()));
	}
}

void WanderState::OnExit(Agent* agent)
{
}
