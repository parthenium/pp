#include <iostream>
#include <chrono>
#include <omp.h>

using namespace std;

int main(){
    int N;
    cout << "Enter a number: ";
    cin >> N;

    auto start_serial = chrono::high_resolution_clock::now();
    int sum_serial = 0;
    for(int i = 1; i < N + 1; i++){
        sum_serial += i;
    }
    auto end_serial = chrono::high_resolution_clock::now();
    auto duration_serial = chrono::duration_cast<chrono::milliseconds>(end_serial - start_serial);

    auto start_parallel = chrono::high_resolution_clock::now();
    int sum_parallel = 0;
    for(int i = 1; i < N + 1; i++){
        #pragma omp parallel for
        for(int j = 1; j < N + 1; j++){
            sum_parallel += i;
        }
    }
    auto end_parallel = chrono::high_resolution_clock::now();
    auto duration_parallel = chrono::duration_cast<chrono::milliseconds>(end_parallel - start_parallel);

    cout << "Time taken for serial matrix multiplication: " << duration_serial.count() << " milliseconds" << endl;
    cout << "Time taken for parallel matrix multiplication: " << duration_parallel.count() << " milliseconds" << endl;
}
