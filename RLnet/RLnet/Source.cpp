#include "neural/neuralnetwork.h"
#include "misc/functions.h"
#include "optimizer/backpropagation.h"
#include "optimizer/shakingtree.h"
#include "dataset/dataset.h"

#include <ctime>
#include <numeric>
#include <fstream>
#include <iostream>
#include <vector>
#include <limits>
#include <thread>

using namespace std;

typedef unsigned int uint;

double LEARNING_RATE = 0.0100;

int main(int argc, char* argv[])
{
	srand(uint(time(0)));
	
	NeuralNetwork n;
	size_t n_hidden_layer = 5;
	n.addLayer({ { "type", LayerType::INPUT },{ "size",2 } });
	n.addLayer({ { "type", LayerType::STANDARD },{ "size", 10} ,{ "activation",ActivationFunction::SIGMOID } });
	n.addLayer({ { "type", LayerType::STANDARD },{ "size", 100} ,{ "activation",ActivationFunction::SIGMOID } });
	n.addLayer({ { "type", LayerType::STANDARD },{ "size", 200} ,{ "activation",ActivationFunction::SIGMOID } });
	n.addLayer({ { "type", LayerType::OUTPUT},{ "size",1} ,{ "activation",ActivationFunction::SIGMOID } });

	n.autogenerate();

	Dataset data("data1000.txt");
	data.split(0.8);

	Backpropagation opt;
	opt.setBatchSize(60);
	LEARNING_RATE = 0.01;
	opt.setNeuralNetwork(&n);
	opt.setDataset(&data);

	double lr_reduce_amplitude = 0.9;
	int lr_reduce_schedule = 500;
	int n_iteration = 50000;
	int validate_every = 10;
	double mintest = 1;
	int i = 0;
	clock_t t = clock();

	while (i < n_iteration)
	{
		opt.minimize();

		if (i % validate_every == 0)
		{
			double strain = n.predictAllForScore(data, TRAIN);
			double stest = n.predictAllForScore(data);

			mintest = stest < mintest ? stest : mintest;
			cout << "it:" << i << '\t' << "    test_score:" << stest << "    train_score:" << strain << "   (best_test_score : " << mintest << ")" << endl;

			double delta_t = (clock() - t) / 1000.0;
		}

		if (i % lr_reduce_schedule == 0)
		{
			LEARNING_RATE = LEARNING_RATE * lr_reduce_amplitude;
			cout << LEARNING_RATE << endl;
		}
		i++;
	}

	return 0;
}