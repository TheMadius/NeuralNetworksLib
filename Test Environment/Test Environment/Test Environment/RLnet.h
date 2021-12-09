#pragma once
#include "neural/neuralnetwork.h"
#include "misc/functions.h"
#include "optimizer/backpropagation.h"
#include "dataset/dataset.h"
#include "Eigen3/Eigen"

#include <iostream>
#include <algorithm> 
#include <random>
#include <vector>
#include <ctime>
#include <map>

class QModel
{
public:
	QModel(std::vector<uint32_t> v, double learningRate, string name_file = "");
	void train(std::vector<const vector<double>*>& input, std::vector<const vector<double>*>& output);
	vector<double> forward(const vector<double>* input, const vector<double>* legalMoves);
	int predict(vector<double>& input, vector<double>& legalMovess);
	int explore(vector<double>& legalMoves);
	void saveModel(string file);
	~QModel();

private:
	NeuralNetwork* model;
	Backpropagation* opt;
	std::vector<uint32_t> topology;
};

