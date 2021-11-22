#pragma once
#include "NeuralNetwork.h"
#include <iostream>
#include <vector>
#include <map>

class QModel
{
public:
	QModel(std::vector<uint32_t> v);
	int predict(RowVector& input, RowVector& legalMoves);
	void train(RowVector& input, RowVector& output);
	int explore(RowVector& legalMoves);
	~QModel();

private:
	NeuralNetwork* model;
	std::vector<uint32_t> topology;
};
