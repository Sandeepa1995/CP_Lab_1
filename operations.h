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

int THREAD_NUMBERS[4] = {1, 2, 4, 8};

pthread_mutex_t mutex;
pthread_rwlock_t rwlock;
Linked_List lst;

std::vector<double> do_tasks_n_get_times(int member_ops, int insert_ops, int delete_ops,
                                         std::function<double(int, int, int, int, int)> task, int min = 0,
                                         int max = 65536);

double do_seq_operations(int member_ops, int insert_ops, int delete_ops, int min = 0, int max = 65536);

std::vector<std::vector<double>> do_parallel_tasks_n_get_times(int member_ops, int insert_ops, int delete_ops,
                                                               std::function<double(int, int, int, int, int, int)> task,
                                                               int min = 0,
                                                               int max = 65536);

double do_mutex_operations(int thread_cnt, int member_ops, int insert_ops, int delete_ops, int min, int max);
void *mutex_operations(void *threadarg);

double do_rwl_operations(int thread_cnt, int member_ops, int insert_ops, int delete_ops, int min, int max);
void *rwl_operations(void *threadarg);


struct thread_data {
    int member_ops;
    int insert_ops;
    int delete_ops;
    int min;
    int max;
};

double do_seq_operations(int member_ops, int insert_ops, int delete_ops, int min, int max) {
    lst = Linked_List::generate_random_list(1000);
    srand(time(nullptr));
    int i;

    std::chrono::time_point<std::chrono::high_resolution_clock> t1, t2;
//    clock_t t1, t2;

    t1 = std::chrono::high_resolution_clock::now();
//    t1 = clock();

    for (i = 0; i < member_ops; i++) {
        lst.member(rand() % (max - min) + min);
    }

    int min_operation = std::min(insert_ops, delete_ops);
    for (i = 0; i < min_operation; i++) {
        lst.insert_node(rand() % (max - min) + min);
        lst.delete_node(rand() % (max - min) + min);
    }

    int remain_insert = insert_ops - min_operation;
    for (i = 0; i < remain_insert; i++) {
        lst.insert_node(rand() % (max - min) + min);
    }

    int remain_delete = delete_ops - min_operation;
    for (i = 0; i < remain_delete; i++) {
        lst.delete_node(rand() % (max - min) + min);
    }

    t2 = std::chrono::high_resolution_clock::now();

    lst.free_list();
//    t2 = clock();

    return std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
//    return ((double)t2 - (double)t1)*1000000/CLOCKS_PER_SEC;
}

double do_mutex_operations(int thread_cnt, int member_ops, int insert_ops, int delete_ops, int min, int max) {
    lst = Linked_List::generate_random_list(1000);
    srand(time(nullptr));

    std::chrono::time_point<std::chrono::high_resolution_clock> t1, t2;
    pthread_t threads[thread_cnt];
    struct thread_data td[thread_cnt];
    int i;

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

    pthread_mutex_init(&mutex, nullptr);
    t1 = std::chrono::high_resolution_clock::now();

    for (i = 0; i < thread_cnt; i++)
        pthread_create(&threads[i], nullptr, mutex_operations, (void *) &td[i]);

    for (i = 0; i < thread_cnt; i++)
        pthread_join(threads[i], nullptr);

    t2 = std::chrono::high_resolution_clock::now();
    pthread_mutex_destroy(&mutex);
    lst.free_list();

    return std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
}

void *mutex_operations(void *threadarg) {
    struct thread_data *data;
    data = (struct thread_data *) threadarg;
    int i;

    for (i = 0; i < data->member_ops; i++) {
        pthread_mutex_lock(&mutex);
        lst.member(rand() % (data->max - data->min) + data->min);
        pthread_mutex_unlock(&mutex);
    }

    int min_operation = std::min(data->insert_ops, data->delete_ops);
    for (i = 0; i < min_operation; i++) {
        pthread_mutex_lock(&mutex);
        lst.insert_node(rand() % (data->max - data->min) + data->min);
        pthread_mutex_unlock(&mutex);

        pthread_mutex_lock(&mutex);
        lst.delete_node(rand() % (data->max - data->min) + data->min);
        pthread_mutex_unlock(&mutex);
    }

    int remain_insert = data->insert_ops - min_operation;
    for (i = 0; i < remain_insert; i++) {
        pthread_mutex_lock(&mutex);
        lst.insert_node(rand() % (data->max - data->min) + data->min);
        pthread_mutex_unlock(&mutex);
    }

    int remain_delete = data->delete_ops - min_operation;
    for (i = 0; i < remain_delete; i++) {
        pthread_mutex_lock(&mutex);
        lst.delete_node(rand() % (data->max - data->min) + data->min);
        pthread_mutex_unlock(&mutex);
    }

    return nullptr;
}

double do_rwl_operations(int thread_cnt, int member_ops, int insert_ops, int delete_ops, int min, int max) {
    lst = Linked_List::generate_random_list(1000);
    srand(time(nullptr));

    std::chrono::time_point<std::chrono::high_resolution_clock> t1, t2;
    pthread_t threads[thread_cnt];
    struct thread_data td[thread_cnt];
    int i;

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

    pthread_rwlock_init(&rwlock, NULL);
    t1 = std::chrono::high_resolution_clock::now();

    for (i = 0; i < thread_cnt; i++)
        pthread_create(&threads[i], nullptr, rwl_operations, (void *) &td[i]);

    for (i = 0; i < thread_cnt; i++)
        pthread_join(threads[i], nullptr);

    t2 = std::chrono::high_resolution_clock::now();
    pthread_rwlock_destroy(&rwlock);
    lst.free_list();

    return std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
}

void *rwl_operations(void *threadarg) {
    struct thread_data *data;
    data = (struct thread_data *) threadarg;
    int i;

    for (i = 0; i < data->member_ops; i++) {
        pthread_rwlock_rdlock(&rwlock);
        lst.member(rand() % (data->max - data->min) + data->min);
        pthread_rwlock_unlock(&rwlock);
    }

    int min_operation = std::min(data->insert_ops, data->delete_ops);
    for (i = 0; i < min_operation; i++) {
        pthread_rwlock_wrlock(&rwlock);
        lst.insert_node(rand() % (data->max - data->min) + data->min);
        pthread_rwlock_unlock(&rwlock);

        pthread_rwlock_wrlock(&rwlock);
        lst.delete_node(rand() % (data->max - data->min) + data->min);
        pthread_rwlock_unlock(&rwlock);
    }

    int remain_insert = data->insert_ops - min_operation;
    for (i = 0; i < remain_insert; i++) {
        pthread_rwlock_wrlock(&rwlock);
        lst.insert_node(rand() % (data->max - data->min) + data->min);
        pthread_rwlock_unlock(&rwlock);
    }

    int remain_delete = data->delete_ops - min_operation;
    for (i = 0; i < remain_delete; i++) {
        pthread_rwlock_wrlock(&rwlock);
        lst.delete_node(rand() % (data->max - data->min) + data->min);
        pthread_rwlock_unlock(&rwlock);
    }

    return nullptr;
}

std::vector<double> do_tasks_n_get_times(int member_ops, int insert_ops, int delete_ops,
                                         std::function<double(int, int, int, int, int)> task, int min,
                                         int max) {
    std::vector<double> times;
    times.reserve(10);

    for (int i = 0; i < 10; i++) {
        times.push_back(task(member_ops, insert_ops, delete_ops, min, max));
    }

    double mean = Utils::mean(&times);
    double stan_devi = Utils::standard_deviation(&times);

    int n = Utils::operations_for_confidence(mean, stan_devi);

    while (n > times.size()) {
        while (n > times.size()) {
            times.push_back(task(member_ops, insert_ops, delete_ops, min, max));
        }
        mean = Utils::mean(&times);
        stan_devi = Utils::standard_deviation(&times);
        n = Utils::operations_for_confidence(mean, stan_devi);
    }

    std::vector<double> results;
    results.push_back(n);
    results.push_back(mean);
    results.push_back(stan_devi);
    return results;
}

std::vector<std::vector<double>> do_parallel_tasks_n_get_times(int member_ops, int insert_ops, int delete_ops,
                                                               std::function<double(int, int, int, int, int, int)> task,
                                                               int min,
                                                               int max) {
    std::vector<std::vector<double>> results;

    for (int thr_cnt = 0; thr_cnt < 4; thr_cnt++) {
        std::vector<double> time;
        time.reserve(10);

        for (int i = 0; i < 10; i++) {
            time.push_back(task(THREAD_NUMBERS[thr_cnt], member_ops, insert_ops, delete_ops, min, max));
        }

        double mean = Utils::mean(&time);
        double stan_devi = Utils::standard_deviation(&time);

        int n = Utils::operations_for_confidence(mean, stan_devi);

        while (n > time.size()) {
            while (n > time.size()) {
                time.push_back(task(THREAD_NUMBERS[thr_cnt], member_ops, insert_ops, delete_ops, min, max));
            }
            mean = Utils::mean(&time);
            stan_devi = Utils::standard_deviation(&time);
            n = Utils::operations_for_confidence(mean, stan_devi);
        }

        std::vector<double> result;
        result.push_back(n);
        result.push_back(mean);
        result.push_back(stan_devi);
        results.push_back(result);
    }
    return results;
}

#endif //CP_LAB_1_OPERATIONS_H
