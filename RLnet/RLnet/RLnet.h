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
private:
	NeuralNetwork* model;
};

