////////////////////////////////////////////////////////////
// Author: <Jak Revai>                                                              
// Project: <Town Simulation Assignment with AIE bootstrap>
////////////////////////////////////////////////////////////
#include "EatState.h"
#include "WanderState.h"
#include "GatherState.h"

EatState::EatState(Graph* graph)
{
	m_graph = graph;
	m_eatLocation = m_graph->FindNodeAt(12, 11);
	timer = 0;
}


EatState::~EatState()
{
}

void EatState::OnEnter(Agent* agent)
{
	agent->inEatState = true;
}

void EatState::Update(Agent* agent, StateMachine* stateMachine, int &HungerStorage, int &ThirstStorage, float deltaTime)
{
	if (agent->role == Role::GATHERER && HungerStorage <= 0)
	{
		agent->ChangeState(new GatherState(m_graph));
	}
	else
	{
		agent->SetEndNode(m_eatLocation);

		if (agent->m_path.at(agent->m_pathCurrentNode) == m_eatLocation && agent->lastNodeReached == true)
		{
			timer += deltaTime;

			if (timer > 1)
			{
				if (HungerStorage >= 10 && agent->GetHunger() <= 100)
				{
					agent->AddHunger(25);
					HungerStorage -= 10;
				}
				else if (agent->GetHunger() >= 100)
				{
					agent->ChangeState(new WanderState());
				}
				timer = 0;
			}
		}
	}	
}

void EatState::OnExit(Agent* agent)
{
	agent->inEatState = false;
}
