#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define INPUT_SIZE 2
#define HIDDEN_SIZE 3
#define OUTPUT_SIZE 1
#define LEARNING_RATE 0.1
#define GENERATIONS 3


struct neuron
{
	double weights[3];  // the array that contains the different weights
	double b;	      // the bias of the neuron
	int size; 	      // the number of inputs 
};

struct layer
{
	struct neuron neurons[3];
};

struct network
{
	struct layer layers[3];
};


double sigmoid(double z)
{
	return 1.0/(1.0 + exp(-1 * z));
}

double sigmoid_prime(double z)
{
	return z * (1.0 - z);
}

/* fonction qui permet de ramener les input entre 0 et 1 (elle rend les données 
traitables pour les neurones)

the sigmoid function that makes the output data of a neuron compatible with
the data the next layer of neurons will need*/

void hidden_prop(double hidden_layer[], double input[],
 double hidden_weights[][HIDDEN_SIZE])
{
	for (int i = 0; i < HIDDEN_SIZE; i++) 
	{
        	hidden_layer[i] = 0.0;
        	for (int j = 0; j < INPUT_SIZE; j++)
		{
            		hidden_layer[i] += input[j] * hidden_weights[j][i];
        	}
        	hidden_layer[i] = sigmoid(hidden_layer[i]);
    	}
}
/*la fonction hidden_prop fait la transition de données entre l'input layer et
le hidden layer*/

void output_prop(double output_layer[], double hidden_layer[],
 double output_weights[][OUTPUT_SIZE])
{
    	for (int i = 0; i < OUTPUT_SIZE; i++)
	{
        	output_layer[i] = 0.0;
       	 	for (int j = 0; j < HIDDEN_SIZE; j++)
		{
            		output_layer[i] += hidden_layer[j] * output_weights[j][i];
        	}
        output_layer[i] = sigmoid(output_layer[i]);
    	}
}
//même fonction que hidden_prop mais entre les layers hidden et output

double Loss_func(double target[], double o_layer[])
{
	double error = 0.0;
	for(int i = 0 ;  i < OUTPUT_SIZE; i++)
	{
		error += 0.5 * pow(target[i] - o_layer[i], 2);
	}
	return error;
}

int main()
{
	double input[INPUT_SIZE] = {0.0,1.0};
//création de l'input layer
	double weights_h[INPUT_SIZE][HIDDEN_SIZE];
//initialisation des poids dans le layer caché
	double weights_o[HIDDEN_SIZE][OUTPUT_SIZE];
//initialisation des poids dans le dernier layer

	double h_layer[HIDDEN_SIZE];
//initiation du hidden layer
	double o_layer[OUTPUT_SIZE];
//initialisation du layer output

	double target[OUTPUT_SIZE] = {1.0};
//définition des cibles à atteindre pour le réseau (fonction Loss)


//activation du réseau

	for(int i = 0; i < GENERATIONS; i++)
	{

		hidden_prop(h_layer, input, weights_h);
//transition de données entre le layer input et la layer hidden

		output_prop(o_layer, h_layer, weights_o);
//transition de données entre le layer hidden et le layer output
	
//calcul de l'erreur

		double error = Loss_func(target, o_layer);

		printf("%f\n", error);

//début de la descente de gradients

		double output_delta[OUTPUT_SIZE];
    		double hidden_delta[HIDDEN_SIZE];

    		for (int i = 0; i < OUTPUT_SIZE; i++)
		{
        		output_delta[i] = 
			(target[i] - o_layer[i]) * 
			sigmoid_prime(o_layer[i]);
		}
        	for (int j = 0; j < HIDDEN_SIZE; j++)
		{
            		weights_o[j][i] += LEARNING_RATE *
			h_layer[j] * output_delta[i];
        	}
    	

    		for (int i = 0; i < HIDDEN_SIZE; i++)
		{
        		hidden_delta[i] = 0.0;
        		for (int j = 0; j < OUTPUT_SIZE; j++)
			{
            			hidden_delta[i] += output_delta[j] 
				* weights_o[i][j];
        		}
        		hidden_delta[i] *= sigmoid_prime(h_layer[i]);
        		for (int j = 0; j < INPUT_SIZE; j++)
			{
            			weights_h[j][i] +=
				 LEARNING_RATE
			 	* input[j] * hidden_delta[i];
        		}
    		}
	}	
}
