#include <iostream>
#include <queue>
#include <vector>
#include <omp.h>
#include <chrono>

using namespace std;

void addEdge(vector<vector<int>>& adj, int u, int v){
    adj[u].push_back(v);
    adj[v].push_back(u);
}

void bfs_parallel(vector<vector<int>>& adj, int s){
    queue<int> q;  
    
    vector<bool> visited(adj.size(), false);
    visited[s] = true;
    q.push(s);

    while(!q.empty()){
        int curr = q.front();
        q.pop();
        cout << curr << " ";
        
        #pragma omp parallel for
        for(int i = 0; i < adj[curr].size(); i++){
            int x = adj[curr][i];
            if(!visited[x]){
                #pragma omp critical
                {
                    if (!visited[x]) {
                        visited[x] = true;
                        q.push(x);
                    }
                }
            }
        }
    }
}

int main(){
    int V = 5;

    vector<vector<int>> adj(V);

    addEdge(adj, 0, 1);
    addEdge(adj, 0, 2);
    addEdge(adj, 2, 3);
    addEdge(adj, 1, 4);
    addEdge(adj, 2, 4);

    cout << "BFS traversal of graph starting from 0 : \n";
    
    auto start_parallel = chrono::high_resolution_clock::now();
    bfs_parallel(adj, 0);
    auto end_parallel = chrono::high_resolution_clock::now();

    auto duration_parallel = chrono::duration_cast<chrono::milliseconds>(end_parallel - start_parallel);
    cout << endl << "Time taken for parallel BFS: " << duration_parallel.count() << " milliseconds" << endl;
    
    return 0;
}
