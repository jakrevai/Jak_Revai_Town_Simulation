////////////////////////////////////////////////////////////
// Author: <Jak Revai>                                                              
// Project: <Town Simulation Assignment with AIE bootstrap>
////////////////////////////////////////////////////////////
#pragma once
#include "Agent.h"
#include "Application.h"
#include "Renderer2D.h"
#include "CollectWaterState.h"
#include "DrinkState.h"
#include "EatState.h"
#include "GatherState.h"
#include "SleepState.h"

class Jak_Revai_Town_SimulationApp : public aie::Application {
public:

	Jak_Revai_Town_SimulationApp();
	virtual ~Jak_Revai_Town_SimulationApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

protected:

	aie::Renderer2D*	m_2dRenderer;
	aie::Font*			m_font;

	Graph* m_Graph;

	Agent* agentVillager; //Default wandering agent
	Agent* agentWaterCollector; //Agent that will visit the well and gather water
	Agent* agentGatherer; //Agent that will roam to bushes and gather food for the village

	//Initial starting nodes of villagers to be passed in
	Node* m_StartNode;

	aie::Texture* grass;
	aie::Texture* bush;
	aie::Texture* well;
	aie::Texture* bucketFull;
	aie::Texture* bucketEmpty;
	aie::Texture* icon_bed;
	aie::Texture* icon_hunger;
	aie::Texture* icon_thirst;
	aie::Texture* roof1; //Roof for Structure1
	aie::Texture* structure1;
	aie::Texture* structure2;
	aie::Texture* structure3;
	aie::Texture* structure4;
	aie::Texture* structure5;
	aie::Texture* defaultVillagerTexture;
	aie::Texture* wellVillagerTexture;
	aie::Texture* gathererVillagerTexture;

	glm::vec2 spritePos; //Sprite drawing position

	int endPos; //End position for a node location used in drawing
	int randHouse[10]; //Array to store random set of values for picking what house to draw
	int gridWidth; //Width of grid
	int gridHeight; //Height of grid

	int HungerStorage; //Value pool that each agent will pull from to replenish their needs
	int ThirstStorage;

	//Conversion to draw on screen
	std::string str_hungerStorage;
	std::string str_thirstStorage;
	char* txtHungerStorage;
	char* txtThirstStorage;

	//Debug bools for ImGui node drawing
	bool drawConnections = false;
	bool drawPath = false; //Havnt got around to having it show each path for each agent, just draws the last updated path
	bool drawNodeText = false;

	//Sideways Path
	aie::Texture* path1;
	//Vertical Path
	aie::Texture* path2;
	//BottomLeft Corner
	aie::Texture* path3;
	//BottomRight Corner
	aie::Texture* path4;
	//TopRight Corner
	aie::Texture* path5;
	//Path EndTop
	aie::Texture* path6;
};