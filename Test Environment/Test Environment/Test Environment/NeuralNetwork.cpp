#include "NeuralNetwork.h"

Scalar activationFunction(Scalar x)
{
	return  1 / (1 + exp(-x));
}

Scalar activationFunctionDerivative(Scalar x)
{
	return 	activationFunction(x) * (1 - activationFunction(x));
}


NeuralNetwork::NeuralNetwork(std::string file_path, double learningRate)
{
	this->learningRate = learningRate;
	loadModelInFile(file_path);
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

void NeuralNetwork::loadWeights(std::string nameFile)
{
	std::ifstream in(nameFile, std::ios::binary);

	while (!in.eof())
	{
		Matrix* matrix = new Matrix();
		typename Matrix::Index rows = 0, cols = 0;

		in.read((char*)(&rows), sizeof(typename Matrix::Index));
		in.read((char*)(&cols), sizeof(typename Matrix::Index));

		if (rows == 0)
			break;

		this->topology.push_back(rows - 1);

		matrix->resize(rows, cols);
		in.read((char*)matrix->data(), rows * cols * sizeof(typename Matrix::Scalar));

		weights.push_back(matrix);
	}
	in.close();
}

void NeuralNetwork::loadModelInFile(std::string file_path)
{
	deleteNN();
	
	loadWeights(file_path);

	this->topology.push_back((weights.back())->cols());

	for (uint i = 0; i < topology.size(); i++) {
		if (i == topology.size() - 1)
			neuronLayers.push_back(new RowVector(topology[i]));
		else
			neuronLayers.push_back(new RowVector(topology[i] + 1));

		deltas.push_back(new RowVector(neuronLayers.size()));

		if (i != topology.size() - 1) {
			neuronLayers.back()->coeffRef(topology[i]) = 1.0;
		}
	}
}

void NeuralNetwork::saveModelInFile(std::string file_path)
{
	std::ofstream out(file_path, std::ios::out | std::ios::binary | std::ios::trunc);
	for (auto matrix : this->weights)
	{
		typename Matrix::Index rows = matrix->rows(), cols = matrix->cols();
		out.write((char*)(&rows), sizeof(typename Matrix::Index));
		out.write((char*)(&cols), sizeof(typename Matrix::Index));
		out.write((char*)matrix->data(), rows * cols * sizeof(typename Matrix::Scalar));
	}

	out.close();
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

void NeuralNetwork::train(std::vector<RowVector*>& input_data, std::vector<RowVector*>& output_data, int epochs)
{
	for (uint j = 0; j < epochs; j++)
	{
		for (uint i = 0; i < input_data.size(); i++) {
			propagateForward(*input_data[i]);
			propagateBackward(*output_data[i]);
			std::cout << *(this->neuronLayers.back()) << std::endl;
		}
	}
}

NeuralNetwork::~NeuralNetwork()
{
	deleteNN();
}

RowVector* NeuralNetwork::forward(RowVector& input)
{
	propagateForward(input);
	return neuronLayers.back();
}

void NeuralNetwork::deleteNN()
{
	this->topology.clear();

	for (int i = 0; i < neuronLayers.size(); ++i)
		delete neuronLayers[i];

	neuronLayers.clear();

	for (int i = 0; i < weights.size(); ++i)
		delete weights[i];

	weights.clear();

	for (int i = 0; i < deltas.size(); ++i)
		delete deltas[i];

	deltas.clear();
}