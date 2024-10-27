# Distance Vector Routing Algorithm with Link Failure, Poisoned Reverse, and Split Horizon

**CS344 Assignment 5: Submission by Group 37**

- **Adarsh Gupta (220101003)**
- **Tanvi Doshi (220101)**
- **Vasudha**
- **Yash Jain**

## Overview

This project implements the **Distance Vector Routing (DVR) Algorithm** with additional features to handle network link failures and prevent routing loops. The implemented solution addresses common routing issues such as the *count-to-infinity* problem using techniques like *Poisoned Reverse* and *Split Horizon*. The DVR simulation showcases how routing tables update in response to a link failure and how these techniques help mitigate loop-related routing issues.

### Features

- **Distance Vector Routing (DVR) Algorithm**: Computes shortest paths between routers in a network graph.
- **Link Failure Simulation**: Observes changes in routing tables following a simulated edge break.
- **Count-to-Infinity Detection**: Identifies routing loops by tracking distance vectors exceeding a threshold.
- **Poisoned Reverse Technique**: Reduces routing loops by advertising unreachable routes to certain neighbors.
- **Split Horizon Technique**: Prevents count-to-infinity issues by restricting specific routing information to neighbors.

## Project Structure

- `Part1.cpp`: Implements the basic DVR algorithm and link failure simulation.
- `Part2.cpp`: Extends Part1 with the Poisoned Reverse mechanism.
- `Part3.cpp`: Extends Part1 with the Split Horizon mechanism.
- `Makefile`: Compiles and runs each part of the simulation.
- `README.md`: (This file) Provides an overview of the project.
- `report.pdf`: Contains a detailed explanation of the implementation, sample output, and conclusions.

## Getting Started

### Prerequisites

- **Compiler**: `g++` (for C++ files).
- **Make**: For compiling the project via the provided Makefile.

### Compilation

To compile the project, navigate to the project directory and run:

```bash
make
```

This will create executable files for `Part1`, `Part2`, and `Part3`.

### Usage

1. **Running Part 1 (DVR with Link Failure):**

   ```bash
   make part1
   ```

   This command runs the basic DVR algorithm, simulates a link failure, and outputs routing tables before and after the failure.
2. **Running Part 2 (DVR with Poisoned Reverse):**

   ```bash
   make part2
   ```

   This command runs the DVR algorithm with the Poisoned Reverse technique. It shows the effect of Poisoned Reverse on routing tables following a link failure.
3. **Running Part 3 (DVR with Split Horizon):**

   ```bash
   make part3
   ```

   This command runs the DVR algorithm with the Split Horizon technique, illustrating how Split Horizon impacts routing tables after a link failure.
4. **Cleaning Up**: To remove compiled files, use:

   ```bash
   make clean
   ```

Inputs Required:
Number of Routers, Number of Links

[Src, Dest, Edge Cost]*Number of Links

After this the algorithm will run and calculate the optimal DVs and then prompt you to add Nodes A and B to simulate link failures between these 2 nodes.

## Code Overview

### Initialization

- **`initializeNodes()`**: Sets up network nodes and neighbors based on input edges.
- **`initializeDistanceVectors()`**: Initializes distance vectors and next-hop tables for each node.

### DVR Update Loop

- **`updateDistanceVector()`**: Updates routing tables based on neighbors' distance vectors.
- **Count-to-Infinity Check**: Monitors if routing tables encounter distances exceeding 100, signaling potential routing loops.

### Poisoned Reverse and Split Horizon

1. **Poisoned Reverse**:
   - Modifies `updateDistanceVector()` to advertise unreachable routes (distance set to infinity) to specific neighbors, preventing routing loops.
2. **Split Horizon**:
   - Prevents a router from sharing route information back to the source of that route, helping to avoid count-to-infinity issues.

---

For more information refer to the report attached.
