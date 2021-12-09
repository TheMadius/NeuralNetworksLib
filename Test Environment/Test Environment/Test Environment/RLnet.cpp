#include "RLnet.h"

QModel::QModel(std::vector<uint32_t> v, double learningRate, string name_file)
{
	model = new NeuralNetwork();
	opt = new Backpropagation();

	model->addLayer({ { "type", LayerType::INPUT },{ "size",v.front() } });
	for (int i = 1; i < v.size() - 1; ++i)
		model->addLayer({ { "type", LayerType::STANDARD },{ "size", v[i]} , { "activation", ActivationFunction::SIGMOID } });
	
	model->addLayer({ { "type", LayerType::OUTPUT},{ "size", v.back()} , { "activation",ActivationFunction::LINEAR } });

	if(name_file == "")
		model->autogenerate();
	else
		model->autogenerate(name_file);

	this->topology = v;

	opt->setNeuralNetwork(model);

	opt->setLearningRate(learningRate);
}

void QModel::saveModel(string file)
{
	model->saveFile(file);
}

QModel::~QModel()
{
	delete this->model;
	delete this->opt;
}

int QModel::predict(vector<double>& input, vector<double>& legalMoves)
{
	auto pred = model->predict(std::move(input));
	auto minmax = *(std::min_element(pred.begin(), pred.end()));

	std::for_each(pred.begin(), pred.end(), [&](double& item) { item = item - minmax;});

	for (int i = 0; i < pred.size(); i++)
		pred[i] = pred[i] * legalMoves[i];

	int maxIndex = std::distance(std::begin(pred), std::max_element(pred.begin(), pred.end()));

	return maxIndex;
}

int QModel::explore(vector<double>& legalMoves)
{
	random_device rnd_device;
	mt19937 mersenne_engine(rnd_device());
	uniform_real_distribution<double> dist(-1, 1);
	auto gen = std::bind(dist, mersenne_engine);
	vector<double>pred(topology.back());

	generate(pred.begin(), pred.end(), gen);

	std::for_each(pred.begin(), pred.end(), [&](double& item) {item = item + 1; });

	for (int i = 0; i < pred.size(); i++)
		pred[i] = pred[i] * legalMoves[i];

	int maxIndex = std::distance(std::begin(pred), std::max_element(pred.begin(), pred.end()));

	return maxIndex;
}

vector<double> QModel::forward(const vector<double>* input, const vector<double>* legalMoves)
{
	auto pred = model->predict(*input);

	for (int i = 0; i < pred.size(); i++)
		pred[i] = pred[i] * (*legalMoves)[i];

	return pred;
}

void QModel::train(std::vector<const vector<double>*>& _input, std::vector<const vector<double>*>& _output)
{
	opt->backpropagate(_input, _output);
}

