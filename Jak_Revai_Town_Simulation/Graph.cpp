////////////////////////////////////////////////////////////
// Author: <Jak Revai>                                                              
// Project: <Town Simulation Assignment with AIE bootstrap>
////////////////////////////////////////////////////////////
#include "Graph.h"

Graph::Graph()
{
}

Graph::~Graph()
{
	//Removing nodes from the graph
	for (auto iter = m_nodes.begin(); iter != m_nodes.end(); iter++)
	{
		delete (*iter);
	}
	m_nodes.clear();
}

//Add node as a vector 2
Node *Graph::AddNode(glm::vec2 m_position)
{
	m_nodes.push_back(new Node(m_position));
	return m_nodes.back();
}

Node *Graph::AddNode(glm::vec2 m_position, glm::vec2 index, int xIndex, int yIndex, NodeType type)
{	
	aie::Texture* texture;

	if (type == NodeType::GRASS)
	{
		texture = new aie::Texture("./textures/grass.png");
	}
	if (type == NodeType::PATH_SIDE)
	{
		texture = new aie::Texture("./textures/path_side.png");
	}
	if (type == NodeType::PATH_UP)
	{
		texture = new aie::Texture("./textures/path_up.png");
	}
	if (type == NodeType::PATH_BOTLEFT)
	{
		texture = new aie::Texture("./textures/path_botLeft.png");
	}
	if (type == NodeType::PATH_BOTRIGHT)
	{
		texture = new aie::Texture("./textures/path_botRight.png");
	}
	if (type == NodeType::PATH_TOPRIGHT)
	{
		texture = new aie::Texture("./textures/path_topRight.png");
	}
	if (type == NodeType::PATH_ENDTOP)
	{
		texture = new aie::Texture("./textures/path_endTop.png");
	}
	if (type == NodeType::BUSH)
	{
		texture = new aie::Texture("./textures/bush.png");
	}
	m_nodes.push_back(new Node(m_position, index, xIndex, yIndex, texture, type));
	return m_nodes.back();
}

void Graph::AddConnection(Node* n1, Node* n2, float cost)
{
	n1->m_connections.push_back(new Edge(n2, cost));
}

Node* Graph::FindNodeAt(int x, int y)
{
	glm::vec2(x, y);
	for (auto node : m_nodes)
	{
		if (node->index.x == x && node->index.y == y)
		{
			return node;
		}
	}

	return nullptr;	
}

void Graph::Draw(aie::Renderer2D * spriteBatch, bool drawConnections, bool drawPath, bool drawNodeText)
{
	for (auto node : m_nodes) //Looping through the nodes to draw them
	{
		node->Draw(spriteBatch, drawConnections, drawPath, drawNodeText);
	}
}

void Graph::Reset()
{
	for (auto node : m_nodes)
	{
		node->Reset();
	}
}

void Graph::AddBush(Node* bush)
{
	m_bushes.push_back(bush);
}

void Graph::Djikstras(Node * startNode, const std::list<Node*>& endNodes, std::list<Node*>& outPath)
{
	std::list<Node*> openList;
	std::list<Node*> closedList;

	Node* endNode = nullptr;

	//Resetting scores
	startNode->m_gScore = 0;
	startNode->m_parent = nullptr;

	openList.push_back(startNode);

	//Sort open list by gScore
	while (openList.empty() == false)
	{
		openList.sort(Node::compareGScore);

		Node* node = openList.front();
		openList.pop_front();

		//If end node has been found stop immediatly
		for (auto it = endNodes.begin(); it != endNodes.end(); it++)
		{
			if (node == *it)
			{
				endNode = node;
				break;
			}
		}

		closedList.push_back(node); //Mark as traversed

		//Check each connection
		for (auto it = node->m_connections.begin(); it != node->m_connections.end(); it++)
		{
			Node* connection = (*it)->connection;
			if (connection == nullptr)
			{
				continue;
			}

			bool inClosed = std::find(closedList.begin(), closedList.end(), connection) != closedList.end();
			bool inOpened = std::find(openList.begin(), openList.end(), connection) != openList.end();

			//Setting gScore to connections cost
			float gScore = node->m_gScore + (*it)->m_cost;

			if (!inClosed)
			{
				//Not traversed, set score
				connection->m_gScore = gScore;
				connection->m_parent = node;
			}
			else
			{
				//Already traversed, check if we have a lower score
				if (gScore < connection->m_gScore)
				{
					connection->m_gScore = gScore;
					connection->m_parent = node;
				}
			}

			if (!inOpened && !inClosed)
			{
				//Add to open list if not already in there
				openList.push_back(connection);
				connection->searched = true;
			}
		}
	} //End while loop

	  //Calculate the path
	Node* current = endNode;
	while (current != nullptr)
	{
		outPath.push_back(current);
		current = current->m_parent;
	}

	//	Set startNode.parent to null

	//	// Process the node, do what you want with it. EG:
	//	if currentNode is endNode
	//		break out of loop
	//		remove currentNode from openList
	//		Add currentNode to closedList
	//		for all connections c in currentNode
	//			Add c.target to openList if not in closedList
	//			c.target.gScore = currentNode.gScore + c.cost
	//			c.target.parent = currentNode
	//			// Calculate Path, in this example as positions
	//			Let path be a Stack of Vector2
	//			Let currentNode = endNode;
	//While currentNode is not null
	//	Add currentNode.position to path
	//	currentNode = currentNode.parent
	//Return path
}

std::vector<Node*> Graph::AStarSeach(Node* startNode, Node* endNode, HeuristicCheck heuristic)
{
	Reset();
	std::list<Node*> openList;
	std::list<Node*> closedList;
	
	//Reset
	startNode->m_parent = nullptr;
	startNode->m_gScore = 0;
	startNode->m_hScore = heuristic(startNode, endNode);
	startNode->m_fScore = startNode->m_gScore + startNode->m_hScore;

	openList.push_back(startNode);

	//Sort open list by gScore
	while (openList.empty() == false)
	{
		openList.sort(Node::compareFScore);

		Node* node = openList.front();
		openList.pop_front();

		//If end node has been found stop immediatly
		if (node == endNode)
		{
			break;
		}

		closedList.push_back(node); //Mark as traversed

									//Check each connection
		for (auto it = node->m_connections.begin(); it != node->m_connections.end(); it++)
		{
			Node* connection = (*it)->connection;
			
			if (connection == nullptr)
			{
				continue;
			}

			//Check if its already in the closed or open list
			bool inClosed = NodeInClosedList(closedList, connection);// std::find(closedList.begin(), closedList.end(), connection) != closedList.end();
			bool inOpened = NodeInOpenList(openList, connection);// std::find(openList.begin(), openList.end(), connection) != openList.end();

			//Setting the scores
			float gScore = node->m_gScore + (*it)->m_cost;
			float hScore = heuristic(connection, endNode);
			float fScore = gScore + hScore;

			if (!inClosed)
			{
				//Not traversed, set score
				connection->m_gScore = gScore;
				connection->m_hScore = hScore;
				connection->m_fScore = fScore;
				connection->m_parent = node;

			}
			else
			{
				//Already traversed, check if its a lower score
				if (fScore < connection->m_fScore)
				{
					connection->m_gScore = gScore;
					connection->m_hScore = hScore;
					connection->m_fScore = fScore;
					connection->m_parent = node;

				}
			}

			if (!inOpened && !inClosed)
			{
				//Add to open list if not already in there
				openList.push_back(connection);
				connection->searched = true;
			}
		}
	} //End while loop

	  //Calculate the path
	std::vector<Node*> path;

	Node* current = endNode;
	//Known memory leak here if it gets caught in an inf loop
	while (current != nullptr)
		//Possibly fixed, additional nodes were being created underneath current ones, error on my part
		//so it was trying to create a path from the node above to the node below creating the inf parent loop
	{
		current->highlighted = true;
		path.push_back(current);
		current = current->m_parent;
	}

	//for each (Node* node in path)
	//{
	//	if (node->index.y == 19)
	//	{
	//		break;
	//	}
	//}

	return path;
}