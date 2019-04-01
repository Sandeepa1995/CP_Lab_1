//
// Created by damitha on 4/1/19.
//

#ifndef CP_LAB_1_UTILS_H
#define CP_LAB_1_UTILS_H

#include <cmath>

namespace Utils{
    double mean(std::vector<double>* vec);
    double standard_deviation(std::vector<double>* vec);
    int operations_for_confidence(double mean, double standard_deviation, float z_val = 1.96, float req_accuracy = 0.05);

}

double Utils::mean(std::vector<double>* vec){
    double total = 0.0;
    unsigned long size = vec->size();
    for (int i = 0; i< size; i++){
        total += vec->at(i);
    }
    return total/size;
}

double Utils::standard_deviation(std::vector<double>* vec){
    double standard_deviation = 0.0;
    unsigned long size = vec->size();
    double mean = Utils::mean(vec);

    for (int i = 0; i< size; i++){
        standard_deviation += pow(vec->at(i)-mean, 2);
    }

    return sqrt(standard_deviation/size);
}

int Utils::operations_for_confidence(double mean, double standard_deviation, float z_val, float req_accuracy){
    return (int)ceil(pow(z_val*standard_deviation/(req_accuracy*mean),2));
}

#endif //CP_LAB_1_UTILS_H
