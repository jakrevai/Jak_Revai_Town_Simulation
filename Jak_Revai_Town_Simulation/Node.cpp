////////////////////////////////////////////////////////////
// Author: <Jak Revai>                                                              
// Project: <Town Simulation Assignment with AIE bootstrap>
////////////////////////////////////////////////////////////
#include "Node.h"
#include "Graph.h"

Node::Node() : m_gScore(0), m_position(0, 0), m_parent(nullptr) {}

//copy constructor setting position
Node::Node(glm::vec2 pos) : m_gScore(0), m_position(pos), m_parent(nullptr) {}

//copy constructor setting position and texture
Node::Node(glm::vec2 pos, glm::vec2 index, int xIndex, int yIndex, aie::Texture* texture, int type) : m_gScore(0), m_position(pos), index(index), m_texture(texture), m_NodeType(type) ,m_parent(nullptr)
{
	m_font = new aie::Font("./font/consolas.ttf", 16);
	m_bush = new aie::Texture("./textures/bush.png");
	//Convert x and y index to string
	stringX = std::to_string(xIndex);
	stringY = std::to_string(yIndex);
	
	//Convert x and y strings to Char*
	charX = (char*)stringX.c_str();
	charY = (char*)stringY.c_str();
}

//Copy constructor setting pos and parent node
Node::Node(glm::vec2 pos, Node* parent) : m_gScore(0), m_position(pos), m_parent(parent) {}

Node::~Node()
{
}


void Node::Draw(aie::Renderer2D* spriteBatch, bool drawConnections, bool drawPath, bool drawNodeText)
{
	glm::vec4 renderColour = glm::vec4(1, 1, 1, 1); //Set default colour to white

	spriteBatch->setRenderColour(1, 1, 1, 1);

	//Render connections
	if (drawConnections == true)
	{
		for (auto edge : m_connections)
		{
			spriteBatch->drawLine(m_position.x, m_position.y, edge->connection->m_position.x, edge->connection->m_position.y);
		}
	}
	if (drawPath == true)
	{
		if (searched == true) //Displays node searched
		{
			renderColour = glm::vec4(0.5, 1, 1, 0.5);
		}
		if (highlighted == true) //Path
		{
			renderColour = glm::vec4(1, 0, 0, 1);
		}	
	}
	if (drawNodeText == true)
	{
		//Visualising node indexing for easier texture placement
		spriteBatch->setRenderColour(0, 0, 0, 1);
		spriteBatch->drawText(m_font, charX, m_position.x - 14, m_position.y + 4, 0); //X

		spriteBatch->setRenderColour(2, 2, 0, 1);
		spriteBatch->drawText(m_font, charY, m_position.x + 2, m_position.y - 12, 0); //Y
	}

	//Draw Nodes
	//Check if the type has been updated to a bush and draw it
	spriteBatch->setRenderColour(renderColour.r, renderColour.g, renderColour.b, renderColour.a);
	
	if (m_NodeType == NodeType::BUSH) //If the type is a bush, then the node it found was already a grass node and nothing else so texture should be grass
	{
		spriteBatch->drawSprite(m_bush, m_position.x, m_position.y); //Draw new Bush
		spriteBatch->drawSprite(m_texture, m_position.x, m_position.y, 32, 32); //Draw Grass
	}
	else
	{
		spriteBatch->drawSprite(m_texture, m_position.x, m_position.y, 32, 32);
	}
}

void Node::Reset()
{
	m_fScore = 9999;
	m_gScore = 9999;
	m_hScore = 9999;
	m_parent = NULL;
	searched = false;
	highlighted = false;
}