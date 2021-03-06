//
// Created by Aryan on 5/4/2017.
//

#ifndef CNET_NET_H
#define CNET_NET_H

#define PRINT_DEBUG
#ifdef PRINT_DEBUG
#define PRINT_SINGLE
#ifdef PRINT_SINGLE
    #define PRINT_DETAILS
    #ifdef PRINT_DETAILS
        #define PRINT_MATRICES
        #define PRINT_LAYER_VALS
    #endif
#endif
#define PRINT_EPOCH
#endif

#define LEARNING_RATE 0.5
#include <vector>
#include "Layer.h"
#include "AFFunctions.h"
#include "AFMatrix.h"





/**
 * This is the Net class documentation!
 * This is a PlantUML diagram of what happens
 * @image ./docs/Training
 * @image Training
 * @tparam T
 */
template <typename T>
class Net {
public:
    int numLayers;
    vector<Layer*> *layers; // TODO: is this chill?
    // TODO: Free the activation and loss functions
    AFLossFunction<double> *lossFn;

    Net(vector<Layer*> *layers, AFLossFunction<double> *lossFunction) {
        this->numLayers = layers->size();
        this->lossFn = lossFunction;
        this->layers = layers;
    }

    double trainSingle(vector<T> *inputVals, vector<T> *expectedVals) {
        // Forward propogation
        (*this->layers)[0]->forwardPass(inputVals);
        for (int i = 1; i < this->numLayers; ++i) {
            (*this->layers)[i]->forwardPass((*this->layers)[i-1]->outputVals);
        }
        vector<T> *actualVals = (*this->layers)[this->numLayers-1]->outputVals;

        // Backpropogation and loss calculation
        double loss = this->lossFn->evaluate(actualVals, expectedVals);
        (*this->layers)[this->numLayers-1]->backpropagateBase(actualVals, expectedVals, this->lossFn);
        (*this->layers)[this->numLayers-1]->updateWeights(LEARNING_RATE);
        for (int i = this->numLayers-2; i >= 0; i--) {
            (*this->layers)[i]->backpropagate((*this->layers)[i+1]->deltas, (*this->layers)[i+1]->weights);
        }

//         TODO: When is the right time to update weights?
        for (int i = 1; i < this->numLayers; ++i) {
            (*this->layers)[i]->updateWeights(LEARNING_RATE);
        }

        #ifdef PRINT_DEBUG
        this->printTraining(loss, inputVals, actualVals, expectedVals);
        #endif

        return loss;
    }

    double trainEpoch(vector<vector<T>> *allInputVals, vector<vector<T>> *allExpectedVals) {
        double loss = 0;
        for (int i = 0; i < allInputVals->size(); ++i) {
            shuffleInplace(allInputVals, allExpectedVals);
            vector<T> *inputVals = &(*allInputVals)[i];
            vector<T> *expectedVals = &(*allExpectedVals)[i];
            loss += this->trainSingle(inputVals, expectedVals);
        }
        #ifdef PRINT_EPOCH
        cout << "The total loss for this epoch is " << loss << endl;
        #endif
		return loss;
    }

    double train(vector<vector<T>> *allInputVals, vector<vector<T>> *allExpectedVals, int numEpochs) {
		double ans = 0;
        for (int i = 0; i < numEpochs; ++i) {
            ans += this->trainEpoch(allInputVals, allExpectedVals);
        }
		return ans;
    }


    #ifdef PRINT_DEBUG
    void printTraining(double loss, vector<double> *inputVals, vector<double> *actualVals, vector<double> *expectedVals) {
        #ifdef PRINT_SINGLE
        cout << "Training..." << endl;
        cout << "Input: " << vectorToString<double>(*inputVals).str() << endl;
        cout << "Expected: " << vectorToString<double>(*expectedVals).str() << endl;
        cout << "Actual: " << vectorToString<double>(*actualVals).str() << endl;
        cout << "Loss: " << loss << endl;
        #ifdef PRINT_DETAILS
        #ifdef PRINT_MATRICES
        cout << "Weights: " << endl;
        for (int i = 0; i < this->numLayers; ++i) {
            cout << (*this->layers)[i]->weights->toString().str() << endl;
        }
        cout << "Weight Gradients: " << endl;
        for (int i = 0; i < this->numLayers; ++i) {
            cout << (*this->layers)[i]->weightGradient->toString().str() << endl;
        }
        #endif

        #ifdef PRINT_LAYER_VALS
        cout << "Outputs: " << endl;
        for (int i = 0; i < this->numLayers; ++i) {
            cout << vectorToString<double>(*(*this->layers)[i]->outputVals).str() << endl;
        }

        cout << "Inputs: " << endl;
        for (int i = 0; i < this->numLayers; ++i) {
            cout << vectorToString<double>(*(*this->layers)[i]->inputVals).str() << endl;
        }

        cout << "Sums: " << endl;
        for (int i = 0; i < this->numLayers; ++i) {
            cout << vectorToString<double>(*(*this->layers)[i]->sums).str() << endl;
        }
        #endif
        #endif
        cout << endl;
        #endif
    }
    #endif

};

#endif //CNET_NET_H
