#include "NeuralNetwork.h"

Scalar activationFunction(Scalar x)
{
	return (x >= 0) ? x : 0.01 * x;
}

Scalar activationFunctionDerivative(Scalar x)
{
	return 	(x >= 0) ? 1 : 0.01 ;
}

NeuralNetwork::NeuralNetwork(std::vector<uint> topology, Scalar learningRate)
{
	srand(static_cast<unsigned int>(time(NULL)));

	this->topology = topology;
	this->learningRate = learningRate;

	for (uint i = 0; i < topology.size(); i++) {
		if (i == topology.size() - 1)
			neuronLayers.push_back(new RowVector(topology[i]));
		else
			neuronLayers.push_back(new RowVector(topology[i] + 1));

		deltas.push_back(new RowVector(neuronLayers.size()));

		if (i != topology.size() - 1) {
			neuronLayers.back()->coeffRef(topology[i]) = 1.0;
		}

		if (i > 0) {
			if (i != topology.size() - 1) {
				weights.push_back(new Matrix(topology[i - 1] + 1, topology[i] + 1));
				weights.back()->setRandom();
			}
			else {
				weights.push_back(new Matrix(topology[i - 1] + 1, topology[i]));
				weights.back()->setRandom();
			}
		}
	}
};

void NeuralNetwork::propagateForward(RowVector& input)
{
	neuronLayers.front()->block(0, 0, 1, neuronLayers.front()->size() - 1) = input;

	for (uint i = 1; i < topology.size(); i++) {
		(*neuronLayers[i]) = (*neuronLayers[i - 1]) * (*weights[i - 1]);
	}

	for (uint i = 1; i < topology.size() - 1; i++) {
		neuronLayers[i]->block(0, 0, 1, topology[i]).unaryExpr(std::ptr_fun(activationFunction));
	}
}

void NeuralNetwork::calcErrors(RowVector& output)
{
	(*deltas.back()) = output - (*neuronLayers.back());

	for (uint i = topology.size() - 2; i > 0; i--) {
		(*deltas[i]) = (*deltas[i + 1]) * (weights[i]->transpose());
	}
}

void NeuralNetwork::updateWeights()
{
	for (uint i = 0; i < topology.size() - 1; i++) {
		if (i != topology.size() - 2) {
			for (uint c = 0; c < weights[i]->cols() - 1; c++) {
				for (uint r = 0; r < weights[i]->rows(); r++) {
					weights[i]->coeffRef(r, c) += learningRate * deltas[i + 1]->coeffRef(c) * activationFunctionDerivative(neuronLayers[i + 1]->coeffRef(c)) * neuronLayers[i]->coeffRef(r);
				}
			}
		}
		else {
			for (uint c = 0; c < weights[i]->cols(); c++) {
				for (uint r = 0; r < weights[i]->rows(); r++) {
					weights[i]->coeffRef(r, c) += learningRate * deltas[i + 1]->coeffRef(c) * activationFunctionDerivative(neuronLayers[i + 1]->coeffRef(c)) * neuronLayers[i]->coeffRef(r);
				}
			}
		}
	}
}

void NeuralNetwork::propagateBackward(RowVector& output)
{
	calcErrors(output);
	updateWeights();
}

void NeuralNetwork::train(std::vector<RowVector*> input_data, std::vector<RowVector*> output_data, int epochs)
{
	for (uint j = 0; j < epochs; j++)
	{
		for (uint i = 0; i < input_data.size(); i++) {
			propagateForward(*input_data[i]);
			propagateBackward(*output_data[i]);
		}
	}
}

NeuralNetwork::~NeuralNetwork()
{
	for (int i = 0; i < neuronLayers.size(); ++i)
		delete neuronLayers[i];

	for (int i = 0; i < weights.size(); ++i)
		delete weights[i];

	for (int i = 0; i < deltas.size(); ++i)
		delete deltas[i];
}


RowVector NeuralNetwork::forward(RowVector& input)
{
	propagateForward(input);
	return *neuronLayers.back();
}
