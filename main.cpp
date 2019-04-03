#include <iostream>
#include <cmath>
#include <time.h>
#include <vector>
#include <functional>
#include <limits>
#include <chrono>
#include <fstream>
#include <sstream>

#include "linked_list.h"
#include "utils.h"
#include "operations.h"


int main() {
    std::string PROJECT_PATH = "/home/damitha/CLionProjects/CP_Lab_1";
    int i;

    // Output files
    // Case 1
    // Seq
    std::ofstream out_1_seq;
    out_1_seq.open(PROJECT_PATH + "/outputs/out_1_seq.csv", std::ofstream::out | std::ofstream::trunc);
    // Mutex
    std::ofstream out_1_mutex;
    out_1_mutex.open(PROJECT_PATH + "/outputs/out_1_mutex.csv", std::ofstream::out | std::ofstream::trunc);
    // RWLock
    std::ofstream out_1_rwl;
    out_1_rwl.open(PROJECT_PATH + "/outputs/out_1_rwl.csv", std::ofstream::out | std::ofstream::trunc);
    // Case 2
    // Seq
    std::ofstream out_2_seq;
    out_2_seq.open(PROJECT_PATH + "/outputs/out_2_seq.csv", std::ofstream::out | std::ofstream::trunc);
    // Mutex
    std::ofstream out_2_mutex;
    out_2_mutex.open(PROJECT_PATH + "/outputs/out_2_mutex.csv", std::ofstream::out | std::ofstream::trunc);
    // RWLock
    std::ofstream out_2_rwl;
    out_2_rwl.open(PROJECT_PATH + "/outputs/out_2_rwl.csv", std::ofstream::out | std::ofstream::trunc);
    // Case 3
    // Seq
    std::ofstream out_3_seq;
    out_3_seq.open(PROJECT_PATH + "/outputs/out_3_seq.csv", std::ofstream::out | std::ofstream::trunc);
    // Mutex
    std::ofstream out_3_mutex;
    out_3_mutex.open(PROJECT_PATH + "/outputs/out_3_mutex.csv", std::ofstream::out | std::ofstream::trunc);
    // RWLock
    std::ofstream out_3_rwl;
    out_3_rwl.open(PROJECT_PATH + "/outputs/out_3_rwl.csv", std::ofstream::out | std::ofstream::trunc);

    ///Case 1
    ///n = 1,000 and m = 10,000, m Member = 0.99, m Insert = 0.005, m Delete = 0.005
    std::vector<double> case_1_seq = do_tasks_n_get_times(9900, 50, 50, do_seq_operations);
    std::vector<std::vector<double>> case_1_mutex = do_parallel_tasks_n_get_times(9900, 50, 50, do_mutex_operations);
    std::vector<std::vector<double>> case_1_rwl = do_parallel_tasks_n_get_times(9900, 50, 50, do_rwl_operations);

    ///Case 2
    ///n = 1,000 and m = 10,000, m Member = 0.90, m Insert = 0.05, m Delete = 0.05
    std::vector<double> case_2_seq = do_tasks_n_get_times(9000, 500, 500, do_seq_operations);
    std::vector<std::vector<double>> case_2_mutex = do_parallel_tasks_n_get_times(9000, 500, 500, do_mutex_operations);
    std::vector<std::vector<double>> case_2_rwl = do_parallel_tasks_n_get_times(9000, 500, 500, do_rwl_operations);

    ///Case 3
    ///n = 1,000 and m = 10,000, m Member = 0.50, m Insert = 0.25, m Delete = 0.25
    std::vector<double> case_3_seq = do_tasks_n_get_times(5000, 2500, 2500, do_seq_operations);
    std::vector<std::vector<double>> case_3_mutex = do_parallel_tasks_n_get_times(5000, 2500, 2500,
                                                                                  do_mutex_operations);
    std::vector<std::vector<double>> case_3_rwl = do_parallel_tasks_n_get_times(5000, 2500, 2500, do_rwl_operations);

    // Write outputs
    // Case 1
    out_1_seq << 1 << ", " << case_1_seq.at(1) << "\n";
    for (i = 0; i < 4; i++) {
        out_1_mutex << THREAD_NUMBERS[i] << ", " << case_1_mutex.at(i).at(1) << "\n";
        out_1_rwl << THREAD_NUMBERS[i] << ", " << case_1_rwl.at(i).at(1) << "\n";
    }
    // Case 2
    out_2_seq << 1 << ", " << case_2_seq.at(1) << "\n";
    for (i = 0; i < 4; i++) {
        out_2_mutex << THREAD_NUMBERS[i] << ", " << case_2_mutex.at(i).at(1) << "\n";
        out_2_rwl << THREAD_NUMBERS[i] << ", " << case_2_rwl.at(i).at(1) << "\n";
    }
    // Case 3
    out_3_seq << 1 << ", " << case_3_seq.at(1) << "\n";
    for (i = 0; i < 4; i++) {
        out_3_mutex << THREAD_NUMBERS[i] << ", " << case_3_mutex.at(i).at(1) << "\n";
        out_3_rwl << THREAD_NUMBERS[i] << ", " << case_3_rwl.at(i).at(1) << "\n";
    }

    // Print results
    // Case 1
    std::cout << "Case 1" << std::endl;
    std::cout << "Operation: Seqen, Threads: 1, Mean: " << case_1_seq.at(1) << " ,Std: " << case_1_seq.at(2) <<
              " ,Iterations: " << case_1_seq.at(0) << std::endl;
    for (i = 0; i < 4; i++) {
        std::cout << "Operation: Mutex, Threads: " << THREAD_NUMBERS[i] << " ,Mean: " << case_1_mutex.at(i).at(1)
                  << " ,Std: " << case_1_mutex.at(i).at(2) << " ,Iterations: " << case_1_mutex.at(i).at(0) << std::endl;
    }
    for (i = 0; i < 4; i++) {
        std::cout << "Operation: RWLoc, Threads: " << THREAD_NUMBERS[i] << " ,Mean: " << case_1_rwl.at(i).at(1)
                  << " ,Std: "
                  << case_1_rwl.at(i).at(2) << " ,Iterations: " << case_1_rwl.at(i).at(0) << std::endl;
    }

    // Case 2
    std::cout << "Case 2" << std::endl;
    std::cout << "Operation: Seqen, Threads: 1, Mean: " << case_2_seq.at(1) << " ,Std: " << case_2_seq.at(2) <<
              " ,Iterations: " << case_2_seq.at(0) << std::endl;
    for (i = 0; i < 4; i++) {
        std::cout << "Operation: Mutex, Threads: " << THREAD_NUMBERS[i] << " ,Mean: " << case_2_mutex.at(i).at(1)
                  << " ,Std: " << case_2_mutex.at(i).at(2) << " ,Iterations: " << case_2_mutex.at(i).at(0) << std::endl;
    }
    for (i = 0; i < 4; i++) {
        std::cout << "Operation: RWLoc, Threads: " << THREAD_NUMBERS[i] << " ,Mean: " << case_2_rwl.at(i).at(1)
                  << " ,Std: "
                  << case_2_rwl.at(i).at(2) << " ,Iterations: " << case_2_rwl.at(i).at(0) << std::endl;
    }

    // Case 3
    std::cout << "Case 3" << std::endl;
    std::cout << "Operation: Seqen, Threads: 1, Mean: " << case_3_seq.at(1) << " ,Std: " << case_3_seq.at(2) <<
              " ,Iterations: " << case_3_seq.at(0) << std::endl;
    for (i = 0; i < 4; i++) {
        std::cout << "Operation: Mutex, Threads: " << THREAD_NUMBERS[i] << " ,Mean: " << case_3_mutex.at(i).at(1)
                  << " ,Std: " << case_3_mutex.at(i).at(2) << " ,Iterations: " << case_3_mutex.at(i).at(0) << std::endl;
    }
    for (i = 0; i < 4; i++) {
        std::cout << "Operation: RWLoc, Threads: " << THREAD_NUMBERS[i] << " ,Mean: " << case_3_rwl.at(i).at(1)
                  << " ,Std: "
                  << case_3_rwl.at(i).at(2) << " ,Iterations: " << case_3_rwl.at(i).at(0) << std::endl;
    }

    return 0;
}