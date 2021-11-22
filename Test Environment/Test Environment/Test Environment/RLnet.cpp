#include "RLnet.h"

QModel::QModel(std::vector<uint32_t> v)
{
	model = new NeuralNetwork(v, 0.01);
	this->topology = v;
}

QModel::~QModel()
{
	delete this->model;
}

int QModel::predict(RowVector& input, RowVector& legalMoves)
{
	auto pred = model->forward(input);
	pred = pred.array() * legalMoves.array();
	RowVector::Index maxIndex;

	pred.maxCoeff(&maxIndex);

	return maxIndex;
}

int QModel::explore(RowVector& legalMoves)
{
	RowVector random(topology.back());
	random.setRandom();

	random = random.array() * legalMoves.array();
	RowVector::Index maxIndex;
	random.maxCoeff(&maxIndex);

	return maxIndex;
}

void QModel::train(RowVector& input, RowVector& output)
{
	model->train({ &input }, { &output }, 1);
}

