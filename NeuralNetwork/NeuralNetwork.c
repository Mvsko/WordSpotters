#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Fonction d'activation avec sigmoid
double sigmoid(double x) {
    return 1 / (1 + exp(-x));
}

// Dérivée fonction sigmoid
double sigmoid_derivative(double x) {
    return x * (1 - x);
}

// Structure réseau de neurones
typedef struct {
    double weights_input_hidden[2][2];
    double weights_hidden_output[2];
    double hidden_layer[2];
    double output;
} NeuralNetwork;

// initialisation réseau neurones avec poids aléatoires
void init_network(NeuralNetwork* nn) {
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            nn->weights_input_hidden[i][j] = (double)rand() / RAND_MAX * 2 - 1; // Poids entre -1 et 1
        }
        nn->weights_hidden_output[i] = (double)rand() / RAND_MAX * 2 - 1;
    }
}

// fonction prédiction sortie réseau
double predict(NeuralNetwork* nn, double input[2]) {
    // Calculer la couche cachée
    for (int i = 0; i < 2; i++) {
        nn->hidden_layer[i] = sigmoid(input[0] * nn->weights_input_hidden[i][0] + input[1] * nn->weights_input_hidden[i][1]);
    }
    
    // Calculer la sortie
    nn->output = sigmoid(nn->hidden_layer[0] * nn->weights_hidden_output[0] + nn->hidden_layer[1] * nn->weights_hidden_output[1]);
    
    return nn->output;
}

// entraîner réseau de neurones
void train(NeuralNetwork* nn, double inputs[4][2], double outputs[4], int epochs, double learning_rate) {
    for (int epoch = 0; epoch < epochs; epoch++) {
        for (int i = 0; i < 4; i++) {
            // Forward pass
            double input[2] = { inputs[i][0], inputs[i][1] };
            double predicted = predict(nn, input);

            // Calculer l'erreur
            double error = outputs[i] - predicted;
            // backward pass
            // dérivée de sortie
            double d_output = error * sigmoid_derivative(predicted);

            // mise à jour poids de couche de sortie
            for (int j = 0; j < 2; j++) {
                nn->weights_hidden_output[j] += learning_rate * d_output * nn->hidden_layer[j];
            }

            // Dérivée de couche cachée
            double d_hidden[2];
            for (int j = 0; j < 2; j++) {
                d_hidden[j] = d_output * nn->weights_hidden_output[j] * sigmoid_derivative(nn->hidden_layer[j]);
            }

            // mise à jour poids de couche cachée
            for (int j = 0; j < 2; j++) {
                nn->weights_input_hidden[j][0] += learning_rate * d_hidden[j] * input[0];
                nn->weights_input_hidden[j][1] += learning_rate * d_hidden[j] * input[1];
            }
           // printf("Epoch %d: %f / %d  \n",epoch,predicted,epochs);
        }
    }
}

int main() {
    NeuralNetwork nn;
    init_network(&nn);

    // inputs fonction NXOR
    double inputs[4][2] = {
        {0, 0},
        {0, 1},
        {1, 0},
        {1, 1}
    };

    // fonction exit NXOR
    double outputs[4] = {0, 1, 1, 0}; // test of !A.!B + A.B

    // Network Trainig
    int nbtest =  10000;
    printf("Training of the NeuralNetwork with %d tests\n",nbtest);
    train(&nn, inputs, outputs, nbtest, 0.1); //Training
    printf("Training complete\n");

    // Network test
    printf("Test du réseau de neurones NXOR:\n");
    for (int i = 0; i < 4; i++) {
        double predicted = predict(&nn, inputs[i]);
        printf("Entrée: (%d, %d) -> Sortie prévue: %.5f (attendue: %.1f)\n", (int)inputs[i][0], (int)inputs[i][1], predicted, outputs[i]);
    }

    return 0;
}
