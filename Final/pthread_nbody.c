// Sequential N-body simulation

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

#define G 6.67430e-11  // Gravitational constant 
#define NUM_BODIES 1000    // Number of bodies in the system
#define DT 60*60*24     // Time step (1 day in seconds)
#define NUM_THREADS 1

// Position, velocity, and mass of each body
typedef struct {
    double x, y;      // Position (x, y)
    double vx, vy;    // Velocity (vx, vy)
    double mass;      // Mass
} Body;

Body bodies[NUM_BODIES];

// Compute the gravitational force between two bodies
void compute_gravitational_force(Body *b1, Body *b2, double *fx, double *fy) {
    // Calculate the distance between them
    double dx = b2->x - b1->x;
    double dy = b2->y - b1->y;
    double distance = sqrt(dx * dx + dy * dy);
    
    // Distance too small - no force calculation!
    if (distance == 0.0) {
        *fx = *fy = 0.0;
        return;
    }
    
    // Gravitational force magnitude
    double force_magnitude = G * b1->mass * b2->mass / (distance * distance);
    
    // Force components 
    *fx = force_magnitude * dx / distance;
    *fy = force_magnitude * dy / distance;
}

// Update positions and velocities of the bodies
void *update_bodies_thread(void *arg) {
    int thread_id = *(int *)arg;
    int chunck_size = NUM_BODIES / NUM_THREADS;
    int start = thread_id * chunck_size;
    int end = (thread_id == NUM_THREADS - 1) ? NUM_BODIES : start + chunck_size;
    double fx, fy;
    
    for (int i = start; i < end; i++) {
        fx = 0.0;
        fy = 0.0;
        
        // Summation of all forces on that body
        for (int j = 0; j < NUM_BODIES; j++) {
            if (i != j) {
                compute_gravitational_force(&bodies[i], &bodies[j], &fx, &fy);
                // Update the velocity of body i due to the force from body j
                bodies[i].vx += fx / bodies[i].mass * DT;
                bodies[i].vy += fy / bodies[i].mass * DT;
            }
        }
    }
    return NULL;
}  
// Update the positions based on the velocities
void update(){
    for (int i = 0; i < NUM_BODIES; i++) {
        bodies[i].x += bodies[i].vx * DT;
        bodies[i].y += bodies[i].vy * DT;
    }
}

// Just printing body positions here
void print_positions() {
    for (int i = 0; i < NUM_BODIES; i++) {
        printf("Body %d: Position = (%.2f, %.2f), Velocity = (%.2f, %.2f)\n", 
               i, bodies[i].x, bodies[i].y, bodies[i].vx, bodies[i].vy);
    }
    printf("\n");
}

int main() {
    //Body bodies[NUM_BODIES];
    
    // Initializing position, velocity, and mass for each body
    for (int i = 0; i < NUM_BODIES; i++) {
        bodies[i].x = rand() % 1000000000; 
        bodies[i].y = rand() % 1000000000;  
        bodies[i].vx = (rand() % 100 - 50) * 1e3; 
        bodies[i].vy = (rand() % 100 - 50) * 1e3; 
        bodies[i].mass = (rand() % 100 + 1) * 1e24; 
    }
    pthread_t threads [NUM_THREADS];
    int thread_ids[NUM_THREADS];

    // Simulate for 1000 steps
    for (int step = 0; step < 1000; step++) {
        printf("Step %d:\n", step);
        print_positions();

        // Create threads to update bodies
        for (int t = 0; t <NUM_THREADS; t++) {
            thread_ids[t] = t;
            pthread_create(&threads[t], NULL, update_bodies_thread, &thread_ids[t]);
        }

        // Wait for threads to complete
        for (int t = 0; t < NUM_THREADS; t++) {
            pthread_join(threads[t], NULL);
        }

        update();
    }

    return 0;
}
