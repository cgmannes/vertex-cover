//============================================
// Include guard.
# ifndef GRAPH_H
# define GRAPH_H


/* ============================================
 * Graph class interface.
 * ============================================ */
class Graph
{
    public:
        //============================================
        // Member function prototypes for class Graph.

        // Constructor.
        Graph(int numberOfNodes = 0, int numberOfEdges = 0, int numberOfProps = 0);

        // Graph building methods.
        void addEdge(int & u, int & v);
        void displayAdjList();

        // Graph search methods.
        void BFS(int s, int target);
        void displayPath(int & s, int & t);
        int pathReverse(int currentNode);

        // Execution of Minisat solver for minimal vertex cover problem.
        bool cnfSatSolver(int & k);

        // Execution of Approx1 for solving the vertex cover problem.
        void approx1();

        // Execution of Approx2 for solving the vertex cover problem.
        void approx2();

        void clearEdgesApprox1(int row);
        void clearEdgesApprox2(int u, int v);
        bool isClear(int flag);

        // Print function for multithreading application.
        void printFunction(std::vector<std::string> input);

        // Accessors and mutators.
        void resetGraph();
        int getNumberOfNodes();
        int getNumberOfEdges();
        int getNumberOfProps();
        std::string getVertexCoverString();

    private:
        //============================================
        // Member variables.
        int numberOfNodes;
        int numberOfEdges;
        int numberOfProps;
        std::string vertexCoverString;
        int * parent;
        std::list<int> * adjList;
        std::list<int> * adjListApprox1;
        std::list<int> * adjListApprox2;
};


# endif