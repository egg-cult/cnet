#include <iostream>
#include "Loader.h"
#include "Net.h"


void xorExperiment() {
    std::cout << "Loading Data..." << std::endl;
    Loader *loader = new Loader();
    vector<vector<double>> *X = new vector<vector<double>>();
    vector<vector<double>> *Y = new vector<vector<double>>();
    loader->loadXorData(X,Y);
    std::cout << "Data Loaded." << std::endl << std::endl;

    std::cout << "Creating Net..." << std::endl;
    Net<double> *net;
    AFActivationFunction<double> *relu = new ReLU<double>();
    AFActivationFunction<double> *leakyRelu = new LeakyReLU<double>(0.1);
    vector<Layer*> *layers = new vector<Layer*>();
    layers->push_back(new Layer(2,4,leakyRelu));
    layers->push_back(new Layer(4,1,leakyRelu));

    AFLossFunction<double> *lossFunction = new AFSquareLossFunction<double>();

    net = new Net<double>(layers, lossFunction); // TODO: Explicitly declare?
    std::cout << "Net Created." << std::endl << std::endl;

    std::cout << "Training Net..." << std::endl;
    net->train(X, Y, 100);
    std::cout << "Net Trained." << std::endl << std::endl;
}


void lineExperiment() {
    std::cout << "Loading Data..." << std::endl;
    Loader *loader = new Loader();
    vector<vector<double>> *X = new vector<vector<double>>();
    vector<vector<double>> *Y = new vector<vector<double>>();
    loader->loadLineData(X,Y, 3.5, 0.0);
    std::cout << "Data Loaded." << std::endl << std::endl;

    std::cout << "Creating Net..." << std::endl;
    Net<double> *net;
    AFActivationFunction<double> *identity = new IdentityFunction<double>();
    vector<Layer*> *layers = new vector<Layer*>();
    layers->push_back(new Layer(1,2,identity));
    layers->push_back(new Layer(2,1,identity));

    AFLossFunction<double> *lossFunction = new AFSquareLossFunction<double>();

    net = new Net<double>(layers, lossFunction); // TODO: Explicitly declare?
    std::cout << "Net Created." << std::endl << std::endl;

    std::cout << "Training Net..." << std::endl;
    net->train(X, Y, 10);
    std::cout << "Net Trained." << std::endl << std::endl;
}


int main() {
    std::cout << "Hello, Beginning Program!" << std::endl;
    int experimentCode = 0;
    switch(experimentCode) {
        case 0 : xorExperiment(); break;
        case 1: lineExperiment(); break;
    }

    return 0;

}
