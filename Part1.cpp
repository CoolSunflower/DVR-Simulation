#include "defs.hpp"

using namespace std;

void initializeNodes(vector<Node>& nodes, vector<Edge>& edges, int N);
void initializeDistanceVectors(vector<Node>& nodes, vector<Edge>& edges, int N);

int main() {
    // Inputting number of routers and number of links
    int N, M;
    cin >> N >> M;

    // Inputting edges and their costs
    vector<Edge> edges(M);
    for (int i = 0; i < M; ++i) {
        cin >> edges[i].src >> edges[i].dest >> edges[i].cost;
    }

    int method = 1; // No error correction method

    vector<Node> nodes;
    initializeNodes(nodes, edges, N);
    initializeDistanceVectors(nodes, edges, N);

    // Run the DVR algorithm until convergence
    bool updated;
    int iteration = 0; // Iteration counter
    do {
        updated = updateDistanceVectors(nodes, N, method);

        // Increment iteration counter
        iteration++;

        // Generate filename
        string filename = "distance_vectors_iteration_" + to_string(iteration) + ".txt";

        // Print the current distance vectors to file
        printDistanceVectorsToFile(nodes, N, filename);

    } while (updated);

    cout << "\nRouting tables after running DVR algorithm:\n";
    printRoutingTables(nodes, N);

    // Simulate link failure
    int failSrc, failDest;
    cout << "Simulate Link Failure between\n";
    cout << "Node A: ";
    cin >> failSrc;
    // cout << "\n";
    cout << "Node B: ";
    cin >> failDest;

    // Remove the edge from edges list
    edges.erase(remove_if(edges.begin(), edges.end(), [failSrc, failDest](Edge& e) {
        return (e.src == failSrc && e.dest == failDest) || (e.src == failDest && e.dest == failSrc);
    }), edges.end());

    // Update neighbors
    nodes[failSrc].neighbors.erase(remove(nodes[failSrc].neighbors.begin(), nodes[failSrc].neighbors.end(), failDest),
                                   nodes[failSrc].neighbors.end());
    nodes[failDest].neighbors.erase(remove(nodes[failDest].neighbors.begin(), nodes[failDest].neighbors.end(), failSrc),
                                    nodes[failDest].neighbors.end());

    nodes[failSrc].distanceVector[failDest] = INFINITY;
    nodes[failDest].distanceVector[failSrc] = INFINITY;
    nodes[failSrc].nextHop[failDest] = -1;
    nodes[failDest].nextHop[failSrc] = -1;

    // Re-run the DVR algorithm until convergence or until any distance exceeds 100
    bool countToInfinity = false;
    iteration = 0; // Reset iteration counter
    do {
        updated = updateDistanceVectors(nodes, N, method);
        countToInfinity = checkCountToInfinity(nodes, N);
        if (countToInfinity) {
            cout << "Count-to-infinity problem detected.\n";
            break;
        }

        // Increment iteration counter
        iteration++;

        // Generate filename
        string filename = "distance_vectors_after_failure_iteration_" + to_string(iteration) + ".txt";

        // Print the current distance vectors to file
        printDistanceVectorsToFile(nodes, N, filename);

    } while (updated);

    cout << "\nRouting tables after link failure";
    if (method == 2) {
        cout << " with Poisoned Reverse";
    } else if (method == 3) {
        cout << " with Split Horizon";
    }
    cout << ":\n";
    printRoutingTables(nodes, N);

    return 0;
}

void initializeNodes(vector<Node>& nodes, vector<Edge>& edges, int N) {
    nodes.resize(N + 1); // Assuming nodes are numbered from 1 to N
    for (int i = 1; i <= N; ++i) {
        nodes[i].id = i;
    }
    for (auto& edge : edges) {
        nodes[edge.src].neighbors.push_back(edge.dest);
        nodes[edge.dest].neighbors.push_back(edge.src);
    }
}

void initializeDistanceVectors(vector<Node>& nodes, vector<Edge>& edges, int N) {
    for (int i = 1; i <= N; ++i) {
        nodes[i].distanceVector.clear();
        nodes[i].nextHop.clear();
        for (int j = 1; j <= N; ++j) {
            if (i == j) {
                nodes[i].distanceVector[j] = 0;
                nodes[i].nextHop[j] = j;
            } else {
                nodes[i].distanceVector[j] = INFINITY;
                nodes[i].nextHop[j] = -1;
            }
        }
    }

    // Set the costs for directly connected neighbors
    for (auto& edge : edges) {
        nodes[edge.src].distanceVector[edge.dest] = edge.cost;
        nodes[edge.src].nextHop[edge.dest] = edge.dest;

        nodes[edge.dest].distanceVector[edge.src] = edge.cost;
        nodes[edge.dest].nextHop[edge.src] = edge.src;
    }
}


bool updateDistanceVectors(vector<Node>& nodes, int N, int method) {
    vector<map<int, int>> oldDVs;
    for(int i = 1; i <= N; i++){
        oldDVs.push_back(nodes[i].distanceVector);   
    }

    for (int i = 1; i <= N; ++i) {
        for (int neighbor : nodes[i].neighbors) {
            // For each neighbor, create a copy of the distance vector to send
            map<int, int> dvToSend = nodes[i].distanceVector;

            // Need to store the old distance vector of the neigbor for calculations
            map<int, int> oldDV = nodes[neighbor].distanceVector;

            // Now, neighbor updates its distance vector based on the received dvToSend
            for(int j = 1; j <= N; j++){
                // Neighbor needs to update its distance vector for all enteries
                if(neighbor == j) continue; // Skip its own entry

                int minCost = INFINITY;
                for(int neighborsNeighbor : nodes[neighbor].neighbors){
                    // for all neighbors of this neighbor we need to find minimum for all destination nodes
                    // except for i, for them we need to use the one supplied by dvToSend
                    if(neighborsNeighbor == i){
                        if((oldDV[neighborsNeighbor] + dvToSend[j]) < minCost){
                            minCost = oldDV[neighborsNeighbor] + nodes[neighborsNeighbor].distanceVector[j];
                            nodes[neighbor].distanceVector[j] = minCost;
                            nodes[neighbor].nextHop[j] = neighborsNeighbor;
                        }
                    }else{
                        if((oldDV[neighborsNeighbor] + nodes[neighborsNeighbor].distanceVector[j]) < minCost){
                            minCost = oldDV[neighborsNeighbor] + nodes[neighborsNeighbor].distanceVector[j];
                            nodes[neighbor].distanceVector[j] = minCost;
                            nodes[neighbor].nextHop[j] = neighborsNeighbor;
                        }                        
                    }
                }
            }
        }
    }

    // To decide whether updated or not
    // we need to compare old distance vectors to new distance vectors
    for(int i = 1; i <= N; i++){
        for(int j = 1; j <= N; j++){
            if(i == j) continue;
            if(oldDVs[i - 1][j] != nodes[i].distanceVector[j]){
                return true;
            }
        }
    }
    return false;
}

void printRoutingTables(const vector<Node>& nodes, int N) {
    for (int i = 1; i <= N; ++i) {
        cout << "Routing table for Node " << i << ":\n";
        cout << "Destination\tCost\tNext Hop\n";
        for (int j = 1; j <= N; ++j) {
            if (nodes[i].distanceVector.at(j) >= INFINITY) {
                cout << j << "\t\t"
                     << "INF"
                     << "\t"
                     << "-\n";
            } else {
                cout << j << "\t\t" << nodes[i].distanceVector.at(j) << "\t" << nodes[i].nextHop.at(j) << "\n";
            }
        }
        cout << "\n";
    }
}

bool checkCountToInfinity(const vector<Node>& nodes, int N) {
    bool countToInfinity = false;
    for (int i = 1; i <= N; ++i) {
        for (auto& entry : nodes[i].distanceVector) {
            if (entry.second > 100 && entry.second < INFINITY) {
                cout << "Node " << i << " has distance >100 to Node " << entry.first << ".\n";
                countToInfinity = true;
            }
        }
    }
    return countToInfinity;
}

// Function to create directory if it doesn't exist
void createDirectoryIfNotExists(const std::string& dirName) {
    struct stat info;

    if (stat(dirName.c_str(), &info) != 0) {
        // Directory does not exist, create it
#ifdef _WIN32
        _mkdir(dirName.c_str());
#else
        mkdir(dirName.c_str(), 0755);
#endif
    } else if (info.st_mode & S_IFDIR) {
        // Directory exists
    } else {
        // Path exists but is not a directory
        cerr << dirName << " exists but is not a directory.\n";
    }
}

// Modified function to print distance vectors to a file in matrix form in "Part1" folder
void printDistanceVectorsToFile(const vector<Node>& nodes, int N, const string& filename) {
    // Ensure the directory "Part1" exists
    createDirectoryIfNotExists("Part1");

    // Construct the full file path
    string fullFilePath = "Part1/" + filename;

    ofstream outFile(fullFilePath);
    if (!outFile.is_open()) {
        cerr << "Error opening file " << fullFilePath << "\n";
        return;
    }

    // Print header
    outFile << "\t";
    for (int j = 1; j <= N; ++j) {
        outFile << j << "\t";
    }
    outFile << "\n";

    // For each node, print its distance vector
    for (int i = 1; i <= N; ++i) {
        outFile << i << "\t";
        for (int j = 1; j <= N; ++j) {
            if (nodes[i].distanceVector.at(j) >= INFINITY) {
                outFile << "INF\t";
            } else {
                outFile << nodes[i].distanceVector.at(j) << "\t";
            }
        }
        outFile << "\n";
    }

    outFile.close();
}