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
  int i, j;
    for(i = 0; i<SIZE; i++) {
        for(j = 0; j<SIZE; j++) {
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
  for(col=0; col < 3; col++){
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
int box_index = *(int*) args;  // Get the box index
  free(args);  // Free the allocated memory for box index

  bool seen[10] = {false};  // Array to track numbers we've seen (1-9, index 0 unused)

  // Calculate the starting row and column
  int row_start = (box_index / 3) * 3;
  int col_start = (box_index % 3) * 3;

  // Traverse the 3x3 subgrid (box)
  int row, col;
  for ( row = row_start; row < row_start + 3; row++) {
      for ( col = col_start; col < col_start + 3; col++) {
          int num = board[row][col];  // Get the number at board[row][col]
          if (num != 0) {  // Only check non-empty cells
              if (seen[num] == true) {  // If we've seen this number before in the box
                  box_check[box_index] = false;  // Mark the box as invalid
                  return NULL;  // No need to continue, early exit
              } else {
                  seen[num] = true;  // Mark this number as seen
              }
          }
      }
  }

  box_check[box_index] = true;  // If no duplicates were found, mark the box as valid
  return NULL;
}

// Spawn a thread to fill each cell in each result matrix.
// How many threads will you spawn?
int main() { 
    // 1. Print the board.
    printf("Board:\n");
    printBoard(board);
    
    // 2. Create pthread_t objects for our threads.
    pthread_t row_threads[SIZE];
    pthread_t col_threads[SIZE];
    pthread_t box_threads[SIZE];
    
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
    int rc;
    for ( rc = 0; rc < SIZE; rc++) {
        if (!row_check[rc]) {
            printf("Row %i did not pass\n", rc);
            all_rows_passed = false;
        }
    }
    if (all_rows_passed) {
        printf("All rows passed!\n");
    }
    
    bool all_cols_passed = true;
    printf("Cols:\n");
    int cc;
    for ( cc = 0; cc < SIZE; cc++) {
        if (!col_check[cc]) {
            printf("Col %i did not pass\n", cc);
            all_cols_passed = false;
        }
    }
    if (all_cols_passed) {
        printf("All cols passed!\n");
    }
    
    bool all_boxes_passed = true;
    printf("Boxes:\n");
    int bc;
    for ( bc = 0; bc < SIZE; bc++) {
        if (!box_check[bc]) {
            printf("Box %i did not pass\n", bc);
            all_boxes_passed = false;
        }
    }
    if (all_boxes_passed) {
        printf("All boxes passed!\n");
    }
    return 0;
}

