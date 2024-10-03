#include <iostream>
#include <pthread.h>
#include <cstdlib>
#include <ctime>
#include <cmath>
using namespace std;

unsigned int seed;
unsigned long long insideCirculePoint=0;
double randomNumberGenerator(){
    return rand_r(&seed) / (double)RAND_MAX;
}
int calculatePI(void* arg){
    auto iteration = *(unsigned long long*)arg;

    for(unsigned long long i=0; i < iteration; i++){
        double x = randomNumberGenerator();
        double y = randomNumberGenerator();
        if(sqrt(x*x + y*y) <=1){
            insideCirculePoint++;
        }
    }
    pthread_exit(nullptr);
}
int main(int argc, char* agrv[]) {
    if(argc != 2){
        cout << "Input format is "<< "'./a.out' " << atoll(agrv[1]) << endl;
    }
    seed = time(nullptr);
    unsigned long long totalIteration = atoll(agrv[1]);

    clock_t startTime = clock();

    pthread_t pthread;
    pthread_create(&pthread, nullptr, calculatePI,&totalIteration);
    return 0;
}
