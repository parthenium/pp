#include <iostream>
#include <vector>
#include <algorithm>
#include <omp.h>
#include <chrono>

using namespace std;

void distributed_HistSort(std::vector<int>& arr, int num_bins) {
    int n = arr.size();
    
    int min_val = *std::min_element(arr.begin(), arr.end());
    int max_val = *std::max_element(arr.begin(), arr.end());
    int range = max_val - min_val + 1;
    
    int bin_size = range / num_bins;
    
    vector<vector<int>> bins(num_bins);
    
    #pragma omp parallel for
    for (int i = 0; i < n; ++i) {
        int bin_index = (arr[i] - min_val) / bin_size;
        if (bin_index == num_bins) {
            bin_index--;
        }
        #pragma omp critical
        bins[bin_index].push_back(arr[i]);
    }
    
    #pragma omp parallel for
    for (int i = 0; i < num_bins; ++i) {
        sort(bins[i].begin(), bins[i].end());
    }
    
    int index = 0;
    for (int i = 0; i < num_bins; ++i) {
        for (int j = 0; j < bins[i].size(); ++j) {
            arr[index++] = bins[i][j];
        }
    }
}

int main() {
    vector<int> arr = {15, 3, 9, 27, 6, 18, 22, 14, 1, 30, 10, 5, 25, 2, 13, 21, 11, 28, 7, 20, 23, 17, 12, 19, 8, 24, 29, 4, 16, 26};
    
    int num_bins = 3;
    
    cout << "Original array: ";
    for (int num : arr) {
        cout << num << " ";
    }
    cout << endl;

    auto start_parallel = chrono::high_resolution_clock::now();
    distributed_HistSort(arr, num_bins);
    
    cout << "Sorted array: ";
    for (int num : arr) {
        cout << num << " ";
    }
    cout << endl;

    auto end_parallel = chrono::high_resolution_clock::now();

    auto duration_parallel = chrono::duration_cast<chrono::milliseconds>(end_parallel - start_parallel);
    cout << endl << "Time taken for parallel Dijkstra: " << duration_parallel.count() << " milliseconds" << endl;
    
    return 0;
}
