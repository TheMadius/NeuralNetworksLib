#pragma once
#include "Eigen3/Eigen"
#include <iostream>
#include <vector>
#include <ctime>
#include <typeinfo>
 
typedef float Scalar;
typedef Eigen::MatrixXf Matrix;
typedef Eigen::RowVectorXf RowVector;
typedef Eigen::VectorXf ColVector;
typedef std::vector<RowVector*> data;
typedef unsigned int uint;


class NeuralNetwork {
public:
    NeuralNetwork(std::vector<uint32_t> topology, Scalar learningRate = Scalar(0.005));
 
    void train(std::vector<RowVector*>& input_data, std::vector<RowVector*>& output_data, int epochs = 1);
    void propagateForward(RowVector& input);
    void propagateBackward(RowVector& output);
    RowVector forward(RowVector& input);
    void calcErrors(RowVector& output);
    void updateWeights();
    ~NeuralNetwork();

private:
    std::vector<RowVector*> neuronLayers; 
    std::vector<RowVector*> deltas;
    std::vector<Matrix*> weights; 
    std::vector<uint32_t> topology;
    Scalar learningRate;
};