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
#include <mutex> 

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

// Initialize mutex.
mutex mtx;

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

    this->vertexCoverString = "CNF-SAT-VC: ";

    adjList = new list<int>[numberOfNodes];
    adjListApprox1 = new list<int>[numberOfNodes];
    adjListApprox2 = new list<int>[numberOfNodes];

    parent = new int[numberOfNodes];
    
}

/* ==================================================
 * Addes edges for an undirected graph.
 * ================================================== */
void Graph::addEdge(int & u, int & v)
{
    adjList[u].push_back(v);
    adjList[v].push_back(u);

    adjListApprox1[u].push_back(v);
    adjListApprox1[v].push_back(u);

    adjListApprox2[u].push_back(v);
    adjListApprox2[v].push_back(u);

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
 * Reduction of input graph to a set of clauses by minisat.
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
        // cout << "CNF-SAT-VC: " << endl;
        vector<string> cnfVector;
        cnfVector.push_back("CNF-SAT-VC: ");
        vertexCoverString = "CNF-SAT-VC: ";
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
                    cnfVector.push_back(" ");
                }
                // cout << coverCounter << " ";
                s << coverCounter;
                vertexCoverString.append(s.str());
                cnfVector.push_back(s.str());
                // This is to reset s to be empty.
                s.str(std::string());
                first = false;
            }
        }
        // cout << endl;
        // cout << vertexCoverString << endl;
        printFunction(cnfVector);
    }

    return res;
}

/* ==================================================
 * Clear edges for approx1.
 * 
 * Removes the edges that are incident to the highest
 * degree vertex (indicated by int row) and returns
 * the reduced adjacency list.
 * ================================================== */
void Graph::clearEdgesApprox1(int row)
{
    // Declare an iterator for the adjacency list row
    // and a vector for collecting the values.
    list<int>::iterator it;
    vector<int> approx1EdgesList;
    for (it = adjListApprox1[row].begin(); it != adjListApprox1[row].end(); it++)
    {
        approx1EdgesList.push_back(*it);
    }

    // Remove the values of int row from adjListApprox1
    // for all the values in approx1EdgesList.
    for (int i = 0; i < approx1EdgesList.size(); i++)
    {
        adjListApprox1[approx1EdgesList[i]].remove(row);
    }
}

/* ==================================================
 * Clear edges for approx2.
 * 
 * Removes the edges in the adjacency matrix that 
 * contain the vertices u or v.
 * ================================================== */
void Graph::clearEdgesApprox2(int u, int v)
{
    for (int i = 0; i < numberOfNodes; i++)
    {
        adjListApprox2[i].remove(u);
        adjListApprox2[i].remove(v);
    }
}

/* ==================================================
 * Checks if the adjacency matrix is empty.
 * 
 * Flag == 1 indicates to check the adjacency matrix
 * associated with approx1 method.
 * 
 * Flag == 2 indicates to check the adjacency matrix
 * associated with approx2 method.
 * ================================================== */
bool Graph::isClear(int flag)
{
    int clearCounter = 0;

    if (flag == 1)
    {
        for (int i = 0; i < numberOfNodes; i++)
        {
            if (adjListApprox1[i].size() == 0)
            {
                clearCounter++;
            }
        }
    }
    else if (flag == 2)
    {
        for (int i = 0; i < numberOfNodes; i++)
        {
            if (adjListApprox2[i].size() == 0)
            {
                clearCounter++;
            }
        }
    }

    if (clearCounter == numberOfNodes)
    {
        return true;
    }
    else
    {
        return false;
    }
    
}

/* ==================================================
 * Vertex cover obtained using approx1 method.
 * ================================================== */
void Graph::approx1()
{
    // Bool values for while loop and a vector for 
    // collecting the vertices in the vertex cover.
    bool approx1Iter = false;
    vector<int> approx1VC;

    while (!approx1Iter)
    {
        // Initialize values determining the highest
        // degree vertex.
        int row = -1;
        int ithDegree = 0;
        int maxDegree = 0;

        // Highest degree vertex is assigned to int row.
        for (int i = 0; i < numberOfNodes; i++)
        {
            ithDegree = adjListApprox1[i].size();
            // cout << "ithDegree: " << ithDegree << " for " << i << endl;

            if ( ithDegree > maxDegree )
            {
                maxDegree = ithDegree;
                // cout << "maxDegree: " << ithDegree << endl;
                row = i;
                // cout << "row " << row << endl; 
            }
        }

        // Add int row to vertex cover, clear edges that 
        // contain int row and clear the row of int row.
        approx1VC.push_back(row);
        clearEdgesApprox1(row);
        adjListApprox1[row].clear();

        // flag == 1 indicates to check if adjListApprox1 is empty.
        int flag = 1;
        approx1Iter = isClear(flag);
        // cout << "------" << endl;
    }
    // cout << endl;

    // Printout of results for approx1 method.
    std::ostringstream s;
    vector<string> approx1Vector;
    approx1Vector.push_back("APPROX-VC-1:");
    // cout << "APPROX-VC-1: " << endl;
    for (int i = 0; i < approx1VC.size(); i++)
    {
        // cout << approx1VC[i] << " ";
        approx1Vector.push_back(" ");
        s << approx1VC[i];
        approx1Vector.push_back(s.str());
        s.str(std::string());
    }
    // cout << endl;
    printFunction(approx1Vector);
}

/* ==================================================
 * Vertex cover obtained using approx2 method.
 * ================================================== */
void Graph::approx2()
{
    // Bool values for while loop and a vector for 
    // collecting the vertices in the vertex cover.
    bool approx2Iter = false;
    vector<int> approx2VC;

    int u; int v;
    while (!approx2Iter)
    {
        // Get edge in the adjacency matrix (u, v)
        // and add the vertices u and v to the 
        // vertex cover. Then remove all edge that
        // contain u or v.
        list<int>::iterator it;
        for (int i = 0; i < numberOfNodes; i++)
        {
            if (adjListApprox2[i].size() != 0)
            {
                it = adjListApprox2[i].begin();
                u = i; v = *it;
                // cout << u << " and " << v << endl;

                approx2VC.push_back(u);
                approx2VC.push_back(v);
                break;
            }
        }

        adjListApprox2[u].clear();
        adjListApprox2[v].clear();
        clearEdgesApprox2(u, v);

        int flag = 2;
        approx2Iter = isClear(flag);
    }

    // Printout of results for approx2 method.
    std::ostringstream s;
    vector<string> approx2Vector;
    approx2Vector.push_back("APPROX-VC-2:");
    // cout << "APPROX-VC-2: " << endl;
    for (int i = 0; i < approx2VC.size(); i++)
    {
        // cout << approx2VC[i] << " ";
        approx2Vector.push_back(" ");
        s << approx2VC[i];
        approx2Vector.push_back(s.str());
        s.str(std::string());
    }
    // cout << endl;
    printFunction(approx2Vector);
    // cout << "APPROX-VC-2: " << endl;
    // for (int i = 0; i < approx2VC.size(); i++)
    // {
    //     cout << approx2VC[i] << " ";
    // }
    // cout << endl;
}

/* ==================================================
 * Mutex protexted print function.
 * ================================================== */
void Graph::printFunction(vector<string> input)
{
    mtx.lock();
    vector<string>::iterator it;
    for (it = input.begin(); it != input.end(); it++)
    {
        cout << *it;
    }
    cout << endl;
    // cout << input << endl;
    mtx.unlock();
}

/* ==================================================
 * Reset graph for new input.
 * ================================================== */
void Graph::resetGraph()
{
    delete[] parent;

    delete[] adjList;

    delete[] adjListApprox1;

    delete[] adjListApprox2;
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