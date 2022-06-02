#include <iostream>
#include <ctime>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
using std::chrono::duration_cast;
using std::chrono::microseconds;
using std::chrono::steady_clock;

void menu()
{
    std::cout << "MENU\n";
    std::cout << "1 ... READ GRAPH FROM FILE\n";
    std::cout << "2 ... GENERATE RANDOM GRAPH WITH n NODES AND RANDOM COST\n"
                 "      (with user input of minimum cost and maximum cost)\n";
    std::cout << "3 ... BELLMAN-FORD ALGORITHM\n";
    std::cout << "4 ... PRINT A LIST OF NODES\n";
    std::cout << "5 ... PRINT SHORTEST PATH\n";
    std::cout << "0 ... EXIT\n";
}

struct Node
{
    int predecessor;
    int cost;
    int name;
};

void readFromFile(const std::string& filePath,
                  std::vector<Node*>& nodeVec,
                  std::vector<std::vector<int>>& adjMatrix)
{
    std::ifstream myFile;
    myFile.open(filePath);
    int numOfNodes = 0;
    myFile >> numOfNodes;
    // imena vozlišč
    int n = 1;
    for(auto& i: nodeVec)
    {
        delete i;
    }
    nodeVec.clear();
    for(int i = 0; i < numOfNodes; i++)
    {
        std::vector<int> tmpVec;
        for(int j = 0; j < numOfNodes; j++)
        {
            int x = 0;
            myFile >> x;
            tmpVec.push_back(x);
        }
        adjMatrix.push_back(tmpVec);
    }
    for(int i = 0; i < numOfNodes; i++)
    {
        Node* node = new Node;
        node->name = i;
        nodeVec.push_back(node);
    }
    myFile.close();
}

int randFunction(int minCost,
                 int maxCost)
{
    int randNum = rand()%(maxCost - minCost) + minCost;
    return randNum;
}

void generateRandomGraph(int val,                               // število vozlišč
                         int minCost,
                         int maxCost,
                         std::vector<Node*>& nodeVec,
                         std::vector<std::vector<int>>& adjMatrix)
{
    // zbrišem vsa vozlišča, ki obstajajo
    for(auto& i: nodeVec)
    {
        delete i;
    }
    nodeVec.clear();
    for(int i = 0; i < val; i++)
    {
        std::vector<int> vec;
        for(int j = 0; j < val; j++)
        {
            if(i != j)
            {
                int randNum = randFunction(minCost, maxCost);
                vec.push_back(randNum);
            }
            else
            {
                vec.push_back(0);
            }
        }
        adjMatrix.push_back(vec);
    }
    for(int i = 0; i < val; i++)
    {
        Node* node = new Node;
        node->name = i;
        node->predecessor = -1;
        node->cost = 0;
        nodeVec.push_back(node);
    }
}

void bellmanFord(int val,
                 std::vector<Node*>& nodeVec,
                 std::vector<std::vector<int>> adjMatrix)
{
    for(auto& i: nodeVec)
    {
        i->cost = INT32_MAX;
        i->predecessor = -1;
    }
    nodeVec[val]->cost = 0;
    nodeVec[val]->predecessor = -1;
    for(int i = 0; i < adjMatrix.size(); i++)
    {
        for(int j = 0; j < adjMatrix[i].size(); j++)
        {
            if(nodeVec[i]->cost + adjMatrix[i][j] < nodeVec[j]->cost
            && nodeVec[i]->cost < INT32_MAX)
            {
                nodeVec[j]->cost = nodeVec[i]->cost + adjMatrix[i][j];
                nodeVec[j]->predecessor = nodeVec[i]->name;
            }
        }
    }
}

void printNodes(std::vector<Node*> nodeVec)
{
    for(auto& i: nodeVec)
    {
        std::cout << "Node number: " << i->name <<
                  "\nPredecessor: ";
        if(i->predecessor == -1)
        {
            std::cout << "none.";
        }
        else
        {
            std::cout << i->predecessor;
        }
        std::cout << "\nCost: " << i->cost << "\n-------------------\n";
    }
}

void printShortestPath(int startNode,
                       int targetNode,
                       int count,
                       std::vector<Node*> nodeVec)
{
    if(startNode == targetNode)
    {
        std::cout << "Cost: " << count << "\n";
        std::cout << "Path: " << nodeVec[targetNode]->name+1 << " ";
    }
    else
    {
        if(nodeVec[targetNode]->predecessor == -1)
        {
            std::cout << "Path not found.\n";
        }
        else
        {
            count += nodeVec[targetNode]->cost;
            printShortestPath(startNode, nodeVec[targetNode]->predecessor, count, nodeVec);
            std::cout << nodeVec[targetNode]->name+1 << " ";
        }
    }
}

int main()
{
    srand(time(nullptr));
    std::chrono::steady_clock::time_point start, end;
    std::vector<Node*> nodeVec;
    std::vector<std::vector<int>> adjMatrix;
    bool running = true;
    int selection = 0;

    std::string filePath = "";
    int val = 0;
    int min = 0;
    int max = 0;
    int targetNode = 0;
    int count = 0;
    while(running)
    {
        menu();
        std::cin >> selection;
        switch(selection)
        {
            case 1:
                std::cout << "Please input the name/path of your file:\n";
                std::cin >> filePath;
                start = steady_clock::now();
                readFromFile(filePath, nodeVec, adjMatrix);
                end = steady_clock::now();
                std::cout << "\nDuration in microseconds: " << duration_cast<microseconds>(end-start).count() << "us\n\n";
                break;
            case 2:
                std::cout << "Please input the number of nodes you want to generate:\n";
                std::cin >> val;
                if(val > 1500)
                {
                    std::cout << "Number of nodes should be smaller than or equal to 1500.\n";
                    std::cout << "Please input the number of nodes you want to generate:\n";
                    std::cin >> val;
                }
                std::cout << "Please input the minimum value of the cost:\n";
                std::cin >> min;
                std::cout << "Please input the maximum value of the cost:\n";
                std::cin >> max;
                start = steady_clock::now();
                generateRandomGraph(val, min, max, nodeVec, adjMatrix);
                end = steady_clock::now();
                std::cout << "\nDuration in microseconds: " << duration_cast<microseconds>(end-start).count() << "us\n\n";
                break;
            case 3:

                std::cout << "Please input the starting node:\n";
                std::cin >> val;
                if(val >= nodeVec.size())
                {
                    std::cout << "Node does not exist.\n";
                }
                else
                {
                    start = steady_clock::now();
                    bellmanFord(val-1, nodeVec, adjMatrix);
                    end = steady_clock::now();
                    std::cout << "\nDuration in microseconds: " << duration_cast<microseconds>(end-start).count() << "us\n\n";
                }
                break;
            case 4:
                printNodes(nodeVec);
                break;
            case 5:
                std::cout << "Please input the target node:\n";
                std::cin >> targetNode;
                if(targetNode >= nodeVec.size())
                {
                    std::cout << "Node does not exist.\n";
                    std::cout << "Please input the target node:\n";
                    std::cin >> targetNode;
                }
                printShortestPath(val-1, targetNode-1, count, nodeVec);
                std::cout << "\n";
                break;
            case 0:
                running = false;
                break;
            default:
                std::cout << "Wrong selection.\n";
                break;
        }
    }
    for(auto& i: nodeVec)
    {
        delete i;
    }
    nodeVec.clear();
    adjMatrix.clear();
}