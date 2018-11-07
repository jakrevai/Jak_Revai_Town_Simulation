////////////////////////////////////////////////////////////
// Author: <Jak Revai>                                                              
// Project: <Town Simulation Assignment with AIE bootstrap>
////////////////////////////////////////////////////////////
#include "Edge.h"

Edge::Edge() : connection(nullptr), m_cost(0) { }

Edge::Edge(Node *node) : connection(node), m_cost(0) { }

Edge::Edge(Node *node, float Cost) : connection(node), m_cost(Cost) { }

Edge::~Edge()
{
}
