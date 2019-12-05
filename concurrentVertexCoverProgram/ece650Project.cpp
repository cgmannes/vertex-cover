/* ==========================================================================
 *
 * Author: Christopher Mannes
 * Student ID: 20743505
 *
 * ECE 650 - Methods and Tools for Software Engineering
 *
 * Assignment 4 - A polynomial-time reduction from VERTEX-COVER to CNF-SAT
 *
 * Solves the vertex cover problem for an input graph G and determines the
 * smallest an integer k for the vertex cover such that there is a reduction
 * to a CNF formula F that is satisfiable. F is provided as an input to the 
 * mini-sat SAT solver.
 *
 * ========================================================================== */

/**
 * Import libraries.
 */
#include <iostream>
#include <sstream> 
#include <vector> 
#include <list>
#include <iterator>
#include <memory>
#include <cstdio>
#include <pthread.h>
// #include <thread>

/**
 * Import header files.
 */
#include <stdio.h>
#include <strings.h>

/**
 * User defined header files.
 */
// Defines Var and Lit
#include "minisat/core/SolverTypes.h"
// Defines Solver
#include "minisat/core/Solver.h"
// Graph header file.
#include "graph.hpp"

using namespace std;

// Global variables.
Graph * inputGraph = nullptr;
bool reset = false;

// N choose k function prototype.
long long nChooseK(long long n, long long k);

// Make combinations function prototype.
vector< vector<int> > makeCombinations(int n0, int n, int K);


/* ==================================================
 * Function for sending an error message to stderr.
 * ================================================== */
void error(string str)
{
    // Output error message.
    cerr << str << endl;

    exit(0);
}

/* ==================================================
 * I/O thread for processing input and output.
 * ================================================== */
// int inputOutput(bool & reset)
void * inputOutput(void *)
{
    string line;
    string inputFlag;
    string inputData;

    // cin >> inputFlag;
    getline(cin, line);
    inputFlag = line[0];

    if (cin.eof()){ exit(0); }

    if (inputFlag[0] == 'V')
    {
        if (reset)
        {
            // cin >> inputData;
            inputData = line.substr(2, line.size() - 2);
            int numberOfNodes = stoi(inputData);

            inputGraph->resetGraph();
            delete inputGraph;
            inputGraph = nullptr;

            inputGraph = new Graph(numberOfNodes);
        }
        else
        {
            reset = true;
            // cin >> inputData;
            inputData = line.substr(2, line.size() - 2);
            int numberOfNodes = stoi(inputData);

            inputGraph = new Graph(numberOfNodes);
        }
    }

    // cin >> inputFlag;
    getline(cin, line);
    inputFlag = line[0];

    if (inputFlag[0] == 'E')
    {
        // cin >> inputData;
        // inputData = inputData.substr(2, inputData.size() - 3);
        inputData = line.substr(2, line.size() - 2);
        inputData = inputData.substr(2, inputData.size() - 3);
        inputData.append(",<");
        // cout <<inputData << endl;

        string delim = ">,<";
        string delim2 = ",";

        int start = 0;
        int end = inputData.find(delim);

        while (end != string::npos)
        {
            string pair = inputData.substr(start, end - start);
            int comma = pair.find(delim2);

            int coord1 = stoi(pair.substr(0, comma));
            int coord2 = stoi(pair.substr(comma + 1, pair.length() - comma));

            if (coord1 < (inputGraph->getNumberOfNodes()) && coord2 < (inputGraph->getNumberOfNodes()))
            {
                inputGraph->addEdge(coord1, coord2);
            }
            else if (coord1 > (inputGraph->getNumberOfNodes()) && coord2 > (inputGraph->getNumberOfNodes()))
            {
                int j = inputGraph->getNumberOfNodes();
                cout << j << endl;
                cout 
                << "Error: For specified edge (" 
                << coord1 << "," << coord2 
                << "), vertices " << coord1 << " and " << coord2 << " do not exist." << endl;
            }
            else if (coord1 > (inputGraph->getNumberOfNodes()))
            {
                cout 
                << "Error: For specified edge (" 
                << coord1 << "," << coord2 
                << "), vertex " << coord1 << " does not exist." << endl;
            }
            else if (coord2 > (inputGraph->getNumberOfNodes()))
            {
                cout 
                << "Error: For specified edge (" 
                << coord1 << "," << coord2 
                << "), vertex " << coord2 << " does not exist." << endl;
            }

            start = end + delim.length();
            end = inputData.find(delim, start);

        }
        // Output adjacency list.
        inputGraph->displayAdjList();
    }

    return 0;
}

/* ==================================================
 * Thread for the Minisat solver.
 * ================================================== */
// int cnfSat()
void * cnfSat(void *)
{
    //Determine the number of nodes in the vertex cover using Minisat sovler
    // and which nodes the cover consists of.
    for (int k = 1; k < inputGraph->getNumberOfNodes() + 1; k++)
    {
        bool satResult = inputGraph->cnfSatSolver(k);

        if (satResult == 1)
        {
            break;
        }
    }

    return 0;
}

/* ==================================================
 * Thread for the approx method 1.
 * ================================================== */
// int approx1VC()
void * approx1VC(void *)
{
    //Determine the number of nodes in the vertex cover using approx1 method
    // and which nodes the cover consists of.
    inputGraph->approx1();

    return 0;
}

/* ==================================================
 * Thread for the approx method 2.
 * ================================================== */
// int approx2VC()
void * approx2VC(void *)
{
    //Determine the number of nodes in the vertex cover using approx2 method
    // and which nodes the cover consists of.
    inputGraph->approx2();

    return 0;
}

/* ==================================================
 * Enter main program.
 * ================================================== */
int main(int argc, char * argv[]) {

    // bool reset = false;

    pthread_t ioThread, cnfThread, approx1Thread, approx2Thread;

    while (!cin.eof())
    {
        // thread io(inputOutput, std::ref(reset));    
        pthread_create(&ioThread, NULL, &inputOutput, NULL);

        // io.join();
        pthread_join(ioThread, NULL);
        
        // thread cnf_sat_vc(cnfSat);
        // thread approx_vc_1(approx1VC);
        // thread approx_vc_2(approx2VC);
        pthread_create(&cnfThread, NULL, &cnfSat, NULL);
        pthread_create(&approx1Thread, NULL, &approx1VC, NULL);
        pthread_create(&approx2Thread, NULL, &approx2VC, NULL);

        // // cnf_sat_vc.join();
        // // approx_vc_1.join();
        // // approx_vc_2.join();
        pthread_join(cnfThread, NULL);
        pthread_join(approx1Thread, NULL);
        pthread_join(approx2Thread, NULL);
    }

    inputGraph->resetGraph();
    delete inputGraph;
    inputGraph = nullptr;

    return 0;
}

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


// V 15
// E {<2,6>,<2,8>,<2,5>,<6,5>,<5,8>,<6,10>,<10,8>}
// E {<2,6>,<2,8>,<2,5>,<16,17>,<6,20>,<30,8>,<6,5>,<5,8>,<6,10>,<10,8>}
// s 2 10
//
// V 5
// E {<0,2>,<2,1>,<2,3>,<3,4>,<4,1>}
// E {<0,2>,<6,1>,<2,3>,<3,4>,<4,1>}
// s 4 0
//
// V 5
// E {<0,1>,<1,2>,<0,3>,<1,3>,<1,4>}
// V 7
// E {<0,3>,<0,6>,<1,5>,<2,3>,<3,4>,<3,5>,<4,6>,<5,6>}