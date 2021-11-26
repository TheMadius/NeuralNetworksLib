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
	RowVector::Index maxIndex;
	Scalar min = pred->minCoeff();

	*pred = pred->array() - min;

	*pred = pred->array() * legalMoves.array();

	pred->maxCoeff(&maxIndex);

	return maxIndex;
}

int QModel::explore(RowVector& legalMoves)
{
	srand(static_cast<unsigned int>(time(NULL)));

	RowVector random(topology.back());
	RowVector::Index maxIndex;
	Scalar min = -1;

	random.setRandom();

	random = random.array() - min;

	random = random.array() * legalMoves.array();
	random.maxCoeff(&maxIndex);

	return maxIndex;
}

RowVector QModel::forward(RowVector* input, RowVector* legalMoves)
{
	auto res = *(this->model->forward(*input));
	res = res.array() - res.minCoeff();
	res = res.array() * legalMoves->array();

	return res;
}

void QModel::train(std::vector<RowVector*> &input, std::vector<RowVector*> &output)
{
	model->train(input, output, 1);
}

