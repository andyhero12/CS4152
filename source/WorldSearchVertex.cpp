//
//  WorldSearchVertex.cpp
//  Heaven
//
//  Created by Colin Wu on 4/9/24.
//

#include "WorldSearchVertex.h"

bool WorldSearchVertex::IsSameState( WorldSearchVertex &rhs )
{

    // same state in a maze search is simply when (x,y) are the same
    if( (x == rhs.x) &&
        (y == rhs.y) )
    {
        return true;
    }
    else
    {
        return false;
    }

}

size_t WorldSearchVertex::Hash()
{
    size_t h1 = std::hash<float>{}(x);
    size_t h2 = std::hash<float>{}(y);
    return h1 ^ (h2 << 1);
}

float WorldSearchVertex::GoalDistanceEstimate( WorldSearchVertex &nodeGoal )
{
    return abs(x - nodeGoal.x) + abs(y - nodeGoal.y);
}

bool WorldSearchVertex::IsGoal( WorldSearchVertex &nodeGoal )
{

    if( (x == nodeGoal.x) &&
        (y == nodeGoal.y) )
    {
        return true;
    }

    return false;
}

// This generates the successors to the given Node. It uses a helper function called
// AddSuccessor to give the successors to the AStar class. The A* specific initialisation
// is done for each node internally, so here you just set the state information that
// is specific to the application
bool WorldSearchVertex::GetSuccessors( AStarSearch<WorldSearchVertex> *astarsearch, WorldSearchVertex *parent_node )
{
    
    //Create list of all possible next tiles
    std::list<std::pair<int, int>> adj_list;
    
    // Left, right, top, bottom
    adj_list.push_back(std::make_pair(0, 1));
    adj_list.push_back(std::make_pair(0, -1));
    adj_list.push_back(std::make_pair(1, 0));
    adj_list.push_back(std::make_pair(-1, 0));
    
    //Diagonals
    adj_list.push_back(std::make_pair(-1, -1));
    adj_list.push_back(std::make_pair(1, 1));
    adj_list.push_back(std::make_pair(-1, 1));
    adj_list.push_back(std::make_pair(1, -1));
    
    int parent_x = -1;
    int parent_y = -1;

    if( parent_node )
    {
        parent_x = parent_node->x;
        parent_y = parent_node->y;
    }
    
    WorldSearchVertex NewNode(0, 0, _world);
    
    // push each possible move except allowing the search to go backwards
    for (const auto& diff : adj_list) {
        NewNode = WorldSearchVertex( x - diff.first, y - diff.second, _world);
        
        if(_world->isPassable(NewNode.x, NewNode.y) && !((parent_x == NewNode.x) && (parent_y == NewNode.y))){
            astarsearch->AddSuccessor( NewNode );
        }
    }
    
    return true;
}

float WorldSearchVertex::GetCost( WorldSearchVertex &successor )
{
    return 0;
}
