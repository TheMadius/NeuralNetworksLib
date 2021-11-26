#pragma once
#include "NeuralNetwork.h"
#include <iostream>
#include <vector>
#include <ctime>
#include <map>

class QModel
{
public:
	QModel(std::vector<uint32_t> v);
	int predict(RowVector& input, RowVector& legalMoves);
	void train(std::vector<RowVector*>& input, std::vector<RowVector*>& output);
	int explore(RowVector& legalMoves);
	RowVector forward(RowVector& input, RowVector& legalMoves);
	~QModel();

private:
	NeuralNetwork* model;
	std::vector<uint32_t> topology;
};

