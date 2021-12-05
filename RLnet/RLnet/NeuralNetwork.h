#pragma once
#include "Eigen3/Eigen"
#include <iostream>
#include <vector>
#include <typeinfo>
 
typedef float Scalar;
typedef Eigen::MatrixXd Matrix;
typedef Eigen::RowVectorXd RowVector;
typedef Eigen::VectorXd ColVector;
typedef std::vector<RowVector*> data;
typedef unsigned int uint;


class NeuralNetwork {
public:
    NeuralNetwork(std::vector<uint32_t> topology, Scalar learningRate = Scalar(0.005));
 
    void train(std::vector<RowVector*> input_data, std::vector<RowVector*> output_data, int epochs = 1);
    void propagateForward(RowVector& input);
    void propagateBackward(RowVector& output);
    RowVector forward(RowVector& input);
    void calcErrors(RowVector& output);
    void updateWeights();
    ~NeuralNetwork();

private:
    std::vector<RowVector*> neuronLayers; 
    std::vector<RowVector*> deltas;
    RowVector* deltas_norm;
    std::vector<Matrix*> weights; 
    std::vector<uint32_t> topology;
    Scalar learningRate;
};