#include "RLnet.h"

vector<double> convertRowVinStdV(RowVector& input)
{
	vector<double> res(input.size());
	for(int i = 0 ; i < input.size(); ++i)
		res[i] = input.coeffRef(i);

	return res;
}

RowVector convertStdVinRowV(vector<double>& input)
{
	RowVector res(input.size());

	for (int i = 0; i < input.size(); ++i)
		res.coeffRef(i) = input[i];

	return res;
}

QModel::QModel(std::vector<uint32_t> v, double learningRate)
{
	model = new NeuralNetwork();
	opt = new Backpropagation();

	model->addLayer({ { "type", LayerType::INPUT },{ "size",v.front() } });
	for (int i = 1; i < v.size() - 1; ++i)
		model->addLayer({ { "type", LayerType::STANDARD },{ "size", v[i]} , { "activation",ActivationFunction::RELU } });
	
	model->addLayer({ { "type", LayerType::OUTPUT},{ "size", v.back()} , { "activation",ActivationFunction::LINEAR } });

	model->autogenerate();

	this->topology = v;

	opt->setNeuralNetwork(model);

	opt->setLearningRate(learningRate);
}

QModel::~QModel()
{
	delete this->model;
	delete this->opt;
}

int QModel::predict(RowVector& input, RowVector& legalMoves)
{
	auto res = model->predict(convertRowVinStdV(input));
	auto pred = convertStdVinRowV(res);

	RowVector::Index maxIndex;
	Scalar min = pred.minCoeff();

	pred = pred.array() - min;

	pred = pred.array() * legalMoves.array();

	pred.maxCoeff(&maxIndex);

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
	auto pred = model->predict(convertRowVinStdV(*input));
	auto res = convertStdVinRowV(pred);

	res = res.array() * legalMoves->array();

	return res;
}

void QModel::train(std::vector<RowVector*> &_input, std::vector<RowVector*> &_output)
{
	std::vector<const std::vector<double>*> input(_input.size());
	std::vector<const std::vector<double>*> output(_output.size());

	for (int i = 0; i < _input.size(); i++)
		input[i] = new vector<double>(convertRowVinStdV(*_input[i]));

	for (int i = 0; i < _output.size(); i++)
		output[i] = new vector<double>(convertRowVinStdV(*_output[i]));

	opt->backpropagate(input, output);

	for (int i = 0; i < input.size(); i++)
		delete input[i];

	for (int i = 0; i < output.size(); i++)
		delete output[i];
}

