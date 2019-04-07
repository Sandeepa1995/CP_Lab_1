//
// Created by damitha on 4/1/19.
//

#ifndef CP_LAB_1_OPERATIONS_H
#define CP_LAB_1_OPERATIONS_H

#include <iostream>
#include <cmath>
#include <time.h>
#include <vector>
#include <functional>
#include <limits>
#include <chrono>
#include <pthread.h>

#include "linked_list.h"
#include "utils.h"

// Number of threads tested
int THREAD_NUMBERS[4] = {1, 2, 4, 8};


// Define the locks
pthread_mutex_t mutex;
pthread_rwlock_t rwlock;

// Define a list
Linked_List lst;

// Function definitions
// Operation functions
double do_seq_operations(int member_ops, int insert_ops, int delete_ops, int min = 0, int max = 65536);
double do_mutex_operations(int thread_cnt, int member_ops, int insert_ops, int delete_ops, int min, int max);
void *mutex_operations(void *threadarg);
double do_rwl_operations(int thread_cnt, int member_ops, int insert_ops, int delete_ops, int min, int max);
void *rwl_operations(void *threadarg);
// Calling functions (Return results)
std::vector<double> do_tasks_n_get_times(int member_ops, int insert_ops, int delete_ops,
                                         std::function<double(int, int, int, int, int)> task, int min = 0,
                                         int max = 65536);
std::vector<std::vector<double>> do_parallel_tasks_n_get_times(int member_ops, int insert_ops, int delete_ops,
                                                               std::function<double(int, int, int, int, int, int)> task,
                                                               int min = 0,
                                                               int max = 65536);

// Data passed to a thread
struct thread_data {
    int member_ops;
    int insert_ops;
    int delete_ops;
    int min;
    int max;
};

// Perform operations in sequence
double do_seq_operations(int member_ops, int insert_ops, int delete_ops, int min, int max) {
    // Generate a list with random values
    lst = Linked_List::generate_random_list(1000);
    // Initialize random function passing the current time as the seed
    clock_gettime(CLOCK_MONOTONIC, &ts);
    srand(time(nullptr)*pow(10,9) + (time_t)ts.tv_nsec);

    // Define variables
    int rand_num, chk;
    float rand_operation;
    // Variables to hold the local operation counts
    int local_member = 0, local_insert = 0, local_delete = 0;
    // Total operations
    float total_ops = member_ops + insert_ops + delete_ops;
    // Percentages of the operations done
    float member_percen = member_ops / total_ops, insert_percen = insert_ops / total_ops;

    // Initialize time variables
    std::chrono::time_point<std::chrono::high_resolution_clock> t1, t2;

    // Start calculating time
    t1 = std::chrono::high_resolution_clock::now();

    // Loop while the there are operations to perform
    while (local_member < member_ops || local_insert < insert_ops || local_delete < delete_ops) {
        // Get a random operation (Value between 0 - 1)
        rand_operation = rand() / (float) RAND_MAX;
        // If random operation variable is below the percentages and operations left to do
        if ((rand_operation < member_percen) && (local_member < member_ops)){
            lst.member(rand() % (max - min) + min);
            local_member++;
        } else if((rand_operation < (member_percen + insert_percen)) && (local_insert < insert_ops)){
            rand_num = rand() % (max - min) + min;
            chk = lst.insert_node(rand_num);
            // If insertion is successful, then delete the same number
            if (chk == 1){
                // Perform member operations in between
                while(local_member < member_ops){
                    rand_operation = rand() / (float) RAND_MAX;
                    if (rand_operation < member_percen){
                        lst.member(rand() % (max - min) + min);
                        local_member++;
                    } else {
                        break;
                    }
                }
                if (local_delete < delete_ops){
                    lst.delete_node(rand_num);
                    local_delete++;
                }
            }
            // Increment operation regardless of success
            local_insert++;
        } else if (local_delete < delete_ops){
            rand_num = rand() % (max - min) + min;
            chk = lst.delete_node(rand_num);
            // If deletion is successful then insert the same number
            if (chk == 1){
                // Perform member operations in between
                while(local_member < member_ops) {
                    rand_operation = rand() / (float) RAND_MAX;
                    if (rand_operation < member_percen) {
                        lst.member(rand() % (max - min) + min);
                        local_member++;
                    } else {
                        break;
                    }
                }
                if (local_insert < insert_ops){
                    lst.insert_node(rand_num);
                    local_insert++;
                }
            }
            // Increment operation regardless of success
            local_delete++;
        } else if (local_member < member_ops){
            // If member operations are remnant
            lst.member(rand() % (max - min) + min);
            local_member++;
        } else if (local_insert < insert_ops){
            // If insertion operations are remnant
            lst.insert_node(rand() % (max - min) + min);
            local_insert++;
        }
    }
    // End time
    t2 = std::chrono::high_resolution_clock::now();

    // Clear list
    lst.free_list();

    // Return time
    return std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
}

// Do mutex based operations (As a whole)
double do_mutex_operations(int thread_cnt, int member_ops, int insert_ops, int delete_ops, int min, int max) {
    // Generate a list with random values
    lst = Linked_List::generate_random_list(1000);
    // Initialize random function passing the current time as the seed
    clock_gettime(CLOCK_MONOTONIC, &ts);
    srand(time(nullptr)*pow(10,9) + (time_t)ts.tv_nsec);

    // Initialize time variables
    std::chrono::time_point<std::chrono::high_resolution_clock> t1, t2;

    // Initialize pthreads
    pthread_t threads[thread_cnt];
    struct thread_data td[thread_cnt];
    int i;

    // Assign data to be passed to a thread
    for (i = 0; i < thread_cnt; i++) {
        int min_member = (int) floor((double) member_ops / thread_cnt);
        int min_insert = (int) floor((double) insert_ops / thread_cnt);
        int min_delete = (int) floor((double) delete_ops / thread_cnt);
        if (i == thread_cnt - 1) {
            td[i].member_ops = member_ops - min_member * (thread_cnt - 1);
            td[i].insert_ops = insert_ops - min_insert * (thread_cnt - 1);
            td[i].delete_ops = delete_ops - min_delete * (thread_cnt - 1);
        } else {
            td[i].member_ops = min_member;
            td[i].insert_ops = min_insert;
            td[i].delete_ops = min_delete;
        }
        td[i].min = min;
        td[i].max = max;
    }

    // Initialize mutex lock
    pthread_mutex_init(&mutex, nullptr);
    // Start time
    t1 = std::chrono::high_resolution_clock::now();

    // Create and start threads
    for (i = 0; i < thread_cnt; i++)
        pthread_create(&threads[i], nullptr, mutex_operations, (void *) &td[i]);

    // Join threads
    for (i = 0; i < thread_cnt; i++)
        pthread_join(threads[i], nullptr);

    // Stop time
    t2 = std::chrono::high_resolution_clock::now();

    // Free resources
    pthread_mutex_destroy(&mutex);
    lst.free_list();

    // Return time
    return std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
}

// Mutex based operation limited to a thead
void *mutex_operations(void *threadarg) {
    // Thread data
    struct thread_data *data;
    data = (struct thread_data *) threadarg;
    // Define variables
    int rand_num, chk;
    float rand_operation;
    // Variables to hold the local operation counts
    int local_member = 0, local_insert = 0, local_delete = 0;
    // Total operations
    float total_ops = data->member_ops + data->insert_ops + data->delete_ops;
    // Percentages of the operations done
    float member_percen = data->member_ops / total_ops, insert_percen = data->insert_ops / total_ops;

    // Loop while the there are operations to perform
    while (local_member < data->member_ops || local_insert < data->insert_ops || local_delete < data->delete_ops) {
        // Get a random operation (Value between 0 - 1)
        rand_operation = rand() / (float) RAND_MAX;
        // If random operation variable is below the percentages and operations left to do
        if ((rand_operation < member_percen) && (local_member < data->member_ops)){
            pthread_mutex_lock(&mutex);
            lst.member(rand() % (data->max - data->min) + data->min);
            pthread_mutex_unlock(&mutex);
            local_member++;
        } else if((rand_operation < (member_percen + insert_percen)) && (local_insert < data->insert_ops)){
            rand_num = rand() % (data->max - data->min) + data->min;
            pthread_mutex_lock(&mutex);
            chk = lst.insert_node(rand_num);
            pthread_mutex_unlock(&mutex);
            // If insertion is successful, then delete the same number
            if (chk == 1){
                // Perform member operations in between
                while(local_member < data->member_ops){
                    rand_operation = rand() / (float) RAND_MAX;
                    if (rand_operation < member_percen){
                        pthread_mutex_lock(&mutex);
                        lst.member(rand() % (data->max - data->min) + data->min);
                        pthread_mutex_unlock(&mutex);
                        local_member++;
                    } else {
                        break;
                    }
                }
                if (local_delete < data->delete_ops){
                    pthread_mutex_lock(&mutex);
                    lst.delete_node(rand_num);
                    pthread_mutex_unlock(&mutex);
                    local_delete++;
                }
            }
            // Increment operation regardless of success
            local_insert++;
        } else if (local_delete < data->delete_ops){
            rand_num = rand() % (data->max - data->min) + data->min;
            pthread_mutex_lock(&mutex);
            chk = lst.delete_node(rand_num);
            pthread_mutex_unlock(&mutex);
            // If deletion is successful then insert the same number
            if (chk == 1){
                while(local_member < data->member_ops) {
                    rand_operation = rand() / (float) RAND_MAX;
                    if (rand_operation < member_percen) {
                        pthread_mutex_lock(&mutex);
                        lst.member(rand() % (data->max - data->min) + data->min);
                        pthread_mutex_unlock(&mutex);
                        local_member++;
                    } else {
                        break;
                    }
                }
                if (local_insert < data->insert_ops){
                    pthread_mutex_lock(&mutex);
                    lst.insert_node(rand_num);
                    pthread_mutex_unlock(&mutex);
                    local_insert++;
                }
            }
            // Increment operation regardless of success
            local_delete++;
        } else if (local_member < data->member_ops){
            // If member operations are remnant
            pthread_mutex_lock(&mutex);
            lst.member(rand() % (data->max - data->min) + data->min);
            pthread_mutex_unlock(&mutex);
            local_member++;
        } else if (local_insert < data->insert_ops){
            // If insertion operations are remnant
            pthread_mutex_lock(&mutex);
            lst.insert_node(rand() % (data->max - data->min) + data->min);
            pthread_mutex_unlock(&mutex);
            local_insert++;
        }
    }

    return nullptr;
}

// Do read write lock based operations (As a whole)
double do_rwl_operations(int thread_cnt, int member_ops, int insert_ops, int delete_ops, int min, int max) {
    // Generate a list with random values
    lst = Linked_List::generate_random_list(1000);
    // Initialize random function passing the current time as the seed
    clock_gettime(CLOCK_MONOTONIC, &ts);
    srand(time(nullptr)*pow(10,9) + (time_t)ts.tv_nsec);

    // Initialize time variables
    std::chrono::time_point<std::chrono::high_resolution_clock> t1, t2;

    // Initialize pthreads
    pthread_t threads[thread_cnt];
    struct thread_data td[thread_cnt];
    int i;

    // Assign data to be passed to a thread
    for (i = 0; i < thread_cnt; i++) {
        int min_member = (int) floor((double) member_ops / thread_cnt);
        int min_insert = (int) floor((double) insert_ops / thread_cnt);
        int min_delete = (int) floor((double) delete_ops / thread_cnt);
        if (i == thread_cnt - 1) {
            td[i].member_ops = member_ops - min_member * (thread_cnt - 1);
            td[i].insert_ops = insert_ops - min_insert * (thread_cnt - 1);
            td[i].delete_ops = delete_ops - min_delete * (thread_cnt - 1);
        } else {
            td[i].member_ops = min_member;
            td[i].insert_ops = min_insert;
            td[i].delete_ops = min_delete;
        }
        td[i].min = min;
        td[i].max = max;
    }

    // Initialize rw_lock
    pthread_rwlock_init(&rwlock, NULL);
    // Start time
    t1 = std::chrono::high_resolution_clock::now();

    // Create and start threads
    for (i = 0; i < thread_cnt; i++)
        pthread_create(&threads[i], nullptr, rwl_operations, (void *) &td[i]);

    // Stop time
    for (i = 0; i < thread_cnt; i++)
        pthread_join(threads[i], nullptr);

    // Stop time
    t2 = std::chrono::high_resolution_clock::now();

    // Free resources
    pthread_rwlock_destroy(&rwlock);
    lst.free_list();

    // Return time
    return std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
}

// RWLock based operation limited to a thead
void *rwl_operations(void *threadarg) {
    // Thread data
    struct thread_data *data;
    data = (struct thread_data *) threadarg;
    // Define variables
    int rand_num, chk;
    float rand_operation;
    // Variables to hold the local operation counts
    int local_member = 0, local_insert = 0, local_delete = 0;
    // Total operations
    float total_ops = data->member_ops + data->insert_ops + data->delete_ops;
    // Percentages of the operations done
    float member_percen = data->member_ops / total_ops, insert_percen = data->insert_ops / total_ops;

    // Loop while the there are operations to perform
    while (local_member < data->member_ops || local_insert < data->insert_ops || local_delete < data->delete_ops) {
        // Get a random operation (Value between 0 - 1)
        rand_operation = rand() / (float) RAND_MAX;
        // If random operation variable is below the percentages and operations left to do
        if ((rand_operation < member_percen) && (local_member < data->member_ops)){
            pthread_rwlock_rdlock(&rwlock);
            lst.member(rand() % (data->max - data->min) + data->min);
            pthread_rwlock_unlock(&rwlock);
            local_member++;
        } else if((rand_operation < (member_percen + insert_percen)) && (local_insert < data->insert_ops)){
            rand_num = rand() % (data->max - data->min) + data->min;
            pthread_rwlock_wrlock(&rwlock);
            chk = lst.insert_node(rand_num);
            pthread_rwlock_unlock(&rwlock);
            // If insertion is successful, then delete the same number
            if (chk == 1){
                // Perform member operations in between
                while(local_member < data->member_ops){
                    rand_operation = rand() / (float) RAND_MAX;
                    if (rand_operation < member_percen){
                        pthread_rwlock_rdlock(&rwlock);
                        lst.member(rand() % (data->max - data->min) + data->min);
                        pthread_rwlock_unlock(&rwlock);
                        local_member++;
                    } else {
                        break;
                    }
                }
                if (local_delete < data->delete_ops){
                    pthread_rwlock_wrlock(&rwlock);
                    lst.delete_node(rand_num);
                    pthread_rwlock_unlock(&rwlock);
                    local_delete++;
                }
            }
            // Increment operation regardless of success
            local_insert++;
        } else if (local_delete < data->delete_ops){
            rand_num = rand() % (data->max - data->min) + data->min;
            pthread_rwlock_wrlock(&rwlock);
            lst.delete_node(rand() % (data->max - data->min) + data->min);
            pthread_rwlock_unlock(&rwlock);
            // If deletion is successful then insert the same number
            if (chk == 1){
                // Perform member operations in between
                while(local_member < data->member_ops) {
                    rand_operation = rand() / (float) RAND_MAX;
                    if (rand_operation < member_percen) {
                        pthread_rwlock_rdlock(&rwlock);
                        lst.member(rand() % (data->max - data->min) + data->min);
                        pthread_rwlock_unlock(&rwlock);
                        local_member++;
                    } else {
                        break;
                    }
                }
                if (local_insert < data->insert_ops){
                    pthread_rwlock_wrlock(&rwlock);
                    chk = lst.insert_node(rand_num);
                    pthread_rwlock_unlock(&rwlock);
                    local_insert++;
                }
            }
            // Increment operation regardless of success
            local_delete++;
        } else if (local_member < data->member_ops){
            // If member operations are remnant
            pthread_rwlock_rdlock(&rwlock);
            lst.member(rand() % (data->max - data->min) + data->min);
            pthread_rwlock_unlock(&rwlock);
            local_member++;
        } else if (local_insert < data->insert_ops){
            // If insertion operations are remnant
            pthread_rwlock_wrlock(&rwlock);
            lst.insert_node(rand() % (data->max - data->min) + data->min);
            pthread_rwlock_unlock(&rwlock);
            local_insert++;
        }
    }
    return nullptr;
}

// Perform the set of operations (of sequential) till required confidence is achieved
std::vector<double> do_tasks_n_get_times(int member_ops, int insert_ops, int delete_ops,
                                         std::function<double(int, int, int, int, int)> task, int min,
                                         int max) {
    // Hold the obtained times
    std::vector<double> times;
    times.reserve(1000);

    // Initial set of times
    for (int i = 0; i < 1000; i++) {
        times.push_back(task(member_ops, insert_ops, delete_ops, min, max));
    }

    // Get mean and std
    double mean = Utils::mean(&times);
    double stan_devi = Utils::standard_deviation(&times);

    // Get "n" for required confidence
    int n = Utils::operations_for_confidence(mean, stan_devi);

    // Repeat till the required confidence is achieved
    while (n > times.size()) {
        while (n > times.size()) {
            times.push_back(task(member_ops, insert_ops, delete_ops, min, max));
        }
        mean = Utils::mean(&times);
        stan_devi = Utils::standard_deviation(&times);
        n = Utils::operations_for_confidence(mean, stan_devi);
    }

    // Return results (n, mean, standard deviation)
    std::vector<double> results;
    results.push_back(n);
    results.push_back(mean);
    results.push_back(stan_devi);
    return results;
}

// Perform the set of operations (of parallel) till required confidence is achieved
std::vector<std::vector<double>> do_parallel_tasks_n_get_times(int member_ops, int insert_ops, int delete_ops,
                                                               std::function<double(int, int, int, int, int, int)> task,
                                                               int min,
                                                               int max) {
    std::vector<std::vector<double>> results;

    // Iterate per thread number
    for (int thr_cnt = 0; thr_cnt < 4; thr_cnt++) {
        // Hold the obtained times
        std::vector<double> time;
        time.reserve(1000);

        // Initial set of times
        for (int i = 0; i < 1000; i++) {
            time.push_back(task(THREAD_NUMBERS[thr_cnt], member_ops, insert_ops, delete_ops, min, max));
        }

        // Get mean and std
        double mean = Utils::mean(&time);
        double stan_devi = Utils::standard_deviation(&time);

        // Get "n" for required confidence
        int n = Utils::operations_for_confidence(mean, stan_devi);

        // Repeat till the required confidence is achieved
        while (n > time.size()) {
            while (n > time.size()) {
                time.push_back(task(THREAD_NUMBERS[thr_cnt], member_ops, insert_ops, delete_ops, min, max));
            }
            mean = Utils::mean(&time);
            stan_devi = Utils::standard_deviation(&time);
            n = Utils::operations_for_confidence(mean, stan_devi);
        }

        // Return results (n, mean, standard deviation)
        std::vector<double> result;
        result.push_back(n);
        result.push_back(mean);
        result.push_back(stan_devi);
        results.push_back(result);
    }
    return results;
}

#endif //CP_LAB_1_OPERATIONS_H
