#include <complex>
#include <cstdio>
#include <omp.h>
#include <chrono>
#include <iostream>
#include <fstream>
int main()
{
    const int maxMemSize = 50*1024*1024;
    const int thread_number = 4;
    
    omp_set_num_threads(thread_number);
    auto start = std::chrono::high_resolution_clock::now();
    auto finish = std::chrono::high_resolution_clock::now();

    int repCount = 0;
    std::ofstream res("res.txt");
    for (size_t chunk_size = 8; chunk_size < 2*1024*1024; chunk_size *= 2) 
    {
        #pragma omp parallel
        {
            #pragma omp single
            {
                repCount = 50000;
                if (repCount * chunk_size > maxMemSize)
                    repCount = maxMemSize / chunk_size;
              
                start = std::chrono::high_resolution_clock::now();
            }

            void** mem = new void*[repCount];
     
            for (int i = 0; i < repCount; ++i)
                mem[i] = malloc(chunk_size);
            
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
                
                res<<duration / repCount<<"\n";
                std::cout<<chunk_size<<","<<duration / repCount<<"\n";
            }
        } 
    }
    res.close();
    return 0;  
}