#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

//  Given a 20 x 20 matrix, divide and conquer the matrix arithmetic
// using a total of 10 threads.

#define MAX 20

int matA[MAX][MAX]; 
int matB[MAX][MAX]; 

int matSumResult[MAX][MAX];
int matDiffResult[MAX][MAX]; 
int matProductResult[MAX][MAX]; 

struct cell{
    int row;
    int col;
};

void fillMatrix(int matrix[MAX][MAX]) {
    for(int i = 0; i<MAX; i++) {
        for(int j = 0; j<MAX; j++) {
            matrix[i][j] = rand()%10+1;
        }
    }
}

void printMatrix(int matrix[MAX][MAX]) {
    for(int i = 0; i<MAX; i++) {
        for(int j = 0; j<MAX; j++) {
            printf("%5d", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Fetches the appropriate coordinates from the argument, and sets
// the cell of matSumResult at the coordinates to the sum of the
// values at the coordinates of matA and matB.
void* computeSum(void* args) { // pass in the number of the ith thread
   struct cell* mycell = (struct cell*)args;
   int row = mycell -> row;
   int col = mycell -> col;
   
   int matANum = matA[row][col];
   int matBNum = matB[row][col];

   matSumResult[row][col] = matANum + matBNum;
}

// Fetches the appropriate coordinates from the argument, and sets
// the cell of matSumResult at the coordinates to the difference of the
// values at the coordinates of matA and matB.
void* computeDiff(void* args) { // pass in the number of the ith thread
    struct cell* mycell = (struct cell*)args;
   int row = mycell -> row;
   int col = mycell -> col;
   
   int matANum = matA[row][col];
   int matBNum = matB[row][col];

   matDiffResult[row][col] = matANum - matBNum;
}

// Fetches the appropriate coordinates from the argument, and sets
// the cell of matSumResult at the coordinates to the inner product
// of matA and matB.
void* computeProduct(void* args) { // pass in the number of the ith thread
  struct cell* mycell = (struct cell*)args;
   int row = mycell -> col;
   int col = mycell -> row;

   // for spot(0, 1)
   // A(0, 0)*B(0, 1) + A(0, 1)*B(1, 1) + A(0, 2)*B(2, 1)
   // spot (r, c)
   // A(r, 0) *B(0, c) + A(r, 1)*B(1, c) + A(r, 2)*B(2, c)
   
    int sum = 0;
    for(int i = 0; i<MAX; i++){
        sum += matA[row][i] * matB[i][col];
    }
   matProductResult[row][col] = sum;
}

// Spawn a thread to fill each cell in each result matrix.
// How many threads will you spawn?
int main() {
    srand(time(0));  // Do Not Remove. Just ignore and continue below.
    
    // 0. Get the matrix size from the command line and assign it to MAX
    // **Code template and directions conflicting with this instruction. MAX would have to 
    // be pre-defined for it to work in the way that the code skeleton was set up, each function signature
    // would have to change from accepting int matrix[MAX][MAX]
    // The instructions specifically ask for a 20x20 matrix.**

    // 1. Fill the matrices (matA and matB) with random values.
    fillMatrix(matA);
    fillMatrix(matB);
    
    // 2. Print the initial matrices.
    printf("Matrix A:\n");
    printMatrix(matA);
    printf("Matrix B:\n");
    printMatrix(matB);
    
    // 3. Create pthread_t objects for our threads.
    pthread_t sumThreads[MAX][MAX];
    pthread_t diffThreads[MAX][MAX];
    pthread_t productThreads[MAX][MAX];
    
    // 4. Create a thread for each cell of each matrix operation.
    // 
    // You'll need to pass in the coordinates of the cell you want the thread
    // to compute.
    // 
    // One way to do this is to malloc memory for the thread number i, populate the coordinates
    // into that space, and pass that address to the thread. The thread will use that number to calcuate 
    // its portion of the matrix. The thread will then have to free that space when it's done with what's in that memory.
    for(int i = 0; i < MAX; i++){
        for(int j = 0; j< MAX; j++){
            struct cell* myCell = (struct cell*)malloc(sizeof(struct cell));
            myCell -> row = i;
            myCell -> col = j;

            pthread_create(&sumThreads[i][j], NULL, computeSum, (void*)myCell);
            pthread_create(&diffThreads[i][j], NULL, computeDiff, (void*)myCell);
            pthread_create(&productThreads[i][j], NULL, computeProduct, (void*)myCell);
        }

    }
    // 5. Wait for all threads to finish.
    for(int i = 0; i < MAX; i++){
        for(int j = 0; j < MAX; j++){
            pthread_join(sumThreads[i][j], NULL);
            pthread_join(diffThreads[i][j], NULL);
            pthread_join(productThreads[i][j], NULL);
        }
    }
    
    // 6. Print the results.
    printf("Results:\n");
    printf("Sum:\n");
    printMatrix(matSumResult);
    printf("Difference:\n");
    printMatrix(matDiffResult);
    printf("Product:\n");
    printMatrix(matProductResult);
    return 0;
  
}