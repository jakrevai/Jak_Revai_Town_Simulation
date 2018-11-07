////////////////////////////////////////////////////////////
// Author: <Jak Revai>                                                              
// Project: <Town Simulation Assignment with AIE bootstrap>
////////////////////////////////////////////////////////////
#include "DrinkState.h"
#include "WanderState.h"
#include "CollectWaterState.h"
#include <ctime>

DrinkState::DrinkState(Graph* graph)
{
	m_graph = graph;
	m_drinkLocation = m_graph->FindNodeAt(19,11);
	timer = 0;
}


DrinkState::~DrinkState()
{
}

void DrinkState::OnEnter(Agent* agent)
{
	agent->inDrinkState = true;
}

void DrinkState::Update(Agent* agent, StateMachine* stateMachine, int& HungerStorage, int& ThirstStorage, float deltaTime)
{
	//Check just incase they run out of water while the WellCollector is at the storage while drinking, forces the WellCollector to get some more
	if (agent->role == Role::WELLCOLLECTOR && ThirstStorage <= 0)
	{
		agent->ChangeState(new CollectWaterState(m_graph));
	}
	else
	{
		agent->SetEndNode(m_drinkLocation);

		if (agent->m_path.at(agent->m_pathCurrentNode) == m_drinkLocation && agent->lastNodeReached == true) //If the agent is currently at the drink location
		{
			timer += deltaTime;

			if (timer > 1)
			{
				//ThirstStorage try making a global extern, it is only set for the agent that collects water so other agents wont drink
				if (ThirstStorage >= 10 && agent->GetThirst() <= 100)
				{
					agent->AddThirst(25);
					ThirstStorage -= 10;
				}
				else if (agent->GetThirst() >= 100) //Once the agent has filled up its thirst value
				{
					agent->ChangeState(new WanderState());
				}
				timer = 0;
			}
		}
	}	
}

void DrinkState::OnExit(Agent* agent)
{
	agent->inDrinkState = false;
}
