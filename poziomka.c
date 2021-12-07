#include <stdio.h>

#ifndef COLUMNS
#define COLUMNS 26
#endif

#ifndef ROWS
#define ROWS 26
#endif

#ifndef SQUARES
#define SQUARES 5
#endif

#ifndef CHOICE
#define CHOICE 1000
#endif

#define true 1

#define false 0

typedef int board_type[ROWS][COLUMNS];

void emptyBoard(board_type board);
void emptyArray(char *array, int n);
void getMove(char *last_move, int *column_of_move, int *row_of_move);
void updateBoardLeft(board_type board, int column_of_move, int row_of_move);
void updateBoardRight(board_type board, int column_of_move, int row_of_move);
void copyBoard(board_type board, board_type temp_board);
void checkBoard(board_type board, board_type temp_board, char *best_moves_Columns, char *best_moves_Rows, int *max_rating, int *n);
int rateBoard(board_type board, board_type temp_board, /*char *best_moves_Columns, char *best_moves_Rows,*/ int row, int column);
int checkMovesLeft(board_type temp_board);
int checkMovesRight(board_type temp_board);

/* empties (fills with zeros) a two dimensional array ("board") */
void emptyBoard(board_type board) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            board[i][j] = 0;
        }
    }
}

/* empties (fills with zeros) a one dimensional array */
void emptyArray(char *array, int n) {
    for (int i = 0; i < n; i++) {
        array[i] = 0;
    }
}

/* adds left player's move to a board */
void updateBoardLeft(board_type board, int column_of_move, int row_of_move) {
    for(int i = 0; i < SQUARES; i++) {
        board[row_of_move + i][column_of_move] = 1;
    }
}

/* adds right player's move to a board */
void updateBoardRight(board_type board, int column_of_move, int row_of_move) {
    for(int i = 0; i < SQUARES; i++) {
        board[row_of_move][column_of_move + i] = 1;
    }
}

/* duplicates a board to a temporary board before simulating possible moves */
void copyBoard(board_type board, board_type temp_board) {
    for(int i = 0; i < ROWS; i++) {
        for(int j = 0; j < COLUMNS; j++) {
            temp_board[i][j] = board[i][j];
        }
    }
}

/* adds left player's move to a board */
int rateBoard(board_type board, board_type temp_board, /*char *best_moves_Columns, char *best_moves_Rows,*/ int row, int column) {
    copyBoard(board, temp_board);
    updateBoardRight(temp_board, column, row);
    return checkMovesRight(temp_board) - checkMovesLeft(temp_board);
}

/* checks the board for all moves possible for the right player; iterates through rows and then columns and checks if 
there are enough (SQUARES) spaces to put a block in horizontally (path_length); calculates the concurring board rating of the move
if the rating is better or equal to the current best (max_rating), changes the current best and puts the coordinates into
corresponding arrays (best_moves_Columns and best_moves_Rows) then increases the iterator (n) */
void checkBoard(board_type board, board_type temp_board, char *best_moves_Columns, char *best_moves_Rows, int *max_rating, int *n) {
    int path_length, current_rating;
    for(int i = 0; i < ROWS; i++) {
        path_length = 0; // resets the counter each time the row changes
        for(int j = 0; j < COLUMNS; j++) {
            if (board[i][j]) {
                path_length = 0; // if the field is taken (1), resets the counter
            }
            else { 
                if (path_length == SQUARES - 1) { // if the counter is as long as a single block (SQUARES - 1 since we count from 0)
                    current_rating = rateBoard(board, temp_board, /*&best_moves_Rows, &best_moves_Columns,*/ i, j - SQUARES + 1);
                    if (current_rating >= *max_rating) {
                        if (current_rating > *max_rating) { // if current rating beats best, resets the iterator (n) and empties both arrays
                            *max_rating = current_rating;
                            emptyArray(best_moves_Columns, *n);
                            emptyArray(best_moves_Rows, *n);
                            *n = 0;
                        }
                        best_moves_Columns[*n] = (char) (j - SQUARES + 1 + (int) 'A'); // puts the starting column into an array that keeps columns of best moves
                        best_moves_Rows[*n] = (char) (i + (int) 'a'); // puts the current row into an array that keeps columns of best moves
                        (*n)++;
                    }
                }
                else {
                    path_length++; // if the field is empty and the path isn't equal to length of the block, increases path
                }
            }
        }
    }
    
}

/* checks all possible moves for the left player - used to determine the board rating in function rateBoard */
int checkMovesLeft(board_type temp_board) {
    int path_length;
    int possibilities = 0;
    for(int i = 0; i < COLUMNS; i++) {
        path_length = 0;
        for(int j = 0; j < ROWS; j++) {
            if (temp_board[j][i]) { // if the field is taken, resets path_length
                path_length = 0;
            }
            else {
                if (path_length == SQUARES - 1) {
                    possibilities++;
                    path_length = 0;
                }
                else {
                    path_length++;
                }
            }
        }
    }
    return possibilities;
}

/* checks all possible moves for the right player - used to determine the board rating in function rateBoard */
int checkMovesRight(board_type temp_board) {
    int path_length = 0;
    int possibilities = 0;
    for(int i = 0; i < ROWS; i++) {
        path_length = 0;
        for(int j = 0; j < COLUMNS; j++) {
            if (temp_board[i][j]) { // if the field is taken, resets path_length
                path_length = 0;
            }
            else {
                if (path_length == SQUARES - 1) {
                    possibilities++;
                    path_length = 0;
                }
                else {
                    path_length++;
                }
            }
        }
    }
    return possibilities;
}

/* takes input and changes coordinates into integers */
void getMove(char *last_move, int *column_of_move, int *row_of_move) {
    *last_move = (char) getchar();
    if (*last_move != '-' && *last_move != '.') {
        if (*last_move >= 'A' && *last_move <= 'Z') {
            *column_of_move = *last_move - 'A';
            *last_move = (char) getchar();
            *row_of_move = *last_move - 'a';
        }
        else {
            *row_of_move = *last_move - 'a';
            *last_move = (char) getchar();
            *column_of_move = *last_move - 'A';
        }
    }
    getchar(); // takes the endline
}

int main(void) {
    char last_move = 0;
    int column_of_move, row_of_move;
    char best_moves_Columns[ROWS * COLUMNS], best_moves_Rows[ROWS * COLUMNS];
    int max_rating = -1 * ROWS * COLUMNS;
    int n = 0; // iterates the best_moves_* arrays

    board_type board;
    board_type temp_board;

    emptyBoard(board);
    while (last_move != '.') {
        getMove(&last_move, &column_of_move, &row_of_move);
        if (last_move != '.') {
            if (last_move != '-') { // if left player doesn't give up their move, updates the board with their move
                updateBoardLeft(board, column_of_move, row_of_move);
            }
            checkBoard(board, temp_board, best_moves_Columns, best_moves_Rows, &max_rating, &n);
            if (n) { // if the iterator isn't equal to zero, prints right player's move
                n = CHOICE % n;
                column_of_move = best_moves_Columns[n] - 'A'; // turns the coordinate into an integer
                row_of_move = best_moves_Rows[n] - 'a';
                printf("%c%c\n", best_moves_Rows[n], best_moves_Columns[n]); // prints right player's move 
                updateBoardRight(board, column_of_move, row_of_move);
                max_rating = -1 * ROWS * COLUMNS; // resets the maximum rating and the iterator
                n = 0;
            }
            else {
                last_move = '.'; // if there are no possible moves, gives up 
                printf(".");
            }
        }
    }
    return 0;
}