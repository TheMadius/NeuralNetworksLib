#include "RLnet.h"

QModel::QModel()
{
	model = new NeuralNetwork({ 64*2, 200, 500, 2000, 4096 }, 0.01);
}

QModel::~QModel()
{
	delete this->model;
}

void QModel::predict(RowVector& input)
{
	
}

void QModel::explore(RowVector& input)
{

}

void QModel::train(RowVector& input, RowVector& output)
{

}

