#include <iostream>
#include <vector>
#include <queue>
#include <omp.h>
using namespace std;
class Graph
{
private:
    int V;                   // Number of vertices
    vector<vector<int>> adj; // Adjacency list

public:
    Graph(int V) : V(V), adj(V) {}

    void addEdge(int u, int v)
    {
        adj[u].push_back(v);
        adj[v].push_back(u); // For undirected graph
    }

    // Parallel Breadth First Search
    void parallelBFS(int start)
    {
        vector<bool> visited(V, false);
        queue<int> q;

        visited[start] = true;
        q.push(start);

        while (!q.empty())
        {
#pragma omp parallel for
            for (int i = 0; i < q.size(); ++i)
            {
                int u = q.front();
                q.pop();

                cout << u << " ";

                // Enqueue adjacent vertices
                for (int v : adj[u])
                {
                    if (!visited[v])
                    {
                        visited[v] = true;
                        q.push(v);
                    }
                }
            }
        }
    }

    // Parallel Depth First Search
    void parallelDFSUtil(int u, vector<bool> &visited)
    {
        visited[u] = true;
        cout << u << " ";

#pragma omp parallel for
        for (int v : adj[u])
        {
            if (!visited[v])
            {
                parallelDFSUtil(v, visited);
            }
        }
    }

    void parallelDFS(int start)
    {
        vector<bool> visited(V, false);
        parallelDFSUtil(start, visited);
    }
};

int main()
{
    int n, a, b, i;
    cout << " ENter no of edges:";
    cin >> n;
    Graph g(n + 1); // Creating a graph with 6 vertices

    for (i = 0; i < n; i++)
    {
        cout << "Enter edge 1 and 2 :";
        cin >> a;
        cin >> b;
        g.addEdge(a, b);
    }
    // Adding edges to the graph
    // g.addEdge(0, 1);
    // g.addEdge(0, 2);
    // g.addEdge(1, 2);
    // g.addEdge(2, 0);
    // g.addEdge(2, 3);
    // g.addEdge(3, 3);

    cout << "BFS traversal starting from node 0: ";
    g.parallelBFS(0);
    cout << endl;

    cout << "DFS traversal starting from node 0: ";
    g.parallelDFS(2);
    cout << endl;

    return 0;
}
