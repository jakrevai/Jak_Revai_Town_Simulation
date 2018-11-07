////////////////////////////////////////////////////////////
// Author: <Jak Revai>                                                              
// Project: <Town Simulation Assignment with AIE bootstrap>
////////////////////////////////////////////////////////////
#include "Agent.h"
#include "IBehaviour.h"
#include "StateMachine.h"
#include "WanderState.h"
#include "CollectWaterState.h"
#include "DrinkState.h"
#include "EatState.h"
#include "GatherState.h"
#include "SleepState.h"

Agent::Agent(aie::Texture* texture, Node* initialStart, Graph* graph, Role newRole)
{
	m_texture = texture;
	
	//Initial path creation
	m_Graph = graph;
	m_StartNode = initialStart;
	
	m_position.x = m_StartNode->m_position.x;
	m_position.y = m_StartNode->m_position.y;
	m_hunger = 100.0f;
	m_thirst = 100;
	m_tiredness = 100.0f;
	m_maxSpeed = 2;
	
	//State Creation
	m_stateMachine = new StateMachine();

	if (newRole == Role::DEFAULT)
	{
		m_stateMachine->ChangeState(this, new WanderState());
		role = newRole;
	}
	else if (newRole == Role::GATHERER)
	{
		m_stateMachine->ChangeState(this, new GatherState(m_Graph));
		role = newRole;
	}
	else if (newRole == Role::WELLCOLLECTOR)
	{
		role = newRole;
		m_stateMachine->ChangeState(this, new CollectWaterState(m_Graph));		
	}

	lastNodeReached = false;
}

Agent::~Agent()
{
	//Behaviour deletion
	//for (auto iterator = m_behaviours.begin(); iterator != m_behaviours.end(); iterator++)
	//{
	//	delete (*iterator);
	//}
	delete m_stateMachine;
}

void Agent::Update(float deltaTime, int &HungerStorage, int &ThirstStorage)
{		
	//Check incase a path returned is just 1 node
	if (m_path.size() <= 1 && initialPathSet == true)
	{
		lastNodeReached = true;
	}

	//Call update from stateMachine
	if (m_stateMachine != nullptr)
	{
		//Couldnt think of any other way to access the global variables that each agent pulls from,
		//so im passing these in from SimulationApp
		m_stateMachine->Update(this, HungerStorage, ThirstStorage, deltaTime);
	}

	//Behaviours list update
	//for (auto iterator = m_behaviours.begin(); iterator != m_behaviours.end(); iterator++)
	//{
	//	(*iterator)->Update(this, deltaTime); //Calls update from IBehaviour
	//}

	//Set the next node incase the start node is the same as the current node in the list
	if (m_StartNode == m_path.at(m_pathCurrentNode))
	{
		m_pathCurrentNode--;
	}
	if (lastNodeReached == false)
	{	
		nextNode = m_path.at(m_pathCurrentNode)->m_position;		
		direction = nextNode - m_position;	
		m_velocity = normalize(direction) * m_maxSpeed;
		m_position = m_position + m_velocity;

		int distance = sqrt((m_position.x - nextNode.x) * (m_position.x - nextNode.x) + (m_position.y - nextNode.y) * (m_position.y - nextNode.y));

		//Distance to how close the agent has to be to the node before moving on to the next
		if (distance <= 5)
		{
			if (m_pathCurrentNode <= 0)
			{
				m_lastNodeVisited = m_path.at(m_pathCurrentNode);
				lastNodeReached = true;

				//State Changing - This will update once the end of path has been reached
				//Should move these into a method and call it in each state instead at the end of path
				if (m_thirst <= 0 && !inDrinkState && !inEatState && !inCollectWaterState && !inGatherState)
					ChangeState(new DrinkState(m_Graph));
				else if (ThirstStorage <= 0 && !inCollectWaterState && role == Role::WELLCOLLECTOR)
					ChangeState(new CollectWaterState(m_Graph));
				else if (m_hunger <= 0.0f && !inEatState && !inDrinkState && !inCollectWaterState && !inGatherState)
					ChangeState(new EatState(m_Graph));
				else if (HungerStorage <= 0 && !inGatherState && role == Role::GATHERER)
					ChangeState(new GatherState(m_Graph));
				else if (m_tiredness <= 0.0f && !inSleepState && !inEatState && !inDrinkState && !inCollectWaterState && !inGatherState)
					ChangeState(new SleepState(m_Graph));
			}
			else
			{
				if (m_stateMachine->GetCurrentState() != new EatState(m_Graph))
					m_hunger -= rand() / (RAND_MAX / (2-0.5f));
				//rand between 0.5 - 2
				
				if (m_stateMachine->GetCurrentState() != new DrinkState(m_Graph))
					m_thirst -= rand() % 3;
				
				if (m_stateMachine->GetCurrentState() != new SleepState(m_Graph))
					m_tiredness -= rand() / (RAND_MAX / (1 - 0.2f));
				//rand between 0.2 - 1
				
				if (m_tiredness <= 0) //Clamping tiredness
					m_tiredness = 0;				

				if (m_hunger <= 0)
					m_hunger = 0;

				if (m_thirst <= 0)
					m_thirst = 0;

				m_pathCurrentNode--; //End of path not yet reached, so continue through the list
				nextNode = m_path.at(m_pathCurrentNode)->m_position;
			}			
		}
	}
	else
	{
		//Calculate new path
		GenerateNewPath();
	}
}

void Agent::GenerateNewPath()
{
	if (lastNodeReached == true) //If the agent has reached the end of the old path generate a new one
	{
		m_path.clear();
		
		m_StartNode = m_lastNodeVisited; //Set the start node to the last node visited

		//m_endNode has to be set somewhere else otherwise it will stay on the same path
		//This is set in the stateMachine
		m_path = m_Graph->AStarSeach(m_StartNode, m_endNode, Node::heuristicDistance);

		SetPath(m_path);
		lastNodeReached = false;		
	}
}

void Agent::ChangeState(State* newState)
{
	if (m_stateMachine->GetCurrentState() != newState)
	{
		m_stateMachine->ChangeState(this, newState);
	}	
}

void Agent::Draw(aie::Renderer2D * render)
{
	if (inCollectWaterState)
		render->drawSprite(bucketEmpty, m_position.x, m_position.y + 32, 24, 24);
	if (inSleepState)
		render->drawSprite(icon_bed, m_position.x, m_position.y + 32, 24, 24);
	if (inDrinkState)
		render->drawSprite(icon_thirst, m_position.x, m_position.y + 32, 24, 24);
	if (inEatState)
		render->drawSprite(icon_hunger, m_position.x, m_position.y + 32, 24, 24);
	if (inGatherState)
		render->drawSprite(icon_cherry, m_position.x, m_position.y + 32, 24, 24);

	render->drawSprite(m_texture, m_position.x, m_position.y, 48, 48);
}

//Adds a behaviour to m_behaviours for the agent to use
void Agent::AddBehaviour(IBehaviour * behaviour)
{
	m_behaviours.push_back(behaviour); 
}

void Agent::SetPath(std::vector<Node*> path)
{
	if (m_path.size() >= 1)
	{
		m_path.clear();
	}
	m_path = path;
	m_pathSize = path.size();
	m_pathCurrentNode = m_pathSize - 1; //Minus 1 for use in zero indexing
	initialPathSet = true;
}

void Agent::SetStartNode(int x, int y)
{
	m_StartNode = m_Graph->FindNodeAt(x, y);
}

void Agent::SetStartNode(Node* node)
{
	m_StartNode = node;
}

void Agent::SetEndNode(int x, int y)
{
	m_endNode = m_Graph->FindNodeAt(x, y);
}

void Agent::SetEndNode(Node* node)
{
	m_endNode = node;
}

void Agent::AddThirst(int thirst)
{
	m_thirst += thirst;
}

void Agent::AddHunger(float hunger)
{
	m_hunger += hunger;
}

int Agent::GetThirst()
{
	return m_thirst;
}

float Agent::GetHunger()
{
	return m_hunger;
}

void Agent::AddTiredness(float tiredness)
{
	m_tiredness += tiredness;
}

float Agent::GetTiredness()
{
	return m_tiredness;
}