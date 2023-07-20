#ifndef NETWORK
#define NETWORK

#include "Car.h"

typedef struct{
    int num_input;
    int num_output;
    int num_hidden_nodes_1;
    //int num_hidden_nodes_2;

    double *input;
    double *output;

    double *hidden_1;
    //double *hidden_2;

    double *hidden_layer_bias_1;
    //double *hidden_layer_bias_2;
    double *output_layer_bias;

    double **hidden_layer_weight_1;
    //double **hidden_layer_weight_2;
    double **output_layer_weight;

    Car* car;
}Network;

Network* NewNetwork();
double Random_Value();
void InitNetwork(Network* n,SDL_Surface* img);
void FreeNetwork(Network* n);
void Softmax(double activation[],int len);
double Sigmoid(double activation);
void FeedForward(Network* n);
int Random_CrossOver_Value();
void CrossOver(Network* N1, Network* N2,Network* N);
void Mutation(Network* N);

#endif