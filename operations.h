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
    int rand_num, chk;
    int local_member = 0, local_insert = 0, local_delete = 0;
    float total_ops = member_ops + insert_ops + delete_ops;
    float member_percen = member_ops / total_ops, insert_percen = insert_ops / total_ops, delete_percen =
            delete_ops / total_ops;
    float rand_operation;

    std::chrono::time_point<std::chrono::high_resolution_clock> t1, t2;

    t1 = std::chrono::high_resolution_clock::now();

    while (local_member < member_ops || local_insert < insert_ops || local_delete < delete_ops) {
        rand_operation = rand() / (float) RAND_MAX;
        if ((rand_operation < member_percen) && (local_member < member_ops)){
            lst.member(rand() % (max - min) + min);
            local_member++;
        } else if((rand_operation < (member_percen + insert_percen)) && (local_insert < insert_ops)){
            rand_num = rand() % (max - min) + min;
            chk = lst.insert_node(rand_num);
            if (chk == 1){
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
            local_insert++;
        } else if (local_delete < delete_ops){
            rand_num = rand() % (max - min) + min;
            chk = lst.delete_node(rand_num);
            if (chk == 1){
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
            local_delete++;
        } else if (local_member < member_ops){
            lst.member(rand() % (max - min) + min);
            local_member++;
        } else if (local_insert < insert_ops){
            lst.insert_node(rand() % (max - min) + min);
            local_insert++;
        }
    }
    t2 = std::chrono::high_resolution_clock::now();

    lst.free_list();

    return std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
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
    int rand_num, chk;
    int local_member = 0, local_insert = 0, local_delete = 0;
    float total_ops = data->member_ops + data->insert_ops + data->delete_ops;
    float member_percen = data->member_ops / total_ops, insert_percen = data->insert_ops / total_ops, delete_percen =
            data->delete_ops / total_ops;
    float rand_operation;

    while (local_member < data->member_ops || local_insert < data->insert_ops || local_delete < data->delete_ops) {
        rand_operation = rand() / (float) RAND_MAX;
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
            if (chk == 1){
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
            local_insert++;
        } else if (local_delete < data->delete_ops){
            rand_num = rand() % (data->max - data->min) + data->min;
            pthread_mutex_lock(&mutex);
            chk = lst.delete_node(rand_num);
            pthread_mutex_unlock(&mutex);
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
            local_delete++;
        } else if (local_member < data->member_ops){
            pthread_mutex_lock(&mutex);
            lst.member(rand() % (data->max - data->min) + data->min);
            pthread_mutex_unlock(&mutex);
            local_member++;
        } else if (local_insert < data->insert_ops){
            rand_num = rand() % (data->max - data->min) + data->min;
            pthread_mutex_lock(&mutex);
            chk = lst.insert_node(rand_num);
            pthread_mutex_unlock(&mutex);
            if (chk == 1){
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
            local_insert++;
        }
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
    int rand_num, chk;
    int local_member = 0, local_insert = 0, local_delete = 0;
    float total_ops = data->member_ops + data->insert_ops + data->delete_ops;
    float member_percen = data->member_ops / total_ops, insert_percen = data->insert_ops / total_ops, delete_percen =
            data->delete_ops / total_ops;
    float rand_operation;

    while (local_member < data->member_ops || local_insert < data->insert_ops || local_delete < data->delete_ops) {
        rand_operation = rand() / (float) RAND_MAX;
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
            if (chk == 1){
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
            local_insert++;
        } else if (local_delete < data->delete_ops){
            rand_num = rand() % (data->max - data->min) + data->min;
            pthread_rwlock_wrlock(&rwlock);
            lst.delete_node(rand() % (data->max - data->min) + data->min);
            pthread_rwlock_unlock(&rwlock);
            if (chk == 1){
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
            local_delete++;
        } else if (local_member < data->member_ops){
            pthread_rwlock_rdlock(&rwlock);
            lst.member(rand() % (data->max - data->min) + data->min);
            pthread_rwlock_unlock(&rwlock);
            local_member++;
        } else if (local_insert < data->insert_ops){
            rand_num = rand() % (data->max - data->min) + data->min;
            pthread_rwlock_wrlock(&rwlock);
            chk = lst.insert_node(rand_num);
            pthread_rwlock_unlock(&rwlock);
            if (chk == 1){
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
            local_insert++;
        }
    }
    return nullptr;
}

std::vector<double> do_tasks_n_get_times(int member_ops, int insert_ops, int delete_ops,
                                         std::function<double(int, int, int, int, int)> task, int min,
                                         int max) {
    std::vector<double> times;
    times.reserve(10);

    for (int i = 0; i < 1000; i++) {
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

        for (int i = 0; i < 1000; i++) {
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
