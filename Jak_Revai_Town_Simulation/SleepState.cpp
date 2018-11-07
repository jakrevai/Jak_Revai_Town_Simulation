////////////////////////////////////////////////////////////
// Author: <Jak Revai>                                                              
// Project: <Town Simulation Assignment with AIE bootstrap>
////////////////////////////////////////////////////////////
#include "SleepState.h"
#include "WanderState.h"

SleepState::SleepState(Graph* graph)
{
	m_graph = graph;
	m_sleepLocation = m_graph->FindNodeAt(12, 19);
	timer = 0;
}


SleepState::~SleepState()
{
}

void SleepState::OnEnter(Agent* agent)
{
	agent->inSleepState = true;
}

void SleepState::Update(Agent* agent, StateMachine* stateMachine, int &HungerStorage, int &ThirstStorage, float deltaTime)
{
	agent->SetEndNode(m_sleepLocation);

	if (agent->m_path.at(agent->m_pathCurrentNode) == m_sleepLocation && agent->lastNodeReached == true)
	{
		timer += deltaTime;

		if (timer > 1)
		{
			if (agent->GetTiredness() <= 100)
			{
				agent->AddTiredness(25);
			}
			else if (agent->GetTiredness() >= 100)
			{
				agent->ChangeState(new WanderState());
			}
			timer = 0;
		}
	}
}

void SleepState::OnExit(Agent* agent)
{
	agent->inSleepState = false;
}
