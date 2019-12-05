/* ==================================================
 *  Member function implementations for class Graph.
 * ================================================== */

/**
 * Import libraries.
 */
#include <iostream>
#include <sstream> 
#include <vector> 
#include <list>
#include <iterator>
#include <memory>

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
 * Graph constructor.
 * ================================================== */
Graph::Graph(int numberOfNodes, int numberOfEdges, int numberOfProps)
{
    this->numberOfNodes = numberOfNodes;

    this->numberOfEdges = numberOfEdges;
    
    this->numberOfProps = numberOfProps;

    this->vertexCoverString = "";

    adjList = new list<int>[numberOfNodes];

    parent = new int[numberOfNodes];
    
}

/* ==================================================
 * Addes edges for an undirected graph.
 * ================================================== */
void Graph::addEdge(int & u, int & v)
{
    adjList[u].push_back(v);
    adjList[v].push_back(u);
    numberOfEdges++;
}

/* ==================================================
 * Function for displaying the adjacency list.
 * ================================================== */
void Graph::displayAdjList()
{
    // Method for displaying adjList.
    for (int i = 0; i < numberOfNodes; i++)
    {
        cout << i << "-->";

        // vector<int> :: iterator it;
        list<int> :: iterator it;
        for (it = adjList[i].begin(); it != adjList[i].end(); it++)
        {
            cout << *it << " ";
        }
        cout << endl;

    }
}

/* ==================================================
 * Performs the breadth first search algorithm.
 * ================================================== */
void Graph::BFS(int s, int target)
{
    // Generate a boolean array with all elements set to false.
    bool * visited = new bool[numberOfNodes];
    for (int i = 0; i < numberOfNodes; i++)
    {
        visited[i] = false;
    }

    list<int> queue;
    list<int> parentQueue;
    
    visited[s] = true;
    queue.push_back(s);

    // vector<int> :: iterator i;
    list<int> :: iterator i;

    bool firstIter = true;

    while (!queue.empty())
    {
        s = queue.front();
        queue.pop_front();

        if (firstIter)
        {
            parent[s] = -1;
            firstIter = false;
        }
        else
        {
            parent[s] = parentQueue.front();
            parentQueue.pop_front();
        }
        
        for ( i = adjList[s].begin(); i != adjList[s].end(); i++)
        {
            if (!visited[*i])
            {
                visited[*i] = true;
                queue.push_back(*i);
                parentQueue.push_back(s);
            }
        }
    }
    delete[] visited;
}

/* ==================================================
 * Displays the shortest path.
 * ================================================== */
void Graph::displayPath(int & s, int & t)
{
    for (int j = 0; j < numberOfNodes; j++)
    {
        // cout << parent[j] << endl;

        int counter = 0;
        list<int> :: iterator it;
        for (it = adjList[j].begin(); it != adjList[j].end(); it++)
        {
            counter++;
        }

        if (counter == 0)
        {
            parent[j] = -2;
        }
        counter = 0;
        // cout << parent[j] << endl;
        // cout << "------------" << endl;
        
    }
    
    // cout << t; 
    list<int> outputList;
    outputList.push_front(t);
    while (true)
    {
        t = pathReverse(t);

        if (t == -2)
        {
            outputList.clear();
            cout << "Error: No connected path exists for specified source and target." << endl;
            break;
        }
        else if (t < 0)
        {
            break;
        }
        outputList.push_front(t);
        // cout << "-" << t;

    }
    // cout << endl;
    if (!outputList.empty())
    {
        list<int> :: iterator i;
        for (i = outputList.begin(); i != prev(outputList.end()); i++)
        {
            cout << *i << "-";
        }
        cout << outputList.back() << endl;
    }
}

/* ==================================================
 * Reverse the path for obtaining the start node.
 * ================================================== */
int Graph::pathReverse(int currentNode)
{
    int nextNode = parent[currentNode];
    // cout << currentNode << "-->" << nextNode << endl;

    return nextNode;
}

/* ==================================================
 * Reduction of input graph to a set of clauses.
 * ================================================== */
bool Graph::cnfSatSolver(int & k)
{
    // Minisat solver object.
    std::unique_ptr<Minisat::Solver> solver(new Minisat::Solver());

    // Minisat vector for storing clauses.
    Minisat::vec<Minisat::Lit> clause;

    // Vector for generating Minisat variables.
    vector<Minisat::Var> satVars;
    for (int i = 0; i < (k*numberOfNodes); i++)
    {
        satVars.push_back(solver->newVar());
    }

    // Generate k clauses for the k term in the reduction formula.
    int counter = 0;
    for (int i = 1; i < k + 1; i++)
    {
        for (int ind =  1 + counter; ind < (k*numberOfNodes + 1); ind += k)
        {
            clause.push(Minisat::mkLit(satVars[ind - 1]));
            // cout << ind << " ";
        }
        // cout << endl;
        solver->addClause(clause);
        numberOfProps++;
        clause.clear();
        counter++;
    }


    // Clauses that correspond to n*(k choose 2).
    long long k2 = nChooseK(k, 2);

    if (k2 != 0)
    {
        for (int i = 1; i < (k*numberOfNodes + 1); i += k)
        {
            int beginComb = i;
            int endComb = i + k - 1;

            vector< vector<int> > result;
            result = makeCombinations(beginComb, endComb, 2);

            // Printout code for result.
            for (int i = 0; i < result.size(); i++) { 
                for (int j = 0; j < result[i].size(); j++) { 
                    // cout << result.at(i).at(j) << " "; 
                    clause.push(~Minisat::mkLit( satVars[result.at(i).at(j) - 1] ));
                } 
                // cout << endl; 
                solver->addClause(clause);
                numberOfProps++;
                clause.clear();
            }
        }
    }


    // Clauses the correspond to k*(n choose 2).
    long long n2 = nChooseK(numberOfNodes, 2);

    if (n2 != 0)
    {
        for (int a = 1; a < k + 1; a++)
        {
            for (int b = a; b < k*numberOfNodes + 1; b += k)
            {
                for (int c = b + k; c < k*numberOfNodes + 1; c += k)
                {
                    clause.push(~Minisat::mkLit( satVars[b - 1] ));
                    clause.push(~Minisat::mkLit( satVars[c - 1] ));

                    solver->addClause(clause);
                    numberOfProps++;
                    clause.clear();

                    // cout << b << " " << c << endl;
                }
            }
        }
    }


    // Clauses that correspond to the number of edges.
    // The vector, record is a vector of nodes already processed.
    vector<int> record;
    
    for (int i = 0; i < numberOfNodes; i++)
    {
        list<int> :: iterator it;
        for (it = adjList[i].begin(); it != adjList[i].end(); it++)
        {
            if (count(record.begin(), record.end(), *it))
            {
                continue;
            }
            else 
            {
                for (int I = i*k + 1; I < i*k + 1 + k; I++)
                {
                    clause.push(Minisat::mkLit( satVars[I - 1] ));
                    // cout << I << " ";
                }

                for (int I = (*it)*k + 1; I < (*it)*k + 1 + k; I++)
                {
                    clause.push(Minisat::mkLit( satVars[I - 1] ));
                    // cout << I << " ";
                }
                record.push_back(i);
            }
            // cout << endl;
            solver->addClause(clause);
            numberOfProps++;
            clause.clear();
        }
    }

    // Get the Minisat solution.
    bool res = solver->solve();

    // Determine the vertices in the vertex cover.
    int coverCounter = 0;
    std::ostringstream s;
    bool first = true;
    if (res == 1)
    {
        for (int i = 0; i < (k*numberOfNodes); i++)
        {
            if ((i % k) == 0 && i != 0)
            {
                coverCounter++;
            }

            bool nodeInVertexCover = solver->modelValue(satVars[i]) == Minisat::l_True;
            if (nodeInVertexCover)
            {
                if (!first)
                {
                    vertexCoverString.append(" ");
                }
                cout << coverCounter << " ";
                s << coverCounter;
                vertexCoverString.append(s.str());
                s.str(std::string());
                first = false;
            }
        }
        cout << endl;
    }

    return res;
}

/* ==================================================
 * Reset graph for new input.
 * ================================================== */
void Graph::resetGraph()
{
    delete[] parent;

    delete[] adjList;

}

/* ==================================================
 * Accessor function for getting number of nodes.
 * ================================================== */
int Graph::getNumberOfNodes()
{
    return numberOfNodes;
}

/* ==================================================
 * Accessor function for getting number of edges.
 * ================================================== */
int Graph::getNumberOfEdges()
{
    return numberOfEdges;
}

/* ==================================================
 * Accessor function for getting number of propositions.
 * ================================================== */
int Graph::getNumberOfProps()
{
    return numberOfProps;
}

/* ==================================================
 * Accessor function for getting number of propositions.
 * ================================================== */
string Graph::getVertexCoverString()
{
    return vertexCoverString;
}