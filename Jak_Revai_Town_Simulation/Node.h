////////////////////////////////////////////////////////////
// Author: <Jak Revai>                                                              
// Project: <Town Simulation Assignment with AIE bootstrap>
////////////////////////////////////////////////////////////
#pragma once
#include <glm\glm.hpp> 
#include <Texture.h>
#include <Renderer2D.h>
#include <Font.h>
#include <glm\ext.hpp>
#include <iostream>
#include <vector>
#include "Edge.h"
#include <string.h>

class Edge; //Forward declared class

class Node
{
public:
	// default constructor
	Node();

	//copy constructor setting position
	Node(glm::vec2 pos);

	//copy constructor setting position, index, index for text debugging, texture
	Node(glm::vec2 pos, glm::vec2 index, int xIndex, int yIndex, aie::Texture* texture, int type);

	//Copy constructor setting pos and parent node
	Node(glm::vec2 pos, Node* parent);

	~Node();
	void Draw(aie::Renderer2D* spriteBatch, bool drawConnections, bool drawPath, bool drawNodeText);
	void Reset();

	static bool compareGScore(Node* a, Node* b) {
		return a->m_gScore < b->m_gScore;
	}

	static bool compareFScore(Node* a, Node* b) {
		return a->m_fScore < b->m_fScore;
	}

	//Manhattan distance check
	static float heuristicManhattan(Node* a, Node* b)
	{
		return float(b->m_position.x - a->m_position.x) + (b->m_position.y - a->m_position.y);
	}

	//Cartisional distance check
	static float heuristicDistance(Node* a, Node* b)
	{
		float x = b->m_position.x - a->m_position.x;
		float y = b->m_position.y - a->m_position.y;

		return sqrt(x * x + y * y);
	}

	int m_gScore;
	int m_fScore;
	int m_hScore;

	//X and Y indexes for the nodes to visualise onscreen
	int xTextIndex = 0;
	int yTextIndex = 0;

	//Char* to convert to from strings
	char* charX;
	char* charY;

	//Converted textIndexes to strings
	std::string stringX;
	std::string stringY;

	glm::vec2 m_position;
	glm::vec2 index;
	aie::Texture* m_texture;
	aie::Texture* m_bush;

	Node* m_parent;
	int m_NodeType;

	aie::Font*	m_font;

	bool highlighted; //Path
	bool searched; //Node been searched
	bool endnodeColour;

	std::vector<Edge*> m_connections;
};

