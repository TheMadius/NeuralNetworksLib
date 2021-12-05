#pragma once
#include "neural/neuralnetwork.h"
#include "misc/functions.h"
#include "optimizer/backpropagation.h"
#include "dataset/dataset.h"
#include "Eigen3/Eigen"

#include <iostream>
#include <vector>
#include <ctime>
#include <map>

typedef double Scalar;
typedef Eigen::MatrixXd Matrix;
typedef Eigen::RowVectorXd RowVector;
typedef Eigen::VectorXd ColVector;
typedef std::vector<RowVector*> data;

class QModel
{
public:
	QModel(std::vector<uint32_t> v, double learningRate);
	int predict(RowVector& input, RowVector& legalMoves);
	void train(std::vector<RowVector*>& input, std::vector<RowVector*>& output);
	int explore(RowVector& legalMoves);
	RowVector forward(RowVector* input, RowVector* legalMoves);
	~QModel();

private:
	NeuralNetwork* model;
	Backpropagation* opt;
	std::vector<uint32_t> topology;
};

