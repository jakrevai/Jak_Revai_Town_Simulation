////////////////////////////////////////////////////////////
// Author: <Jak Revai>                                                              
// Project: <Town Simulation Assignment with AIE bootstrap>
////////////////////////////////////////////////////////////
#include "CollectWaterState.h"
#include "WanderState.h"

CollectWaterState::CollectWaterState(Graph* graph)
{
	m_graph = graph;
	m_well = m_graph->FindNodeAt(1, 21);
	m_endNode = m_graph->FindNodeAt(19, 11); //Drop off zone
	thirstStored = 0;
	timer = 0;
	atWell = false;
	atDropOff = false;
}

CollectWaterState::~CollectWaterState()
{
}

void CollectWaterState::OnEnter(Agent * agent)
{
	agent->inCollectWaterState = true;
}

void CollectWaterState::Update(Agent* agent, StateMachine* stateMachine, int& HungerStorage, int& ThirstStorage, float deltaTime)
{		
	if (ThirstStorage <= 0)
	{
		agent->SetEndNode(m_well);

		if (agent->initialPathSet == false)
		{
			agent->m_path = agent->m_Graph->AStarSeach(agent->m_StartNode, agent->m_endNode, Node::heuristicDistance);
			agent->SetPath(agent->m_path);
		}

		if (agent->m_path.at(agent->m_pathCurrentNode) == m_well)
		{
			atWell = true;
		}

		if (atWell == true && agent->lastNodeReached == true) //If the agent is currently at the well
		{
			if (thirstStored < 100)
			{				
				timer += deltaTime; //Add deltaTime to the timer
				
				if (timer >= 1) //Add 5 thirst to the storage at 1 second intervals
				{
					thirstStored += 20;
					timer = 0;
				}
			}
			else if (thirstStored == 100)
			{
				agent->SetEndNode(m_endNode); //thirst storage has been filled, visit the drop off to store it
				atWell = false;
			}
		}
	}

	if (atWell == false && agent->m_path.at(agent->m_pathCurrentNode) == m_endNode && agent->lastNodeReached == true && thirstStored == 100) //If the agent is currently at the well
	{
		ThirstStorage = thirstStored;
		agent->ChangeState(new WanderState());
	}
}

void CollectWaterState::OnExit(Agent * agent)
{
	agent->inCollectWaterState = false;
}
