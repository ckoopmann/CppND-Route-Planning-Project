#include "route_planner.h"
#include "route_model.h"
#include <algorithm>

RoutePlanner::RoutePlanner(RouteModel &model, float start_x, float start_y, float end_x, float end_y): m_Model(model) {
    // Convert inputs to percentage:
    start_x *= 0.01;
    start_y *= 0.01;
    end_x *= 0.01;
    end_y *= 0.01;

    // TODO 2: Use the m_Model.FindClosestNode method to find the closest nodes to the starting and ending coordinates.
    // Store the nodes you find in the RoutePlanner's start_node and end_node attributes.
    start_node = &model.FindClosestNode(start_x, start_y);
    end_node = &model.FindClosestNode(end_x, end_y);

}


// TODO 3: Implement the CalculateHValue method.
// Tips:
// - You can use the distance to the end_node for the h value.
// - Node objects have a distance method to determine the distance to another node.

float RoutePlanner::CalculateHValue(RouteModel::Node const *node) {
    float distance = node->distance(*end_node);
    return distance;
}


// TODO 4: Complete the AddNeighbors method to expand the current node by adding all unvisited neighbors to the open list.
// Tips:
// - Use the FindNeighbors() method of the current_node to populate current_node.neighbors vector with all the neighbors.
// - For each node in current_node.neighbors, set the parent, the h_value, the g_value. 
// - Use CalculateHValue below to implement the h-Value calculation.
// - For each node in current_node.neighbors, add the neighbor to open_list and set the node's visited attribute to true.

void RoutePlanner::AddNeighbors(RouteModel::Node *current_node) {
    current_node->FindNeighbors();
    float current_g_value = current_node->g_value;
    for (RouteModel::Node *neighbor : current_node->neighbors) {
        neighbor->g_value = current_g_value + current_node->distance(*neighbor);
        neighbor->h_value = CalculateHValue(neighbor);
        neighbor->parent = current_node;
        neighbor->visited = true;
        open_list.push_back(neighbor);
    }

}


// TODO 5: Complete the NextNode method to sort the open list and return the next node.
// Tips:
// - Sort the open_list according to the sum of the h value and g value.
// - Create a pointer to the node in the list with the lowest sum.
// - Remove that node from the open_list.
// - Return the pointer.

/**
 * Compare the F values of two nodes.
 */
bool Compare(RouteModel::Node const *a, RouteModel::Node const *b) {
  float f1 = a->g_value + a->h_value; 
  float f2 = b->g_value + b->h_value; 
  return f1 > f2; 
}


/**
 * Sort the vector of Nodes
 */
void NodeSort(std::vector<RouteModel::Node*> nodes) {
  sort(nodes.begin(), nodes.end(), Compare);
}

RouteModel::Node *RoutePlanner::NextNode() {
    NodeSort(open_list);
    RouteModel::Node *next = open_list.back();
    open_list.pop_back();
    return next;
}


// TODO 6: Complete the ConstructFinalPath method to return the final path found from your A* search.
// Tips:
// - This method should take the current (final) node as an argument and iteratively follow the 
//   chain of parents of nodes until the starting node is found.
// - For each node in the chain, add the distance from the node to its parent to the distance variable.
// - The returned vector should be in the correct order: the start node should be the first element
//   of the vector, the end node should be the last element.

std::vector<RouteModel::Node> RoutePlanner::ConstructFinalPath(RouteModel::Node *current_node) {
    // Create path_found vector
    distance = 0.0f;
    std::vector<RouteModel::Node> path_found;

    // TODO: Implement your solution here.
    while(current_node){
        path_found.push_back(*current_node);
        RouteModel::Node *parent = current_node->parent;
        if(parent){
            distance += parent->distance(*current_node);
        }
        current_node = current_node->parent;
    }
    std::reverse(path_found.begin(), path_found.end()); 

    distance *= m_Model.MetricScale(); // Multiply the distance by the scale of the map to get meters.
    return path_found;

}


// TODO 7: Write the A* Search algorithm here.
// Tips:
// - Use the AddNeighbors method to add all of the neighbors of the current node to the open_list.
// - Use the NextNode() method to sort the open_list and return the next node.
// - When the search has reached the end_node, use the ConstructFinalPath method to return the final path that was found.
// - Store the final path in the m_Model.path attribute before the method exits. This path will then be displayed on the map tile.

void RoutePlanner::AStarSearch() {
    std::cout << "Start Search";
    RouteModel::Node *current_node = start_node;
    AddNeighbors(current_node);

    int counter = 0;
    // TODO: Implement your solution here.
    while(open_list.size() > 0){
        std::cout << open_list.size() << ", ";
        if(current_node == end_node){
            std::cout << "End Reached, Constructing final path";
            m_Model.path = ConstructFinalPath(current_node);
            return;
        }
        counter++;
        current_node = NextNode();
        AddNeighbors(current_node);
    }
    return;
}
