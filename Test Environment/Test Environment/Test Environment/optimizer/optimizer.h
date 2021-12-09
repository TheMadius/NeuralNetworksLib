#pragma once

#include "../neural/neuralnetwork.h"

class Optimizer
{
public:
	Optimizer();
	~Optimizer();

	virtual void minimize();

	void setNeuralNetwork(NeuralNetwork* net);

	void minimizeThread();

protected:
	NeuralNetwork* _n;

	double LEARNING_RATE;
};

