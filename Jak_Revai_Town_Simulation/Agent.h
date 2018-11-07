////////////////////////////////////////////////////////////
// Author: <Jak Revai>                                                              
// Project: <Town Simulation Assignment with AIE bootstrap>
////////////////////////////////////////////////////////////
#pragma once

#include <glm\glm.hpp>
#include <list>
#include <Texture.h>
#include <Renderer2D.h>

#include "Node.h"
#include "Graph.h"

namespace aie
{
	class Texture;
	class Renderer2D;
};

//Forward declaring
class IBehaviour;
class StateMachine;
class State;

//Role for the Agent (Defines STARTING state, and what state to resume from wander)
enum Role
{
	//Default wander
	DEFAULT,
	WELLCOLLECTOR,
	GATHERER
};

class Agent
{
public:
	Agent(aie::Texture* texture, Node* initialStart, Graph* graph, Role state);
	~Agent();

	virtual void Update(float deltaTime, int &HungerStorage, int &ThirstStorage);
	virtual void Draw(aie::Renderer2D* render);
	void AddBehaviour(IBehaviour* behaviour);	
	void GenerateNewPath(); //Generate a new A* path for the agent
	void ChangeState(State* newState); //Changes the Agent's current state

	//Getters
	int GetThirst();
	float GetHunger();
	float GetTiredness();

	//Setters
	void SetPath(std::vector<Node*> path);
	void SetStartNode(int x, int y);
	void SetStartNode(Node* node);
	void SetEndNode(int x, int y);
	void SetEndNode(Node* node);
	void AddThirst(int thirst);
	void AddHunger(float hunger);
	void AddTiredness(float tiredness);

	bool lastNodeReached; //Flag to let the agent know to update its path
	bool initialPathSet = false;

	//State flags to determine what icon to draw
	//Want to find a more elegant way for deciding what icons to draw
	//Could try to overload the == operator and compare the two states possibly
	bool inCollectWaterState = false;
	bool inEatState = false;
	bool inDrinkState = false;
	bool inSleepState = false;
	bool inGatherState = false;

	int role; //Defines what type of villager to be
	
	//Pathfinding attributes
	Graph* m_Graph;
	Node* m_StartNode;
	Node* m_endNode;
	std::vector<Node*> m_path; //Path is added backwards in the list, so the Final Node in the list is actually the first one, so need to navigate backwards through it
	int m_pathCurrentNode; //Setting current node to be the first one in the list(first one added, but last in the list)

protected:
	std::list<IBehaviour*> m_behaviours;
	aie::Texture* m_texture;
	
	float m_maxSpeed;
	float m_tiredness;
	float m_hunger;
	int m_thirst;
	
	glm::vec2 m_velocity;
	glm::vec2 m_position;
	glm::vec2 direction;

	int m_pathSize; //Minus 1 for zero indexing the m_path	
	glm::vec2 nextNode;
	Node* m_lastNodeVisited;

	aie::Texture* icon_bed = new aie::Texture("./textures/icon_bed.png");		//When in Sleep State
	aie::Texture* icon_hunger = new aie::Texture("./textures/icon_hunger.png"); //When in Eat State
	aie::Texture* icon_thirst = new aie::Texture("./textures/icon_thirst.png"); //When in Drink State
	aie::Texture* icon_cherry = new aie::Texture("./textures/icon_cherry.png"); //Gatherer Role
	aie::Texture* icon_axe = new aie::Texture("./textures/icon_axe.png");		//Logger Role - Eventually
	aie::Texture* bucketFull = new aie::Texture("./textures/bucketFull.png");	//When in Well State. Has Water to drop off at storage.
	aie::Texture* bucketEmpty = new aie::Texture("./textures/bucketEmpty.png");	//When in Well State. On the way to the Well location.

	StateMachine* m_stateMachine;
};