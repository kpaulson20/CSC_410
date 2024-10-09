// A NEUROMANCER CYBER HEIST - help needed with synchronization
// Task: Make this a fair game where the players take turns

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_PLAYERS 5
#define GAME_DURATION 10 


int currentPlayer = 0; // Index of the current player
int gameActive = 1;    // Game state
int scores[NUM_PLAYERS] = {0}; // Keep track of each player's score

pthread_mutex_t lock;
pthread_cond_t cond;

void* hack(void* arg) {
    int id = *(int*)arg;

    while (gameActive) {
        pthread_mutex_lock(&lock);

        // Players wait until its their turn to attempt to hack
        while (id != currentPlayer) {
            pthread_cond_wait(&cond, &lock);
        }

        // Simulate hacking
        printf("Player %d is attempting to hack... -------------- Current Player (%d)\n", id + 1, currentPlayer+1);
        sleep(1); 
        
        // Randomly determine success or failure
        int hackResult = rand() % 10 + 1;
        if (hackResult <= 6) { // 60% chance of success
            printf("Player %d succeeded in hacking!\n", id + 1);
            scores[id]++;
        } else {
            printf("Player %d failed to hack!\n", id + 1);
        }

        // Move to the next player
        currentPlayer = (currentPlayer + 1) % NUM_PLAYERS;

        //Communicate to other players that the current player is finished trying
        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&lock);
    }
    
    return NULL;
}

int main() {
    pthread_t players[NUM_PLAYERS];
    int playerIds[NUM_PLAYERS];
    int winner = 0;

    srand(time(NULL));
    
    // Initialize variables
    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&cond, NULL);

    // Start player threads
    for (int i = 0; i < NUM_PLAYERS; i++) {
        playerIds[i] = i;
        pthread_create(&players[i], NULL, hack, &playerIds[i]);
    }

    // Let the game run for a specified duration
    sleep(GAME_DURATION);
    gameActive = 0; // End the game

    // Tell all players to stop waiting
    pthread_cond_broadcast(&cond);

    // Join player threads
    for (int i = 0; i < NUM_PLAYERS; ++i) {
        pthread_join(players[i], NULL);
    }

    printf("--------- GAME OVER! ---------\n\n");

    int maxScore = scores[0];
    for (int i = 0; i < NUM_PLAYERS; i++){
        if(scores[i] > maxScore) {
            maxScore = scores[i];
            winner = i;
        }
    }
    printf("Player %d wins with %d points\n", winner+1, scores[winner]);

    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&cond);

    return 0;
}
