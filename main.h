/*
Approximate Pi using the Leibniz formula
*/

#include <thread>
#include <iostream>
#include <vector>
#include <chrono>
#include <mutex>
#include <condition_variable>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

#define NUM_THREADS 2 // adjust this to change the number of threads

#define PAD 8 // prevent false sharing

static long long int num_steps = 1000000000;
double step;

// add synchronization primitive(s) here
mutex m;
condition_variable cv;
int value = 1;

// Do not modify this function
inline void single_sum_thread(int id, int num_threads, double sum[NUM_THREADS][PAD])
{
    double x;
    sum[id][0] = 0.0;
    for (long long int i = id; i < num_steps; i = i + num_threads)
    {
        x = (i + 0.5) * step;
        sum[id][0] += 4.0 / (1.0 + x * x);
    }
}

// TODO
inline void pi_sum_thread(double *pi, double sum[][PAD], int i)
{
    unique_lock<mutex> ul(m);
    cv.wait(ul, []{return value > 0;});
    value = 0;
    *pi += sum[i][0] * step;
    value = 1;
    ul.unlock();
    cv.notify_all();
    sleep(1); // DO NOT REMOVE THIS
}
