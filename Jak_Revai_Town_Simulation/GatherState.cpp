////////////////////////////////////////////////////////////
// Author: <Jak Revai>                                                              
// Project: <Town Simulation Assignment with AIE bootstrap>
////////////////////////////////////////////////////////////
#include "GatherState.h"
#include "WanderState.h"

GatherState::GatherState(Graph* graph)
{
	m_graph = graph;
	m_endNode = m_graph->FindNodeAt(12,11); //Probably more taxing than just running it once in the App.cpp and pass it in the constructor
	foodStored = 0;
	timer = 0;
}


GatherState::~GatherState()
{
}

void GatherState::OnEnter(Agent* agent)
{
	agent->inGatherState = true;
}

void GatherState::Update(Agent* agent, StateMachine* stateMachine, int &HungerStorage, int &ThirstStorage, float deltaTime)
{
	if (HungerStorage <= 0)
	{
		if (agent->initialPathSet == false)
		{
			//Pick random bush to visit
			agent->SetEndNode(agent->m_Graph->m_bushes.at(rand() % agent->m_Graph->m_bushes.size()));
			agent->m_path = agent->m_Graph->AStarSeach(agent->m_StartNode, agent->m_endNode, Node::heuristicDistance);
			agent->SetPath(agent->m_path); //Inital path set is set to true in here
		}

		if (agent->m_path.at(agent->m_pathCurrentNode) && agent->lastNodeReached == true) //If the agent is currently at the well
		{
			if (foodStored < 100)
			{
				if (bushVisited < 2)
				{
					timer += deltaTime; //Add deltaTime to the timer

					if (timer >= 1)
					{
						foodStored += 25;
						timer = 0;
						bushVisited++;
					}

					if (foodStored != 100 && bushVisited == 2)
					{
						agent->SetEndNode(agent->m_Graph->m_bushes.at(rand() % agent->m_Graph->m_bushes.size())); //Find a new bush to visit
						bushVisited = 0;
					}
				}
			}
			else
			{
				agent->SetEndNode(m_endNode);
			}			
		}
	}

	if (agent->m_path.at(agent->m_pathCurrentNode) == m_endNode && agent->lastNodeReached == true && foodStored == 100)
	{
		HungerStorage = foodStored;
		agent->ChangeState(new WanderState());
	}
}

void GatherState::OnExit(Agent * agent)
{
	agent->inGatherState = false;
}
