#include <iostream>
#include <omp.h>
#include <limits.h>
#include <chrono>

#define V 6
#define INF INT_MAX

using namespace std;
using namespace chrono;

int min_distance(int dist[], int sptSet[], int n) {
    int min = INF;
    int min_index = -1;

    #pragma omp parallel for
    for (int v = 0; v < n; v++) {
        if (!sptSet[v] && dist[v] <= min) {
            #pragma omp critical
            {
                if (dist[v] < min) {
                    min = dist[v];
                    min_index = v;
                }
            }
        }
    }

    return min_index;
}

void dijkstra(int graph[V][V], int start) {
    int dist[V];
    int sptSet[V];

    #pragma omp parallel for
    for (int i = 0; i < V; i++) {
        dist[i] = INF;
        sptSet[i] = 0;
    }
    dist[start] = 0;

    for (int count = 0; count < V - 1; count++) {
        int u = min_distance(dist, sptSet, V);

        sptSet[u] = 1;

        #pragma omp parallel for
        for (int v = 0; v < V; v++) {
            if (!sptSet[v] && graph[u][v] && dist[u] != INF && dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v];
            }
        }
    }

    cout << "Vertex \t Distance from Source\n";
    for (int i = 0; i < V; i++) {
        cout << i << " \t " << dist[i] << endl;
    }
}
int main() {
    int graph[V][V] = {
        {0, 9, 6, 0, 0, 0},
        {9, 0, 0, 5, 0, 0},
        {6, 0, 0, 3, 0, 0},
        {0, 5, 3, 0, 4, 2},
        {0, 0, 0, 4, 0, 7},
        {0, 0, 0, 2, 7, 0}
    };

    auto start_parallel = chrono::high_resolution_clock::now();
    dijkstra(graph, 0);
    auto end_parallel = chrono::high_resolution_clock::now();

    auto duration_parallel = chrono::duration_cast<chrono::milliseconds>(end_parallel - start_parallel);
    cout << endl << "Time taken for parallel Dijkstra: " << duration_parallel.count() << " milliseconds" << endl;
    return 0;
}
