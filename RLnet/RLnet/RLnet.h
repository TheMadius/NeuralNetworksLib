#pragma once
#include "NeuralNetwork.h"
#include <iostream>
#include <vector>
#include <map>

class QModel
{
public:
	QModel();
	~QModel();
	void predict(RowVector& input);
	void explore(RowVector& input);
	void train(RowVector& input, RowVector& output);
private:
	NeuralNetwork* model;
};

