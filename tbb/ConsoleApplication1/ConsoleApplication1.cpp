#include <complex>
#include <cstdio>
#include <omp.h>
#include <chrono>
#include <iostream>
#include <fstream>
int main()
{
    // std::cout << "Start\n";
    const int memSize = 50 * 1024 * 1024;
    omp_set_num_threads(4);
    const int thread_number = 20;
    double* counts = new double[thread_number];
    omp_set_num_threads(thread_number);
    auto start = std::chrono::high_resolution_clock::now();
    auto finish = std::chrono::high_resolution_clock::now();
    size_t chunk_size = 8;
    int repCount = 0;
    std::ofstream res("res.txt");
    for (size_t chunk_size = 8; chunk_size < 2 * 1024 * 1024; chunk_size *= 2)
    {
#pragma omp parallel
        {
#pragma omp single
            {
                repCount = 50000;
                if (repCount * chunk_size > memSize)
                    repCount = memSize / chunk_size;
                // std::cout << "fff" << repCount << std::endl;
                start = std::chrono::high_resolution_clock::now();
            }
            void** mem = new void*[repCount];
            int tid = omp_get_thread_num();
            // std::cout <<":"<<tid<<":\n";
            for (int i = 0; i < repCount; ++i)
            {
                // executed++;
                mem[i] = malloc(chunk_size);
                // std::cout << tid <<" : "<< executed <<std::endl;
            }
            auto finish = std::chrono::high_resolution_clock::now();

            for (int i = 0; i < repCount; ++i)
            {
                free(mem[i]);
            }
            delete mem;
#pragma omp barrier
#pragma omp single 
            {
                finish = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> elapsed = finish - start;
                double duration = elapsed.count();
                // counts[tid] = elapsed.count();
                // double total = 0;
                // for (int i = 0; i < thread_number; i++) {
                //     total += counts[i];
                //     std::cout <<"thread "<< i+1 << ": " << counts[i] << std::endl;
                // }
                res << duration / repCount << "\n";
                std::cout << chunk_size << "," << duration / repCount << "\n";

                // std::cout << repCount <<"\\ \t"<< chunk_size <<" bytes: ";
                // std::cout << duration << " / "<< duration / repCount << std::endl;
                // std::cout << "Average: " << duration / repCount << std::endl;
                // delete[] counts;
            }
        }
    }
    res.close();
    return 0;
}