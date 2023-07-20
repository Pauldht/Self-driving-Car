#include <SDL2/SDL.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "Network.h"
#include "Car.h"

Network* NewNetwork()
{
    Network* n = malloc(sizeof(Network));
    n->num_input = 6;
    n->num_hidden_nodes_1 = 4;
    n->num_output = 4;
    return n;
}

double Random_Value()
{
    double value = (((rand()/(float)RAND_MAX)-0.5)*2);
    return value;
}

void InitNetwork(Network* n,SDL_Surface *img)
{
    assert(n != NULL);
    n->car = newCar();
    InitCar(n->car,img);

    n->num_input = 6;
    n->num_hidden_nodes_1 = 4;
    n->num_output = 4;

    n->hidden_layer_weight_1 = malloc(n->num_input*sizeof(double*));
    for(int i = 0; i< n->num_input; i++)
    {
        n->hidden_layer_weight_1[i] = malloc(sizeof(double)*n->num_hidden_nodes_1);
    }
    for(int i = 0; i < n->num_input; i++)
    {
        for(int j = 0; j< n->num_hidden_nodes_1 ; j++)
        {
            n->hidden_layer_weight_1[i][j] = Random_Value();
        }
    }

    n->output_layer_weight = malloc(n->num_hidden_nodes_1*sizeof(double*));
    for(int i = 0; i< n->num_hidden_nodes_1; i++)
    {
        n->output_layer_weight[i] = malloc(sizeof(double)*n->num_output);
    }
    for(int i = 0; i < n->num_hidden_nodes_1; i++)
    {
        for(int j = 0; j< n->num_output; j++)
        {
            n->output_layer_weight[i][j] = Random_Value();
        }
    }

    n->hidden_layer_bias_1 = malloc(sizeof(double)*n->num_hidden_nodes_1);
    for(int i = 0; i<n->num_hidden_nodes_1; i++)
    {
        n->hidden_layer_bias_1[i] = Random_Value();
    }
    
    n->output_layer_bias = malloc(sizeof(double)*n->num_output);
    for(int i = 0; i<n->num_output; i++)
    {
        n->output_layer_bias[i] = Random_Value();
    }
    
    n->input = malloc(sizeof(double)*n->num_input);
    n->hidden_1 = malloc(sizeof(double)*n->num_hidden_nodes_1);
    n->output = malloc(sizeof(double)*n->num_output);
}

void FreeNetwork(Network* n)
{
    freeCar(n->car);
    
    for(int i = 0; i< n->num_input; i++)
        free(n->hidden_layer_weight_1[i]);
    free(n->hidden_layer_weight_1);

    for(int i = 0; i< n->num_hidden_nodes_1; i++)
        free(n->output_layer_weight[i]);
    free(n->output_layer_weight);

    free(n->hidden_layer_bias_1);
    free(n->output_layer_bias);
    
    free(n->input);
    free(n->hidden_1);
    free(n->output);
}

void Softmax(double activation[], int len)
{
	double sum = 0;
    
    //get the sum of the list 
    for (int k = 0; k < len; k++)
    {
        sum += exp(activation[k]); 
    }

    //modification of the list to get proba according to softmax method
    for (int index = 0;index < len; index++)
    {
        activation[index] = exp(activation[index])/sum; 
    }

}

double Sigmoid(double activation)
{
	return ((double)1/(double)(1+exp(-activation)));
}

void FeedForward(Network* n)
{
    double activation;

    for(int i = 0; i< n->num_hidden_nodes_1; i++)
    {
        activation = 0;
        for(int j = 0; j < n->num_input; j++)
        {
            activation += n->input[j]*n->hidden_layer_weight_1[j][i];
        }
        n->hidden_1[i] = Sigmoid(activation);
    }

    for(int i = 0; i< n->num_output; i++)
    {
        activation = 0;
        for(int j = 0; j < n->num_hidden_nodes_1; j++)
        {
            activation += n->hidden_1[j]*n->output_layer_weight[j][i];
        }
        n->output[i] = Sigmoid(activation);
    }
}

int Random_CrossOver_Value(Network *n)
{
    int luck = (n->num_input*n->num_hidden_nodes_1+n->num_hidden_nodes_1*n->num_output)/2;
    int proba = rand()%(luck);
    if(proba == (luck-1))
    {
        return 1;
    }
    else
        return 0;
}

void CrossOver(Network* N1, Network* N2,Network* N)
{

    for(int i = 0; i < N->num_input; i++)
    {
        for(int j = 0; j< N->num_hidden_nodes_1 ; j++)
        {
            if(rand()%2 == 0)
                N->hidden_layer_weight_1[i][j] = N2->hidden_layer_weight_1[i][j];
            else
                N->hidden_layer_weight_1[i][j] = N1->hidden_layer_weight_1[i][j];
        }
    }

    for(int i = 0; i < N->num_hidden_nodes_1; i++)
    {
        for(int j = 0; j< N->num_output; j++)
        {
            if(rand()%2 == 0)
                N->output_layer_weight[i][j] = N2->output_layer_weight[i][j];
            else
                N->output_layer_weight[i][j] =  N1->output_layer_weight[i][j];
        }
    }

    for(int i = 0; i<N->num_hidden_nodes_1; i++)
    {
        if (rand()%2 == 0)
            N->hidden_layer_bias_1[i] = N2->hidden_layer_bias_1[i];
        else
            N->hidden_layer_bias_1[i] = N1->hidden_layer_bias_1[i];
    }   
    
    for(int i = 0; i<N->num_output; i++)
    {
        if(rand()%2 == 0)
            N->output_layer_bias[i] = N2->output_layer_bias[i];
        else
            N->output_layer_bias[i] = N1->output_layer_bias[i];
    }
}

void Mutation(Network* N)
{
    for(int i = 0; i < N->num_input; i++)
    {
        for(int j = 0; j< N->num_hidden_nodes_1 ; j++)
        {
            int x = Random_CrossOver_Value(N);
            if(x == 1)
                N->hidden_layer_weight_1[i][j] += Random_Value();
        }
    }

    for(int i = 0; i < N->num_hidden_nodes_1; i++)
    {
        for(int j = 0; j< N->num_output; j++)
        {
            int x = Random_CrossOver_Value(N);
            if(x == 1)
                N->output_layer_weight[i][j] += Random_Value();
        }
    }

    for(int i = 0; i<N->num_hidden_nodes_1; i++)
    {
        int x = Random_CrossOver_Value(N);
            if(x == 1)
                N->hidden_layer_bias_1[i] += Random_Value();
    }

    for(int i = 0; i<N->num_output; i++)
    {
        int x = Random_CrossOver_Value(N);
            if(x == 1)
                N->output_layer_bias[i] += Random_Value();
    }
}