/**
 * Unit test using doctest. See documentation at
 * https://github.com/onqtam/doctest/blob/master/doc/markdown/tutorial.md
 */
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <strings.h>

#include <memory>
#include <minisat/core/SolverTypes.h>
#include <minisat/core/Solver.h>

#include "graph.hpp"

using namespace std;

/* ==================================================
 * Recursive n choose k implementation.
 * ================================================== */
long long nChooseK(long long n, long long k)
{
    if (n < k)
    {
        return 0;
    }

    if (k == 0)
    {
        return 1;
    }

    return (n * nChooseK(n-1, k-1)  ) / k;
}

/* ==================================================
 * Recursive implementation to obtain combinations.
 * ================================================== */
// https://www.geeksforgeeks.org/make-combinations-size-k/
void makeCombinationsUtil(vector<vector<int> > & ans, 
                            vector<int> & tmp, int n, int left, int k) 
{ 
    // Pushing this vector to a vector of vector. 
    if (k == 0) { 
        ans.push_back(tmp); 
        return; 
    } 
  
    // i iterates from left to n. First time left will be n0. 
    for (int i = left; i <= n; ++i) 
    { 
        tmp.push_back(i); 
        makeCombinationsUtil(ans, tmp, n, i + 1, k - 1); 
  
        // Popping out last inserted element from the vector.
        tmp.pop_back(); 
    } 
} 

vector<vector<int> > makeCombinations(int n0, int n, int k) 
{ 
    vector< vector<int> > ans; 
    vector<int> tmp; 

    makeCombinationsUtil(ans, tmp, n, n0, k); 

    return ans; 
} 

// 
//    | 0 1 2
// ---+------
//  0 | 0 1 1
//  1 | 1 0 0
//  2 | 1 0 0
// 
TEST_CASE("TEST CASE 1 INPUT : V 3; E {<0,1>,<0,2>};") {

    // ============================================
    // Generate a graph object for the specified 
    // number of nodes and specified edges of test 
    // case 1 input.
    // ============================================

    // Initialize nunber of nodes and edges.
    const int trueNumberOfNodes = 3;
    const int trueNumberOfEdges = 2;

    // Instantiate graph object.
    Graph * graph = new Graph(trueNumberOfNodes);

    // // Add edges.
    int coord1 = 0;
    int coord2 = 1;
    graph->addEdge(coord1, coord2);

    coord1 = 0;
    coord2 = 2;
    graph->addEdge(coord1, coord2);

    // ============================================

    //-- Initialize variables for expected results.
    bool satResult = true;
    int vertexCover = 1;
    int trueProps = vertexCover + 
                    (trueNumberOfNodes * nChooseK(vertexCover, 2)) + 
                    (vertexCover * nChooseK(trueNumberOfNodes, 2)) +
                    trueNumberOfEdges;

    // ============================================

    //-- Calculated results.

    bool res = graph->cnfSatSolver(vertexCover);
    int numberOfNodes = graph->getNumberOfNodes();
    int numberOfEdges = graph->getNumberOfEdges();
    int numberOfProps = graph->getNumberOfProps();
    string vertexCoverString = graph->getVertexCoverString();

    // ============================================
    //  Test Case 1

    CHECK(satResult == res);
    CHECK(trueNumberOfNodes == numberOfNodes);
    CHECK(trueNumberOfEdges == numberOfEdges);
    CHECK(trueProps == numberOfProps);
    CHECK(vertexCoverString == "0");

    graph->resetGraph();
    delete graph;
    graph = nullptr;

    // ============================================
}

// 
//    | 0 1 2
// ---+------
//  0 | 0 1 1
//  1 | 1 0 1
//  2 | 1 1 0
// 
TEST_CASE("TEST CASE 2 INPUT : V 3; E {<0,1>,<0,2>,<1,2>};") {

    // ============================================
    // Generate a graph object for the specified 
    // number of nodes and specified edges of test 
    // case 2 input.
    // ============================================

    // Initialize nunber of nodes and edges.
    const int trueNumberOfNodes = 3;
    const int trueNumberOfEdges = 3;

    // Instantiate graph object.
    Graph * graph = new Graph(trueNumberOfNodes);

    // // Add edges.
    int coord1 = 0;
    int coord2 = 1;
    graph->addEdge(coord1, coord2);

    coord1 = 0;
    coord2 = 2;
    graph->addEdge(coord1, coord2);

    coord1 = 1;
    coord2 = 2;
    graph->addEdge(coord1, coord2);

    // ============================================

    //-- Initialize variables for expected results.
    bool satResult = true;
    int vertexCover = 2;
    int trueProps = vertexCover + 
                    (trueNumberOfNodes * nChooseK(vertexCover, 2)) + 
                    (vertexCover * nChooseK(trueNumberOfNodes, 2)) +
                    trueNumberOfEdges;

    // ============================================

    //-- Calculated results.

    bool res = graph->cnfSatSolver(vertexCover);
    int numberOfNodes = graph->getNumberOfNodes();
    int numberOfEdges = graph->getNumberOfEdges();
    int numberOfProps = graph->getNumberOfProps();
    string vertexCoverString = graph->getVertexCoverString();

    // ============================================
    //  Test Case 2

    CHECK(satResult == res);
    CHECK(trueNumberOfNodes == numberOfNodes);
    CHECK(trueNumberOfEdges == numberOfEdges);
    CHECK(trueProps == numberOfProps);
    CHECK( ( vertexCoverString == "0 1" || vertexCoverString == "0 2" || vertexCoverString == "0 2" ) );

    graph->resetGraph();
    delete graph;
    graph = nullptr;

    // ============================================
}

// 
//    | 0 1 2 3 4
// ---+----------
//  0 | 0 0 0 1 1
//  1 | 0 0 0 1 1
//  2 | 0 0 0 1 0
//  3 | 1 1 1 0 1
//  4 | 1 1 0 1 0
// 
TEST_CASE("TEST CASE 3 INPUT : V 5; E {<0,3>,<0,4>,<1,3>,<1,4>,<2,3>,<3,4>};") {

    // ============================================
    // Generate a graph object for the specified 
    // number of nodes and specified edges of test 
    // case 3 input.
    // ============================================

    // Initialize nunber of nodes and edges.
    const int trueNumberOfNodes = 5;
    const int trueNumberOfEdges = 6;

    // Instantiate graph object.
    Graph * graph = new Graph(trueNumberOfNodes);

    // // Add edges.
    int coord1 = 0;
    int coord2 = 3;
    graph->addEdge(coord1, coord2);

    coord1 = 0;
    coord2 = 4;
    graph->addEdge(coord1, coord2);

    coord1 = 1;
    coord2 = 3;
    graph->addEdge(coord1, coord2);

    coord1 = 1;
    coord2 = 4;
    graph->addEdge(coord1, coord2);

    coord1 = 2;
    coord2 = 3;
    graph->addEdge(coord1, coord2);

    coord1 = 3;
    coord2 = 4;
    graph->addEdge(coord1, coord2);

    // ============================================

    //-- Initialize variables for expected results.
    bool satResult = true;
    int vertexCover = 3;
    int trueProps = vertexCover + 
                    (trueNumberOfNodes * nChooseK(vertexCover, 2)) + 
                    (vertexCover * nChooseK(trueNumberOfNodes, 2)) +
                    trueNumberOfEdges;

    // ============================================

    //-- Calculated results.

    bool res = graph->cnfSatSolver(vertexCover);
    int numberOfNodes = graph->getNumberOfNodes();
    int numberOfEdges = graph->getNumberOfEdges();
    int numberOfProps = graph->getNumberOfProps();
    string vertexCoverString = graph->getVertexCoverString();

    // ============================================
    //  Test Case 3

    CHECK(satResult == res);
    CHECK(trueNumberOfNodes == numberOfNodes);
    CHECK(trueNumberOfEdges == numberOfEdges);
    CHECK(trueProps == numberOfProps);
    CHECK( ( vertexCoverString == "0 1 3") );

    graph->resetGraph();
    delete graph;
    graph = nullptr;

    // ============================================
}

// 
//    | 0 1 2 3 4 5 6 7 8 9
// ---+--------------------
//  0 | 0 1 0 0 0 0 0 0 0 0
//  1 | 1 0 0 0 0 0 0 0 0 0
//  2 | 0 0 0 1 0 0 0 0 0 0
//  3 | 0 0 1 0 0 0 0 0 0 0
//  4 | 0 0 0 0 0 1 0 0 0 0
//  5 | 0 0 0 0 1 0 0 0 0 0
//  6 | 0 0 0 0 0 0 0 1 0 0
//  7 | 0 0 0 0 0 0 1 0 0 0
//  8 | 0 0 0 0 0 0 0 0 0 1
//  9 | 0 0 0 0 0 0 0 0 1 0
//
TEST_CASE("TEST CASE 4 INPUT : V 10; E {<0,1>,<2,3>,<4,5>,<6,7>,<8,9>};") {

    // ============================================
    // Generate a graph object for the specified 
    // number of nodes and specified edges of test 
    // case 3 input.
    // ============================================

    // Initialize nunber of nodes and edges.
    const int trueNumberOfNodes = 10;
    const int trueNumberOfEdges = 5;

    // Instantiate graph object.
    Graph * graph = new Graph(trueNumberOfNodes);

    // // Add edges.
    int coord1 = 0;
    int coord2 = 1;
    graph->addEdge(coord1, coord2);

    coord1 = 2;
    coord2 = 3;
    graph->addEdge(coord1, coord2);

    coord1 = 4;
    coord2 = 5;
    graph->addEdge(coord1, coord2);

    coord1 = 6;
    coord2 = 7;
    graph->addEdge(coord1, coord2);

    coord1 = 8;
    coord2 = 9;
    graph->addEdge(coord1, coord2);

    // ============================================

    //-- Initialize variables for expected results.
    bool satResult = true;
    int vertexCover = 5;
    int trueProps = vertexCover + 
                    (trueNumberOfNodes * nChooseK(vertexCover, 2)) + 
                    (vertexCover * nChooseK(trueNumberOfNodes, 2)) +
                    trueNumberOfEdges;

    // ============================================

    //-- Calculated results.

    bool res = graph->cnfSatSolver(vertexCover);
    int numberOfNodes = graph->getNumberOfNodes();
    int numberOfEdges = graph->getNumberOfEdges();
    int numberOfProps = graph->getNumberOfProps();
    string vertexCoverString = graph->getVertexCoverString();

    // ============================================
    //  Test Case 4

    CHECK(satResult == res);
    CHECK(trueNumberOfNodes == numberOfNodes);
    CHECK(trueNumberOfEdges == numberOfEdges);
    CHECK(trueProps == numberOfProps);
    CHECK( ( vertexCoverString == "0 2 4 6 8") );

    graph->resetGraph();
    delete graph;
    graph = nullptr;

    // ============================================



}

// 
//    | 0 1 2 3 4
// ---+----------
//  0 | 0 1 1 1 1
//  1 | 1 0 1 1 1
//  2 | 1 1 0 1 1
//  3 | 1 1 1 0 1
//  4 | 1 1 1 1 0
// 
TEST_CASE("TEST 6 : V 5; E {<0,1>,<0,2>,<0,3>,<0,4>,<1,2>,<1,3>,<1,4>,<2,3>,<2,4>,<3,4>};") {

    // ============================================
    // Generate a graph object for the specified 
    // number of nodes and specified edges of test 
    // case 3 input.
    // ============================================

    // Initialize nunber of nodes and edges.
    const int trueNumberOfNodes = 5;
    const int trueNumberOfEdges = 10;

    // Instantiate graph object.
    Graph * graph = new Graph(trueNumberOfNodes);

    // // Add edges.
    int coord1 = 0;
    int coord2 = 1;
    graph->addEdge(coord1, coord2);

    coord1 = 0;
    coord2 = 2;
    graph->addEdge(coord1, coord2);

    coord1 = 0;
    coord2 = 3;
    graph->addEdge(coord1, coord2);

    coord1 = 0;
    coord2 = 4;
    graph->addEdge(coord1, coord2);

    coord1 = 1;
    coord2 = 2;
    graph->addEdge(coord1, coord2);

    coord1 = 1;
    coord2 = 3;
    graph->addEdge(coord1, coord2);

    coord1 = 1;
    coord2 = 4;
    graph->addEdge(coord1, coord2);

    coord1 = 2;
    coord2 = 3;
    graph->addEdge(coord1, coord2);

    coord1 = 2;
    coord2 = 4;
    graph->addEdge(coord1, coord2);

    coord1 = 3;
    coord2 = 4;
    graph->addEdge(coord1, coord2);

    // ============================================

    //-- Initialize variables for expected results.
    bool satResult = false;
    int vertexCover = 3;
    int trueProps = vertexCover + 
                    (trueNumberOfNodes * nChooseK(vertexCover, 2)) + 
                    (vertexCover * nChooseK(trueNumberOfNodes, 2)) +
                    trueNumberOfEdges;

    // ============================================

    //-- Calculated results.

    bool res = graph->cnfSatSolver(vertexCover);
    int numberOfNodes = graph->getNumberOfNodes();
    int numberOfEdges = graph->getNumberOfEdges();
    int numberOfProps = graph->getNumberOfProps();
    string vertexCoverString = graph->getVertexCoverString();

    // ============================================
    //  Test Case 4

    CHECK(satResult == res);
    CHECK(trueNumberOfNodes == numberOfNodes);
    CHECK(trueNumberOfEdges == numberOfEdges);
    CHECK(trueProps == numberOfProps);
    CHECK( ( vertexCoverString == "") );

    graph->resetGraph();
    delete graph;
    graph = nullptr;

    // ===========================================
}

//     //-- Init.
//     const int numNodes = 5;
//     const int weight   = 1;

//     // ============================================
    
//     std::unique_ptr< Graph > graph(new Graph(numNodes));

//     graph->add_edge(0, 1, weight);
//     graph->add_edge(0, 2, weight);
//     graph->add_edge(0, 3, weight);
//     graph->add_edge(0, 4, weight);
//     graph->add_edge(1, 2, weight);
//     graph->add_edge(1, 3, weight);
//     graph->add_edge(1, 4, weight);
//     graph->add_edge(2, 3, weight);
//     graph->add_edge(2, 4, weight);
//     graph->add_edge(3, 4, weight);

//     // ============================================

//     //-- Run the graph with varrying number of propositions.
//     bool sat = false;
//     int numCover = 0;
//     int numProp  = 0;
//     std::string cover = "";
//     std::vector< int > used;
//     long long realProp = 0;


//     // ============================================
//     //  Test Case 6A
//     //   -- CNF-SAT-VC
//     // ============================================

//     //-- Run for specified 
//     used.clear();
//     numCover = 3;
//     sat = graph->CNF_SAT_VC(used, numProp, numCover);
//     realProp = numCover + numNodes * C(numCover, 2) + numCover * C(numNodes, 2) + graph->getNumEdges();
//     cover_to_string(used, cover);

//     //-- Correct Answer?
//     CHECK(sat == false);
//     CHECK(numProp == realProp);
//     CHECK(cover == "");



//     // ============================================
//     //  Test Case 6B
//     //   -- CNF-SAT-VC
//     // ============================================
//     //-- Run for specified 
//     used.clear();
//     numCover = 4;
//     sat = graph->CNF_SAT_VC(used, numProp, numCover);
//     realProp = numCover + numNodes * C(numCover, 2) + numCover * C(numNodes, 2) + graph->getNumEdges();
//     cover_to_string(used, cover);

//     //-- Correct Answer?
//     CHECK(sat == true);
//     CHECK(numProp == realProp);
//     CHECK(cover == "0 1 2 3");


//     // ============================================

//     //-- Delete the pointer.
//     graph.reset();
// }