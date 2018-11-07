////////////////////////////////////////////////////////////
// Author: <Jak Revai>                                                              
// Project: <Town Simulation Assignment with AIE bootstrap>
////////////////////////////////////////////////////////////
#include "Node.h"
//#include "Edge.h"
#include <vector>
#include <stack>
#include <queue>
#include <iostream>
#include <list>
#include <functional>

#include <Texture.h>
#include <Renderer2D.h>
#include <glm\glm.hpp>

enum NodeType
{
	GRASS,
	BUSH,
	PATH_SIDE,
	PATH_UP,
	PATH_BOTLEFT,
	PATH_BOTRIGHT,
	PATH_TOPRIGHT,
	PATH_ENDTOP,
};

class Graph
{
public:

	Graph();
	~Graph();

	Node *AddNode(glm::vec2 m_position);
	Node *AddNode(glm::vec2 m_position, glm::vec2 index, int xIndex, int yIndex, NodeType type); //Position of node, vec2 index of node easier to traverse, int index for visual display, NodeType defines what texture to use
	void AddConnection(Node *n1, Node *n2, float cost);
	Node* FindNodeAt(int x, int y);

	void Draw(aie::Renderer2D* spriteBatch, bool drawConnections, bool drawPath, bool drawNodeText);
	void Reset();

	//Djikstras Search
	void Djikstras(Node* startNode, const std::list<Node*> &endNodes, std::list<Node*> &outPath);

	//A* Search
	typedef std::function<float(Node* a, Node* b)> HeuristicCheck; //Function pointer to compare the two nodes coming in
	std::vector<Node*> AStarSeach(Node* startNode, Node* endNode, HeuristicCheck heuristic);
	std::vector<Node*> m_nodes;

	//Bush list that the Gatherer Agent will use to navigate to
	std::vector<Node*> m_bushes;
	void AddBush(Node* bush);

protected:
	bool NodeInClosedList(std::list<Node*> &closedList, Node* target)
	{
		for (auto it = closedList.begin(); it != closedList.end(); it++)
		{
			if (*it == target)
			{
				return true;
			}
		}
		return false;
	}
	bool NodeInOpenList(std::list<Node*> &openList, Node* target)
	{
		for (auto it = openList.begin(); it != openList.end(); it++)
		{
			if (*it == target)
			{
				return true;
			}
		}
		return false;
	}
};