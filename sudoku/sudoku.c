#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE 9

// Sudoku board. Modify this to test your checker!
// 0 == empty cell; 1-9 is the filled in digit.
int board[SIZE][SIZE] = {
    {1,0,0,0,0,0,0,0,0},
    {0,2,0,0,0,0,0,0,0},
    {0,0,3,0,0,0,0,0,0},
    {0,0,0,4,0,0,0,0,0},
    {0,0,0,0,5,0,0,0,0},
    {0,0,0,0,0,6,0,0,0},
    {0,0,0,0,0,0,7,0,0},
    {0,0,0,0,0,0,0,8,0},
    {0,0,0,0,0,0,0,0,9},
};

bool row_check[SIZE];
bool col_check[SIZE];
bool box_check[SIZE];

void printBoard(int board[SIZE][SIZE]) {
    for(int i = 0; i<SIZE; i++) {
        for(int j = 0; j<SIZE; j++) {
            printf("%5d", board[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Checks the given row for duplicate numbers, and updates the row_check
// value for that row appropriately. If no number is repeated in that row,
// row_check[row] will be set to true; otherwise, it will be false.
void* checkRow(void* args) {
  // make var for row index
  int row_index = *(int*) args;
  free(args);
  // make var for numbers we've seen
  bool seen[10] = {false};
  int col;
  for(col=0; col < SIZE; col++){
    int num = board[row_index][col];
    if(num != 0){
      if(seen[num] == true){
          row_check[row_index] = false;
          return NULL;
      } else {
        seen[num] = true;
      }
    }
  }
  row_check[row_index] = true;
  return NULL;
}

// Checks the given col for duplicate numbers, and updates the col_check
// value for that col appropriately. If no number is repeated in that col,
// col_check[col] will be set to true; otherwise, it will be false.
void* checkCol(void* args) {
  // make var for col index
  int col_index = *(int*) args;
  free(args);
  // make var for numbers we've seen
  bool seen[10] = {false};
  int row;
  for(row=0; row < SIZE; row++){
    int num = board[row][col_index];
    if(num != 0){
      if(seen[num] == true){
          col_check[col_index] = false;
          return NULL;
      } else {
        seen[num] = true;
      }
    }
  }
  col_check[col_index] = true;
  return NULL;
}

// Checks the given 3x3 box for duplicate numbers, and updates the box_check
// value for that box appropriately. If no number is repeated in that box,
// box_check[box] will be set to true; otherwise, it will be false.
void* checkBox(void* args) {
    return NULL;
}

// Spawn a thread to fill each cell in each result matrix.
// How many threads will you spawn?
int main() { 
    // 1. Print the board.
    printf("Board:\n");
    printBoard(board);
    
    // 2. Create pthread_t objects for our threads.
    pthread_t row_threads[SIZE]
    pthread_t col_threads[SIZE]
    pthread_t box_threads[SIZE]
    
    // 3. Create a thread for each cell of each matrix operation.
    int r, c, b;
    for(r = 0; r < SIZE; r++){
      int* row_index = malloc(sizeof(int));
      *row_index = r;
      pthread_create(&row_threads[r], NULL, checkRow, (void*)row_index);
    }
    for(c = 0; c < SIZE; c++){
      int* col_index = malloc(sizeof(int));
      *col_index = c;
      pthread_create(&col_threads[c], NULL, checkCol, (void*)col_index);
    }
    for(b = 0; b < SIZE; b++){
      int* box_index = malloc(sizeof(int));
      *box_index = b;
      pthread_create(&box_threads[b], NULL, checkBox, (void*)box_index);
    }
    
    // 4. Wait for all threads to finish.
    for(r = 0; r < SIZE; r++){
      pthread_join(row_threads[r], NULL);
    }
    for(c = 0; c < SIZE; c++){
      pthread_join(col_threads[c], NULL);
    }
    for(b = 0; b < SIZE; b++){
      pthread_join(box_threads[b], NULL);
    }

    // 5. Print the results.
    printf("Results:\n");
    bool all_rows_passed = true;
    printf("Rows:\n");
    for (int i = 0; i < SIZE; i++) {
        if (!row_check[i]) {
            printf("Row %i did not pass\n", i);
            all_rows_passed = false;
        }
    }
    if (all_rows_passed) {
        printf("All rows passed!\n");
    }
    
    bool all_cols_passed = true;
    printf("Cols:\n");
    for (int i = 0; i < SIZE; i++) {
        if (!col_check[i]) {
            printf("Col %i did not pass\n", i);
            all_cols_passed = false;
        }
    }
    if (all_cols_passed) {
        printf("All cols passed!\n");
    }
    
    bool all_boxes_passed = true;
    printf("Boxes:\n");
    for (int i = 0; i < SIZE; i++) {
        if (!box_check[i]) {
            printf("Box %i did not pass\n", i);
            all_boxes_passed = false;
        }
    }
    if (all_boxes_passed) {
        printf("All boxes passed!\n");
    }
    return 0;
}

