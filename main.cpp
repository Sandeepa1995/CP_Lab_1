#include <iostream>
#include <cmath>
#include <time.h>
#include <vector>
#include <functional>
#include <limits>
#include <chrono>

#include "linked_list.h"
#include "utils.h"
#include "operations.h"

int main() {
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
    std::vector<std::vector<double>> case_3_mutex = do_parallel_tasks_n_get_times(5000, 2500, 2500, do_mutex_operations);
    std::vector<std::vector<double>> case_3_rwl = do_parallel_tasks_n_get_times(5000, 2500, 2500, do_rwl_operations);

    std::cout << "case_1_seq" << std::endl;
    std::cout << case_1_seq.at(0) << std::endl;
    std::cout << case_1_seq.at(1) << std::endl;
    std::cout << case_1_seq.at(2) << std::endl;

    std::cout << "case_1_mutex 1" << std::endl;
    std::cout << case_1_mutex.at(0).at(0) << std::endl;
    std::cout << case_1_mutex.at(0).at(1) << std::endl;
    std::cout << case_1_mutex.at(0).at(2) << std::endl;

    std::cout << "case_1_mutex 2" << std::endl;
    std::cout << case_1_mutex.at(1).at(0) << std::endl;
    std::cout << case_1_mutex.at(1).at(1) << std::endl;
    std::cout << case_1_mutex.at(1).at(2) << std::endl;

    std::cout << "case_1_mutex 4" << std::endl;
    std::cout << case_1_mutex.at(2).at(0) << std::endl;
    std::cout << case_1_mutex.at(2).at(1) << std::endl;
    std::cout << case_1_mutex.at(2).at(2) << std::endl;

    std::cout << "case_1_mutex 8" << std::endl;
    std::cout << case_1_mutex.at(3).at(0) << std::endl;
    std::cout << case_1_mutex.at(3).at(1) << std::endl;
    std::cout << case_1_mutex.at(3).at(2) << std::endl;

    std::cout << "case_1_rwl 1" << std::endl;
    std::cout << case_1_rwl.at(0).at(0) << std::endl;
    std::cout << case_1_rwl.at(0).at(1) << std::endl;
    std::cout << case_1_rwl.at(0).at(2) << std::endl;

    std::cout << "case_1_rwl 2" << std::endl;
    std::cout << case_1_rwl.at(1).at(0) << std::endl;
    std::cout << case_1_rwl.at(1).at(1) << std::endl;
    std::cout << case_1_rwl.at(1).at(2) << std::endl;

    std::cout << "case_1_rwl 4" << std::endl;
    std::cout << case_1_rwl.at(2).at(0) << std::endl;
    std::cout << case_1_rwl.at(2).at(1) << std::endl;
    std::cout << case_1_rwl.at(2).at(2) << std::endl;

    std::cout << "case_1_rwl 8" << std::endl;
    std::cout << case_1_rwl.at(3).at(0) << std::endl;
    std::cout << case_1_rwl.at(3).at(1) << std::endl;
    std::cout << case_1_rwl.at(3).at(2) << std::endl;
    
    
    

    std::cout << "case_2_seq" << std::endl;
    std::cout << case_2_seq.at(0) << std::endl;
    std::cout << case_2_seq.at(1) << std::endl;
    std::cout << case_2_seq.at(2) << std::endl;

    std::cout << "case_2_mutex 1" << std::endl;
    std::cout << case_2_mutex.at(0).at(0) << std::endl;
    std::cout << case_2_mutex.at(0).at(1) << std::endl;
    std::cout << case_2_mutex.at(0).at(2) << std::endl;

    std::cout << "case_2_mutex 2" << std::endl;
    std::cout << case_2_mutex.at(1).at(0) << std::endl;
    std::cout << case_2_mutex.at(1).at(1) << std::endl;
    std::cout << case_2_mutex.at(1).at(2) << std::endl;

    std::cout << "case_2_mutex 4" << std::endl;
    std::cout << case_2_mutex.at(2).at(0) << std::endl;
    std::cout << case_2_mutex.at(2).at(1) << std::endl;
    std::cout << case_2_mutex.at(2).at(2) << std::endl;

    std::cout << "case_2_mutex 8" << std::endl;
    std::cout << case_2_mutex.at(3).at(0) << std::endl;
    std::cout << case_2_mutex.at(3).at(1) << std::endl;
    std::cout << case_2_mutex.at(3).at(2) << std::endl;

    std::cout << "case_2_rwl 1" << std::endl;
    std::cout << case_2_rwl.at(0).at(0) << std::endl;
    std::cout << case_2_rwl.at(0).at(1) << std::endl;
    std::cout << case_2_rwl.at(0).at(2) << std::endl;

    std::cout << "case_2_rwl 2" << std::endl;
    std::cout << case_2_rwl.at(1).at(0) << std::endl;
    std::cout << case_2_rwl.at(1).at(1) << std::endl;
    std::cout << case_2_rwl.at(1).at(2) << std::endl;

    std::cout << "case_2_rwl 4" << std::endl;
    std::cout << case_2_rwl.at(2).at(0) << std::endl;
    std::cout << case_2_rwl.at(2).at(1) << std::endl;
    std::cout << case_2_rwl.at(2).at(2) << std::endl;

    std::cout << "case_2_rwl 8" << std::endl;
    std::cout << case_2_rwl.at(3).at(0) << std::endl;
    std::cout << case_2_rwl.at(3).at(1) << std::endl;
    std::cout << case_2_rwl.at(3).at(2) << std::endl;
    
    
    

    std::cout << "case_3_seq" << std::endl;
    std::cout << case_3_seq.at(0) << std::endl;
    std::cout << case_3_seq.at(1) << std::endl;
    std::cout << case_3_seq.at(2) << std::endl;

    std::cout << "case_3_mutex 1" << std::endl;
    std::cout << case_3_mutex.at(0).at(0) << std::endl;
    std::cout << case_3_mutex.at(0).at(1) << std::endl;
    std::cout << case_3_mutex.at(0).at(2) << std::endl;

    std::cout << "case_3_mutex 2" << std::endl;
    std::cout << case_3_mutex.at(1).at(0) << std::endl;
    std::cout << case_3_mutex.at(1).at(1) << std::endl;
    std::cout << case_3_mutex.at(1).at(2) << std::endl;

    std::cout << "case_3_mutex 4" << std::endl;
    std::cout << case_3_mutex.at(2).at(0) << std::endl;
    std::cout << case_3_mutex.at(2).at(1) << std::endl;
    std::cout << case_3_mutex.at(2).at(2) << std::endl;

    std::cout << "case_3_mutex 8" << std::endl;
    std::cout << case_3_mutex.at(3).at(0) << std::endl;
    std::cout << case_3_mutex.at(3).at(1) << std::endl;
    std::cout << case_3_mutex.at(3).at(2) << std::endl;

    std::cout << "case_3_rwl 1" << std::endl;
    std::cout << case_3_rwl.at(0).at(0) << std::endl;
    std::cout << case_3_rwl.at(0).at(1) << std::endl;
    std::cout << case_3_rwl.at(0).at(2) << std::endl;

    std::cout << "case_3_rwl 2" << std::endl;
    std::cout << case_3_rwl.at(1).at(0) << std::endl;
    std::cout << case_3_rwl.at(1).at(1) << std::endl;
    std::cout << case_3_rwl.at(1).at(2) << std::endl;

    std::cout << "case_3_rwl 4" << std::endl;
    std::cout << case_3_rwl.at(2).at(0) << std::endl;
    std::cout << case_3_rwl.at(2).at(1) << std::endl;
    std::cout << case_3_rwl.at(2).at(2) << std::endl;

    std::cout << "case_3_rwl 8" << std::endl;
    std::cout << case_3_rwl.at(3).at(0) << std::endl;
    std::cout << case_3_rwl.at(3).at(1) << std::endl;
    std::cout << case_3_rwl.at(3).at(2) << std::endl;

    return 0;
}