#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <iterator>
#include <vector>
#include <list>

/***********************************************************
*File	:main.ccp
*Author	:Odalys Martinez-Villa
*Class	:COP 3530
*Purpose: homework3 - Program that finds the driving
* direction of a specified initial and end location.
***********************************************************/

using namespace std;

//method that quick sorts
void QuickSort(string data_str[], int left, int right) {
    int i, j;
    string x;
    string temp;

    i = left;
    j = right;
    x = data_str[(left + right) / 2];

    do {
        while ((data_str[i] < x) && (i < right)) {
            i++;
        }
        while ((data_str[j] > x) && (j > left)) {
            j--;
        }
        if (i <= j) {
            temp = data_str[i];
            data_str[i] = data_str[j];
            data_str[j] = temp;
            i++;
            j--;
        }
    } while (i <= j);

    if (left < j) {
        QuickSort(data_str, left, j);
    }
    if (i < right) {
        QuickSort(data_str, i, right);
    }
}

template <class Container>
void split(const string& str, Container& cont) {
    istringstream iss(str);
    copy(istream_iterator<string>(iss), istream_iterator<string>(), back_inserter(cont));
}

int remove_duplicates(string data[], int len) {
    //Return, if array is empty
    // or contains a single element...
    if (len == 0 || len == 1) {
        return len;
    }

    int j = 0;
    for (int i = 0; i < len - 1; i++) {
        if (data[i] != data[i + 1]) {
            data[j++] = data[i];
        }
    }
    data[j++] = data[len - 1];

    return j;
}

// Data structure to store Adjacency list nodes
struct Node {
    int val, speed;
    string street, direction, distance;
    Node* next;
};

// Data structure to store graph edges
struct Edge {
    int src, dest, speed;
    string street, direction, distance;
};

class Graph {
    // Function to allocate new node of Adjacency List
    Node* getAdjListNode(int value, int speed, string street, string direct, string dist, Node* head)
    {
        Node* newNode = new Node;
        newNode->val = value;
        newNode->speed = speed;
        newNode->street = street;
        newNode->direction = direct;
        newNode->distance = dist;

        // point new node to current head
        newNode->next = head;

        return newNode;
    }

    int N;	// number of nodes in the graph

public:

    // An array of pointers to Node to represent
    // adjacency list
    Node** head;

    // Constructor
    Graph(Edge edges[], int n, int N)
    {
        // allocate memory
        head = new Node * [N]();
        this->N = N;

        // initialize head pointer for all vertices
        for (int i = 0; i < N; ++i)
            head[i] = nullptr;

        // add edges to the directed graph
        for (unsigned i = 0; i < n; i++)
        {
            int src = edges[i].src;
            int dest = edges[i].dest;
            int speed = edges[i].speed;
            string street = edges[i].street;
            string direc = edges[i].direction;
            string dist = edges[i].distance;

            // insert in the beginning
            Node* newNode = getAdjListNode(dest, speed, street, direc, dist, head[src]);

            // point head pointer to new node
            head[src] = newNode;
        }
    }

    // Destructor
    ~Graph() {
        for (int i = 0; i < N; i++)
            delete[] head[i];

        delete[] head;
    }
};

// print all neighboring vertices of given vertex
void printList(Node* ptr, int i, string data[])
{
    while (ptr != nullptr)
    {
        cout << "[" << data[i] << ",    " << ptr->street << ",  " << data[ptr->val]
             << ",   " << ptr->direction << ",   " << ptr->distance << ",    " << ptr->speed << "] \n";

        ptr = ptr->next;
    }

    cout << endl;
}


// This class represents a directed graph using adjacency
// list representation
class GraphNew
{
    int V;    // No. of vertices
    list<int>* adj;    // adjacency lists
public:

    GraphNew(int V) {
        this->V = V;
        adj = new list<int>[2 * V];
    }

    int output[10];
    int i = 0;
    void addEdge(int v, int w, int weight)
    {
        // split all edges of weight 2 into two
        // edges of weight 1 each.  The intermediate
        // vertex number is maximum vertex number + 1,
        // that is V.
        if (weight == 2)
        {
            adj[v].push_back(v + V);
            adj[v + V].push_back(w);
        }
        else // Weight is 1
            adj[v].push_back(w); // Add w to v�s list.
    }

    // To print the shortest path stored in parent[]
    int printShortestPath(int parent[], int s, int d)
    {
        int level = 0;
        // If we reached root of shortest path tree
        if (parent[s] == -1)
        {
            return level;
        }
        printShortestPath(parent, parent[s], d);
        if (s < V) {
            output[i] = s;
            i++;
        }
        return level;
    }
    // This function mainly does BFS and prints the
    // shortest path from src to dest. It is assumed
    // that weight of every edge is 1
    int findShortestPath(int src, int dest)
    {
        // Mark all the vertices as not visited
        bool* visited = new bool[2 * V];
        int* parent = new int[2 * V];

        // Initialize parent[] and visited[]
        for (int i = 0; i < 2 * V; i++)
        {
            visited[i] = false;
            parent[i] = -1;
        }
        // Create a queue for BFS
        list<int> queue;

        // Mark the current node as visited and enqueue it
        visited[src] = true;
        queue.push_back(src);

        // 'i' will be used to get all adjacent vertices of a vertex
        list<int>::iterator i;

        while (!queue.empty())
        {
            // Dequeue a vertex from queue and print it
            int s = queue.front();

            if (s == dest)
                return printShortestPath(parent, s, dest);

            queue.pop_front();

            // Get all adjacent vertices of the dequeued vertex s
            // If a adjacent has not been visited, then mark it
            // visited and enqueue it
            for (i = adj[s].begin(); i != adj[s].end(); ++i)
            {
                if (!visited[*i])
                {
                    visited[*i] = true;
                    queue.push_back(*i);
                    parent[*i] = s;
                }
            }
        }
    }
};


int main() {
    // array of graph edges.
    Edge graphEdges[50];

    string interName[50];
    int i = 0;
    string edges[50][6];
    int e = 0;

    ifstream file("mapcampus.txt");
    string str;
    while (getline(file, str)) {
        //cout << str << "\n" << i+1 << "\n";
        // Calling split function to split each line in the file...
        vector<string> words;
        split(str, words);
        //Adding intersection names into interName Array...
        interName[i] = words[0];
        i++;

        string source = words[0];
        string destination = words[2];
        string street = words[1];
        string direction = words[3];
        string distance = words[4];
        string speed = words[5];
        //Adding edge data to edges array...
        edges[e][0] = source;
        edges[e][1] = destination;
        edges[e][2] = street;
        edges[e][3] = direction;
        edges[e][4] = distance;
        edges[e][5] = speed;
        e++;
    }

    //Calling QuickSort function...
    QuickSort(interName, 0, i - 1);


    //Calling removeDuplicates function to remove duplicates...
    int len = remove_duplicates(interName, i);

    //Number of vertices(Nodes) in the graph
    int N = len;
    //Number of edges in the graph
    int n = e - 1;

    //New graph class for finding shortest path...
    int V = len;
    GraphNew g(V);

    int e0, e1, e5;
    string e2, e3, e4;
    //loop for converting each intersection name to an Vertex/Node(int)...
    for (int i = 0; i < e - 1; i++) {
        for (int j = 0; j < len; j++) {
            if (edges[i][0] == interName[j]) {
                e0 = j;
            }
        }
        for (int j = 0; j < len; j++) {
            if (edges[i][1] == interName[j]) {
                e1 = j;
            }
        }
        e2 = edges[i][2];
        e3 = edges[i][3];
        e4 = edges[i][4];
        e5 = 0;
        stringstream spd(edges[i][5]);
        spd >> e5;
        graphEdges[i] = { e0, e1, e5, e2, e3, e4 };   //graph Edge structure.

        g.addEdge(e0, e1, e5);
    }

    Graph graph(graphEdges, n, N);
    // print adjacency list representation of graph
//	for (int i = 0; i < N; i++)
//	{
//		// print all neighboring vertices of vertex i
//		printList(graph.head[i], i, interName);
//	}

    //print Intersection Names list...
    //(after QuickSort and RemoveDuplicate Operations)
////    cout << "\n";
////    for(int j=0; j<len; j++){
////            cout << j+1 << ") " << interName[j] << "\n" ;
////
////    }

    //Start and End intersection variables...
    string Start, End;
    cout << "Please enter Start and End Intersections...\n";
    cout << "\tEnter Start Intersection :- ";   //Case Sensetive
    cin >> Start;
    cout << "\tEnter End Intersection :- "; //Case Sensetive
    cin >> End;
    cout << "\n";

    int s, d;   //s=start, d=end...
    //Get start and end values...
    for (int j = 0; j < len; j++) {
        if (Start == interName[j]) {
            //            cout << Start << " : " << j;
            s = j;
        }
    }

    for (int j = 0; j < len; j++) {
        if (End == interName[j]) {
            //            cout << End << " : " << j;
            d = j;
        }
    }

    g.findShortestPath(s, d);

    cout << "From " << interName[s] << "\n";
    for (int i = 1; i < g.i; i++) {
        cout << "Take " << graph.head[g.output[i]]->street << ", " << graph.head[g.output[i]]->direction;
        cout << " to " << interName[g.output[i]] << "\t" << graph.head[g.output[i]]->distance << endl;
    }

    return 0;
}
