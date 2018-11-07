////////////////////////////////////////////////////////////
// Author: <Jak Revai>                                                              
// Project: <Town Simulation Assignment with AIE bootstrap>
////////////////////////////////////////////////////////////
#pragma once
#include "Node.h"

class Node; //Forward declared class

class Edge
{
public:
	// default constructor
	Edge();

	// overloaded construct
	Edge(Node* node);
	Edge(Node* node, float Cost);

	~Edge();

	Node* connection;
	float m_cost;
};

