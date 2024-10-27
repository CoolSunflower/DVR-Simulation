#ifndef DEFS_HPP
#define DEFS_HPP
#include <iostream>
#include <algorithm>
#include <fstream>  // Included for file operations
#include <sys/types.h>
#include <sys/stat.h>
#include <vector>
#include <map>
#ifdef _WIN32
#include <direct.h>  // For Windows mkdir
#else
#include <unistd.h>  // For POSIX mkdir
#endif
const int INFINITY = 999; // Representation of infinity

// Structure for edges in the network graph
struct Edge {
    int src;    // Source node
    int dest;   // Destination node
    int cost;   // Cost of the edge
};

// Structure for each node (router) in the network
struct Node {
    int id;                                      // Unique identifier for the node
    std::map<int, int> distanceVector;           // Destination -> Cost
    std::map<int, int> nextHop;                  // Destination -> Next hop
    std::vector<int> neighbors;                   // Neighboring nodes
};

// Function prototypes
void initializeNodes(std::vector<Node>& nodes, const std::vector<Edge>& edges, int N);
void initializeDistanceVectors(std::vector<Node>& nodes, const std::vector<Edge>& edges, int N);
bool updateDistanceVectors(std::vector<Node>& nodes, int N, int method);
void printRoutingTables(const std::vector<Node>& nodes, int N);
bool checkCountToInfinity(const std::vector<Node>& nodes, int N);
void createDirectoryIfNotExists(const std::string& dirName);
void printDistanceVectorsToFile(const std::vector<Node>& nodes, int N, const std::string& filename);

#endif // DEFS_HPP
