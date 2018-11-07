////////////////////////////////////////////////////////////
// Author: <Jak Revai>                                                              
// Project: <Town Simulation Assignment with AIE bootstrap>
////////////////////////////////////////////////////////////
#include "Jak_Revai_Town_SimulationApp.h"
#include "Font.h"
#include "Input.h"
#include <cmath>
#include <ctime>
#include <imgui.h>
#include "StateMachine.h"

Jak_Revai_Town_SimulationApp::Jak_Revai_Town_SimulationApp() {
}

Jak_Revai_Town_SimulationApp::~Jak_Revai_Town_SimulationApp() {

}

bool Jak_Revai_Town_SimulationApp::startup() {
	
	m_2dRenderer = new aie::Renderer2D();
	m_font = new aie::Font("./font/consolas.ttf", 24);

	grass = new aie::Texture("./textures/grass.png");
	bush = new aie::Texture("./textures/bush.png");
	well = new aie::Texture("./textures/well.png");
	bucketFull = new aie::Texture("./textures/bucketFull.png");
	bucketEmpty = new aie::Texture("./textures/bucketEmpty.png");
	icon_bed = new aie::Texture("./textures/icon_bed.png");
	icon_hunger = new aie::Texture("./textures/icon_hunger.png");
	icon_thirst = new aie::Texture("./textures/icon_thirst.png");

	//Paths
	path1 = new aie::Texture("./textures/path_side.png");
	path2 = new aie::Texture("./textures/path_up.png");
	path3 = new aie::Texture("./textures/path_botLeft.png");
	path4 = new aie::Texture("./textures/path_botRight.png");
	path5 = new aie::Texture("./textures/path_topRight.png");
	path6 = new aie::Texture("./textures/path_endTop.png");
	
	//Structures
	roof1 = new aie::Texture("./textures/roof_1.png");
	structure1 = new aie::Texture("./textures/structure_1.png");
	structure2 = new aie::Texture("./textures/structure_2.png");
	structure3 = new aie::Texture("./textures/structure_3.png");
	structure4 = new aie::Texture("./textures/structure_4.png");
	structure5 = new aie::Texture("./textures/structure_5.png");

	//Villagers
	defaultVillagerTexture = new aie::Texture("./textures/defaultVillager.png");
	wellVillagerTexture = new aie::Texture("./textures/wellVillager.png");
	gathererVillagerTexture = new aie::Texture("./textures/gathererVillager.png");

	m_Graph = new Graph();
	srand(time(0)); //RAND() SEED
	gridWidth = 40; //Width of grid
	gridHeight = 22; //Height of grid
	const int offsetX = grass->getWidth() / 2 / 2; //Original Texture size should be 64x64, drawing them at 32x32, so need to divide twice
	const int offsetY = grass->getHeight() / 2 / 2;
	const int textureSizeX = grass->getWidth() / 2;
	const int textureSizeY = grass->getHeight() / 2;
	int maxDist = textureSizeX + offsetX;

	//Build Graph Nodes - bit of a silly way to decide where the paths are, but its easy enough for this simulation
	for (int i = 0; i < gridWidth; i++)
	{				
		for (int j = 0; j < gridHeight; j++)
		{	
			if (i > 1 && i < 21 && j == 19) //Paths below houses
			{
				m_Graph->AddNode(glm::vec2(offsetX + i * textureSizeX, offsetY + j * textureSizeY), glm::vec2(i, j), i, j, NodeType::PATH_SIDE);
			}
			else if (i == 21 && j == 19) //Cornerpath
				m_Graph->AddNode(glm::vec2(offsetX + i * textureSizeX, offsetY + j * textureSizeY), glm::vec2(i, j), i, j, NodeType::PATH_TOPRIGHT);
			else if (i == 21 && j > 11 && j < 19) //Path downwards
				m_Graph->AddNode(glm::vec2(offsetX + i * textureSizeX, offsetY + j * textureSizeY), glm::vec2(i, j), i, j, NodeType::PATH_UP);
			else if(i == 21 && j == 11) //Corner path
				m_Graph->AddNode(glm::vec2(offsetX + i * textureSizeX, offsetY + j * textureSizeY), glm::vec2(i, j), i, j, NodeType::PATH_BOTRIGHT);
			else if (i > 4 && i < 21 && j == 11) //Path below second set of houses
				m_Graph->AddNode(glm::vec2(offsetX + i * textureSizeX, offsetY + j * textureSizeY), glm::vec2(i, j), i, j, NodeType::PATH_SIDE);	


			else if (i > 0 && i < 2 && j > 19 && j < 21) //Path to Well
				m_Graph->AddNode(glm::vec2(offsetX + i * textureSizeX, offsetY + j * textureSizeY), glm::vec2(i, j), i, j, NodeType::PATH_UP);
			else if (i == 1 && j == 19) //Cornerpath to well
				m_Graph->AddNode(glm::vec2(offsetX + i * textureSizeX, offsetY + j * textureSizeY), glm::vec2(i, j), i, j, NodeType::PATH_BOTLEFT);

			else if (i > 4 && i < 22 && j > 19 && j < 21) //Room for house textures
				continue;
			else if (i > 4 && i < 21 && j == 12) //Second set of houses
				continue;
			else
			{
				m_Graph->AddNode(glm::vec2(offsetX + i * textureSizeX, offsetY + j * textureSizeY), glm::vec2(i, j), i, j, NodeType::GRASS);
			}
		}
	}

	//Building connections
	//Connections cost might be a bit borked, seems to prefer diagonal movement over the straight low cost paths
	for each (Node* a in m_Graph->m_nodes)
	{		
		for each (Node* b in m_Graph->m_nodes)
		{
			if (a == b)
			{
				continue;
			}

			float x = b->m_position.x - a->m_position.x;
			float y = b->m_position.y - a->m_position.y;
			float sqrDist = x * x + y * y;
			float dist = sqrt(sqrDist);

			//Non-Weighted connections
			//if (dist <= (maxDist)) //Add diagonal connections with a cost
			//{
			//	m_Graph->AddConnection(a, b, dist);
			//}
			if (dist <= (float)maxDist) //Add diagonal connections with a cost
			{
				float cost = dist;

				//Weighted Connections
				for (int i = NodeType::PATH_SIDE; i != NodeType::PATH_ENDTOP; i++)
				{
					if (a->m_NodeType == i || b->m_NodeType == i)
					{
						cost = 2;
						break;
					}
					else if (a->m_NodeType == NodeType::GRASS || b->m_NodeType == NodeType::GRASS)
					{
						cost = 10;
						break;
					}
				}
				m_Graph->AddConnection(a, b, cost);
			}			
		}
	}

	//Setting inital storage of thirst/hunger to 0
	HungerStorage = 0;
	ThirstStorage = 0;

	//Create villager agent
	m_StartNode = m_Graph->m_nodes.at(rand() % m_Graph->m_nodes.size()); //Set starting node position
	agentVillager = new Agent(defaultVillagerTexture, m_StartNode, m_Graph, Role::DEFAULT);
	
	//Recall rand() for a different position on the agentWaterCollector
	m_StartNode = m_Graph->m_nodes.at(rand() % m_Graph->m_nodes.size());
	agentWaterCollector = new Agent(wellVillagerTexture, m_StartNode, m_Graph, Role::WELLCOLLECTOR);

	m_StartNode = m_Graph->m_nodes.at(rand() % m_Graph->m_nodes.size());
	agentGatherer = new Agent(gathererVillagerTexture, m_StartNode, m_Graph, Role::GATHERER);

	//Adding 10 random values in the range of 0-12 to an array to pick what houses to draw
	for (int i = 0; i < 10; i++)
	{
		randHouse[i] = rand() % 12;
	}

	//Find 10 random nodes and mark them as type BUSH
	for (int i = 0; i < 10; i++)
	{
		int j = rand() % gridWidth;
		int k = rand() % gridHeight;
		Node* node = m_Graph->FindNodeAt(j, k);

		if (node != NULL && node->m_NodeType == NodeType::GRASS)
		{
			node->m_NodeType = NodeType::BUSH; //Assign the node found to now be a bush
			m_Graph->AddBush(node); //Add to a bush array
		}
		else
		{
			i--; //If the node doesnt exist(Tried searching the housing area) then minus 1 from i and try again till we have 10 total bushes
		}
	}

	return true; //End Startup
}

void Jak_Revai_Town_SimulationApp::shutdown() 
{
	delete m_font;
	delete m_2dRenderer;
	delete m_Graph;
	delete agentVillager;
	delete agentWaterCollector;
	delete agentGatherer;
}

void Jak_Revai_Town_SimulationApp::update(float deltaTime) {

	// input example
	aie::Input* input = aie::Input::getInstance();

	//Imgui
	ImGui::Checkbox("Draw Connections", &drawConnections);
	ImGui::Checkbox("Draw Agent Path", &drawPath);
	ImGui::Checkbox("Draw Node Text(Black = X, Yellow = Y)", &drawNodeText);

	agentVillager->Update(deltaTime, HungerStorage, ThirstStorage);
	agentWaterCollector->Update(deltaTime, HungerStorage, ThirstStorage);
	agentGatherer->Update(deltaTime, HungerStorage, ThirstStorage);

	//Convert to string
	str_hungerStorage = std::to_string(HungerStorage);
	str_thirstStorage = std::to_string(ThirstStorage);

	//Convert to char
	txtHungerStorage = (char*)str_hungerStorage.c_str();
	txtThirstStorage = (char*)str_thirstStorage.c_str();

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void Jak_Revai_Town_SimulationApp::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_2dRenderer->begin();

	// draw your stuff here!
	agentVillager->Draw(m_2dRenderer);
	agentWaterCollector->Draw(m_2dRenderer);
	agentGatherer->Draw(m_2dRenderer);

	spritePos = m_Graph->FindNodeAt(1, 21)->m_position; //find the node to draw the well
	m_2dRenderer->drawSprite(well, spritePos.x, spritePos.y); //Well Position

	 //Drawing icons to represent the drop off locations
	spritePos = m_Graph->FindNodeAt(12, 19)->m_position; //Sleep location
	m_2dRenderer->drawSprite(icon_bed, spritePos.x, spritePos.y + 48, 24, 24);

	spritePos = m_Graph->FindNodeAt(12, 11)->m_position; //Food location
	m_2dRenderer->drawSprite(icon_hunger, spritePos.x, spritePos.y + 48, 24, 24);

	spritePos = m_Graph->FindNodeAt(19, 11)->m_position; //Thirst location
	m_2dRenderer->drawSprite(icon_thirst, spritePos.x, spritePos.y + 48, 24, 24);

	//The magic numbers are the node index location, they are found via displaying the index text over the nodes for ease of use
	//Draw grass under the houses as well as the houses above the pathway
	spritePos = m_Graph->FindNodeAt(5, 19)->m_position;
	endPos = m_Graph->FindNodeAt(22, 19)->m_position.x;
	
	//Adding 32 to i because thats the size of the texture im manually setting for the houses
	//Draw Houses and Grass
	int randHouseCounter = 0;
	//Draw first set of houses
	for (int i = spritePos.x; i < endPos; i += 32) //X
	{	
		if (randHouse[randHouseCounter] <= 4)
		{
			m_2dRenderer->drawSprite(structure5, i, spritePos.y + 32, 32, 32);
		}
		else if(randHouse[randHouseCounter] <= 8 && randHouse[randHouseCounter] > 4)
		{
			m_2dRenderer->drawSprite(structure4, i, spritePos.y + 32, 32, 32);
		}
		else
		{
			m_2dRenderer->drawSprite(structure3, i, spritePos.y + 32, 32, 32);
		}
		m_2dRenderer->drawSprite(grass, i, spritePos.y + 32, 32, 32);
		randHouseCounter++;
	}

	//Drawing second set of houses
	randHouseCounter = 0;
	spritePos = m_Graph->FindNodeAt(5, 11)->m_position;
	endPos = m_Graph->FindNodeAt(21, 11)->m_position.x;
	for (int i = spritePos.x; i < endPos; i += 32) //X
	{
		if (randHouse[randHouseCounter] <= 4)
		{
			m_2dRenderer->drawSprite(structure5, i, spritePos.y + 32, 32, 32);
		}
		else if (randHouse[randHouseCounter] <= 8 && randHouse[randHouseCounter] > 4)
		{
			m_2dRenderer->drawSprite(structure4, i, spritePos.y + 32, 32, 32);
		}
		else
		{
			m_2dRenderer->drawSprite(structure3, i, spritePos.y + 32, 32, 32);
		}
		m_2dRenderer->drawSprite(grass, i, spritePos.y + 32, 32, 32);
		randHouseCounter++;
	}

	//Display hunger/thirst text
	m_2dRenderer->drawText(m_font, "Hunger: ", 1, 32);
	m_2dRenderer->drawText(m_font, txtHungerStorage, 96, 32);
	m_2dRenderer->drawText(m_font, "Thirst: ", 1, 8);
	m_2dRenderer->drawText(m_font, txtThirstStorage, 96, 8);
	
	m_Graph->Draw(m_2dRenderer, drawConnections, drawPath, drawNodeText);
	
	//Drawing that little bit of missing texture at the top
	for (int i = 0; i < gridWidth; i++)
	{
		m_2dRenderer->drawSprite(grass, 32 * i + 16, 32 * gridHeight + 16, 32, 32);
	}

	// done drawing sprites
	m_2dRenderer->end();
}