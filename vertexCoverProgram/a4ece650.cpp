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
 * Enter main program.
 * ================================================== */
int main(int argc, char * argv[]) {

    // freopen( "output.txt", "w", stdout );

    string inputFlag;
    string inputData;
    Graph * inputGraph = nullptr;

    bool reset = false;

    while (true)
    {
        cin >> inputFlag;

        if (inputFlag[0] == 'V')
        {
            if (reset)
            {
                // cout << "sec" << endl;
                cin >> inputData;
                int numberOfNodes = stoi(inputData);

                inputGraph->resetGraph();
                delete inputGraph;
                inputGraph = nullptr;

                inputGraph = new Graph(numberOfNodes);
            }
            else
            {
                reset = true;
                // cout << "first" << endl;
                cin >> inputData;
                int numberOfNodes = stoi(inputData);

                inputGraph = new Graph(numberOfNodes);
            }

            // cin >> inputData;
            // int numberOfNodes = stoi(inputData);

            // inputGraph = new Graph(numberOfNodes);

        }
        else if (inputFlag[0] == 'E')
        {
            cin >> inputData;
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

                // inputGraph->addEdge(coord1, coord2);

                // int j = inputGraph->getNumberOfNodes();

                // cout << "before " << j << endl;
                // cout << coord1 << " and " << coord2 << endl;
                // cout << "---------------------------------" << endl;

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

            
            //Determine the number of nodes in the vertex cover.
            for (int k = 1; k < inputGraph->getNumberOfNodes() + 1; k++)
            {
                bool satResult = inputGraph->cnfSatSolver(k);

                if (satResult == 1)
                {
                    break;
                }
            }
        }
        else if (inputFlag[0] == 'q')
        {
            inputGraph->resetGraph();
            delete inputGraph;
            inputGraph = nullptr;
            // std::fclose(stdout);

            return 0;
        }
    }

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