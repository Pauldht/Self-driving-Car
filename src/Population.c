#include <SDL2/SDL.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "Network.h"
#include "Population.h"
#include "Display.h"


Population* NewPopulation()
{
    Population *P = malloc(sizeof(Population));

    int nb_population = 30;

    P->Generation = malloc(sizeof(Network)*nb_population);

    return P;
}

void InitPopulation(Population* P, SDL_Surface* img)
{
    P->nb_by_generation = 30;
    //if any changes are made think about change this nb in the NewPopulation() function  
    P->nb_of_population = 0;
    P->fitness = 0.0;

    for(int i = 0; i<P->nb_by_generation; i++)
    {
        InitNetwork(&P->Generation[i],img);
    }

}

void FreePopulation(Population* P)
{
    for(int i = 0; i<P->nb_by_generation; i++)
    {
        FreeNetwork(&P->Generation[i]);
    }

    free(P->Generation);

    free(P);
}

void PrintPopulation(Population* P, int specific)
{
    Network *n = &P->Generation[specific];
    assert(n!=NULL);

    for(int i = 0; i < n->num_input; i++)
    {
        for(int j = 0; j< n->num_hidden_nodes_1 ; j++)
        {
            printf("hidden_layer_weight_1[%d][%d] = %lf\n",i,j,n->hidden_layer_weight_1[i][j]);
        }
    }

    for(int i = 0; i < n->num_hidden_nodes_1; i++)
    {
        for(int j = 0; j< n->num_output; j++)
        {
            printf("output_layer_weight[%d][%d] = %lf\n",i,j,n->output_layer_weight[i][j]);
        }
    }
    for(int i = 0; i<n->num_hidden_nodes_1; i++)
    {
        printf("hidden_layer_bias_1[%d] = %lf\n",i,n->hidden_layer_bias_1[i]);
    }

    for(int i = 0; i<n->num_output; i++)
    {
        printf("output_layer_bias[%d] = %lf\n",i,n->output_layer_bias[i]);
    }
}

void Set_Network(Network * N, Network *n)
{

    for(int i = 0; i < n->num_input; i++)
    {
        for(int j = 0; j< n->num_hidden_nodes_1 ; j++)
        {
            n->hidden_layer_weight_1[i][j] = N->hidden_layer_weight_1[i][j];
        }
    }

    for(int i = 0; i < n->num_hidden_nodes_1; i++)
    {
        for(int j = 0; j< n->num_output; j++)
        {
            n->output_layer_weight[i][j] = N->output_layer_weight[i][j];
        }
    }

    for(int i = 0; i<n->num_hidden_nodes_1; i++)
    {
        n->hidden_layer_bias_1[i] = N->hidden_layer_bias_1[i];
    }
    
    for(int i = 0; i<n->num_output; i++)
    {
        n->output_layer_bias[i] = N->output_layer_bias[i];
    }
}

Population* Change_Population(Population* P,SDL_Surface* img)
{
    int nbr = P->nb_by_generation;
    int tab[nbr];
    int index[nbr];
    int pivot;

    for(int i = 0; i<nbr; i++)
    {
        tab[i] = P->Generation[i].car->score;
        index[i] = i;
    }

    for(int i = 0; i<nbr; i++)
    {
        for(int j = 0; j<nbr; j++)
        {
            for(int y = i; y <nbr; y++)
            {
                if(tab[i] < tab[y])
                {
                    pivot = tab[i];
                    tab[i] = tab[y];
                    tab[y] = pivot;
                    pivot = index[i];
                    index[i] = index[y];
                    index[y] = pivot;
                }
            }
        }
    }

    //New Population
    Population* p = NewPopulation();
    InitPopulation(p,img);
    p->nb_of_population = P->nb_of_population + 1;
    for(int i = 0; i<30; i++)
        Set_Network(&P->Generation[index[0]],&p->Generation[i]);
    

    //CrossOver
    for(int i = 1; i<30; i++)
        CrossOver(&P->Generation[index[0]],&P->Generation[index[rand()%5 + 1]],&p->Generation[i]);

    
    //Mutation
    for(int i = 5; i<30; i++)
        Mutation(&p->Generation[i]);
    
    //free Population and Network
    FreePopulation(P);


    return p;
}

int BestScore(Population* P)
{
    int max = 0;
    for(int i = 0; i<P->nb_by_generation; i++)
    {   if(P->Generation[i].car->score > max)
            max = P->Generation[i].car->score;
    }

    return max;
}

int BestCarIndex(Population* P)
{
    int max = 0;
    int index = 0;
    for(int i = 0; i<P->nb_by_generation; i++)
    {   if(P->Generation[i].car->score > max){
            max = P->Generation[i].car->score;
            index = i;}
    }

    return index;
}