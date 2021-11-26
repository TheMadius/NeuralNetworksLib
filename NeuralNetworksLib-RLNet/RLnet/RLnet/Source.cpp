#include "RLnet.h"
#include "NeuralNetwork.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

void genData(std::string filename, int count)
{
	std::ofstream file1(filename + "-in");
	std::ofstream file2(filename + "-out");

	for (int r = 0; r < count; r++) {
		Scalar x = rand() / Scalar(RAND_MAX);
		Scalar y = rand() / Scalar(RAND_MAX);

		file1 << x << ", " << y << std::endl;
		file2 << 2 * x + 10 + y << std::endl;
	}

	file1.close();
	file2.close();
}


void ReadCSV(std::string filename, std::vector<RowVector*>& data)
{
	data.clear();
	std::ifstream file(filename);
	std::string line, word;
	getline(file, line, '\n');
	std::stringstream ss(line);
	std::vector<Scalar> parsed_vec;
	while (getline(ss, word, ',')) {
		parsed_vec.push_back(Scalar(std::stof(&word[0])));
	}
	int cols = parsed_vec.size();
	data.push_back(new RowVector(cols));
	for (int i = 0; i < cols; i++) {
		data.back()->coeffRef(0, i) = parsed_vec[i];
	}

	if (file.is_open()) {
		while (getline(file, line, '\n')) {
			std::stringstream ss(line);
			data.push_back(new RowVector(1, cols));
			int i = 0;
			while (getline(ss,word, ',')) {
				data.back()->coeffRef(i) = Scalar(std::stof(&word[0]));
				i++;
			}
		}
	}
}

int main()
{
	NeuralNetwork n({ 2, 3, 1 }, 0.01);
	QModel mod();
	data in_dat, out_dat;
	genData("test", 10);

	ReadCSV("test-in", in_dat);
	ReadCSV("test-out", out_dat);

	n.train(in_dat, out_dat, 100);
	
	return 0;
}