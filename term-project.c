#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define BOARD_SIZE 7

typedef struct {
    int row;
    int col;
} Coordinate;

typedef struct {
    char player;
    Coordinate position;
} Piece;

// Function prototypes
void initializeBoard(Piece board[BOARD_SIZE][BOARD_SIZE], int numPiecesP1, int numPiecesP2);
void printBoard(Piece board[BOARD_SIZE][BOARD_SIZE]);
int isValidMove(Coordinate from, Coordinate to, Piece board[BOARD_SIZE][BOARD_SIZE]);
int isGameOver(Piece board[BOARD_SIZE][BOARD_SIZE], int turnLimit, int* winner);
Coordinate getUserMove(Piece board[BOARD_SIZE][BOARD_SIZE]);
Coordinate getComputerMove(Piece board[BOARD_SIZE][BOARD_SIZE]);
void makeMove(Piece board[BOARD_SIZE][BOARD_SIZE], Coordinate from, Coordinate to);

int main() {
    Piece board[BOARD_SIZE][BOARD_SIZE];
    int numPiecesP1, numPiecesP2, turnLimit, currentPlayer = 1, turns = 0, winner;
    bool vsAI;

    // Get user input
    printf("Do you want to play against AI? (1 for yes, 0 for no): ");
    int vsAI_int;
    scanf("%d", &vsAI_int);
    vsAI = vsAI_int;



    printf("Enter the number of pieces for each player: ");
    scanf("%d", &numPiecesP1);
    numPiecesP2 = numPiecesP1; // Both players have the same number of pieces
    printf("Enter the turn limit: ");
    scanf("%d", &turnLimit);

    initializeBoard(board, numPiecesP1, numPiecesP2);

    while (!isGameOver(board, turnLimit, &winner)) {
        printBoard(board);

        Coordinate move;

        if (currentPlayer == 1 || !vsAI) {
            // User's turn
            printf("Player %d, make your move:\n", currentPlayer);
            move = getUserMove(board);
        } else {
            // AI's turn
            printf("AI's turn:\n");
            move = getComputerMove(board);
        }

        makeMove(board, move, move);

        turns++;

        if (turns >= turnLimit) {
            printf("Turn limit reached. Game over.\n");
            break;
        }

        currentPlayer = 3 - currentPlayer; // Switch player
    }

    printBoard(board);

    if (winner == 0) {
        printf("It's a draw!\n");
    } else {
        printf("Player %d wins!\n", winner);
    }

    return 0;
}

void initializeBoard(Piece board[BOARD_SIZE][BOARD_SIZE], int numPiecesP1, int numPiecesP2) {
    // Implement board initialization logic
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            board[i][j].player = ' ';
        }
    }

    // Place pieces for Player 1
    for (int i = 0; i < numPiecesP1; i++) {
        board[i][0].player = 'X';
        board[i][0].position.row = i;
        board[i][0].position.col = 0;
    }

    // Place pieces for Player 2
    for (int i = 0; i < numPiecesP2; i++) {
        board[i][BOARD_SIZE - 1].player = 'O';
        board[i][BOARD_SIZE - 1].position.row = i;
        board[i][BOARD_SIZE - 1].position.col = BOARD_SIZE - 1;
    }
}

void printBoard(Piece board[BOARD_SIZE][BOARD_SIZE]) {
    // Implement board printing logic
    printf("   1 2 3 4 5 6 7\n");
    for (int i = 0; i < BOARD_SIZE; i++) {
        printf("%c  ", 'a' + i);
        for (int j = 0; j < BOARD_SIZE; j++) {
            printf("%c ", board[i][j].player);
        }
        printf("\n");
    }
}

int isValidMove(Coordinate from, Coordinate to, Piece board[BOARD_SIZE][BOARD_SIZE]) {
    // Implement move validation logic
    if (from.row < 0 || from.row >= BOARD_SIZE || from.col < 0 || from.col >= BOARD_SIZE ||
        to.row < 0 || to.row >= BOARD_SIZE || to.col < 0 || to.col >= BOARD_SIZE) {
        return 0; // Move outside the board
    }

    if (board[from.row][from.col].player == ' ' || board[to.row][to.col].player != ' ') {
        return 0; // Invalid move
    }

    int rowDiff = abs(from.row - to.row);
    int colDiff = abs(from.col - to.col);

    return (rowDiff == 1 && colDiff == 0) || (rowDiff == 0 && colDiff == 1);
}

int isGameOver(Piece board[BOARD_SIZE][BOARD_SIZE], int turnLimit, int* winner) {
    // Implement game over logic
    // Check if any player has no valid moves or if the turn limit is reached

    int p1MoveableSpace = 0, p2MoveableSpace = 0;

    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j].player == 'X') {
                // Check available moves for Player 1
                if (i > 0 && board[i - 1][j].player == ' ') p1MoveableSpace++;
                if (i < BOARD_SIZE - 1 && board[i + 1][j].player == ' ') p1MoveableSpace++;
                if (j > 0 && board[i][j - 1].player == ' ') p1MoveableSpace++;
                if (j < BOARD_SIZE - 1 && board[i][j + 1].player == ' ') p1MoveableSpace++;
            } else if (board[i][j].player == 'O') {
                // Check available moves for Player 2
                if (i > 0 && board[i - 1][j].player == ' ') p2MoveableSpace++;
                if (i < BOARD_SIZE - 1 && board[i + 1][j].player == ' ') p2MoveableSpace++;
                if (j > 0 && board[i][j - 1].player == ' ') p2MoveableSpace++;
                if (j < BOARD_SIZE - 1 && board[i][j + 1].player == ' ') p2MoveableSpace++;
            }
        }
    }

    if (p1MoveableSpace == 0 && p2MoveableSpace == 0) {
        *winner = 0; // Draw
        return 1;
    } else if (turnLimit == 0) {
        // If you want to consider the turn limit, uncomment the lines below
        //*winner = (p1MoveableSpace > p2MoveableSpace) ? 1 : 2;
        //return 1;
    }

    return 0;
}

Coordinate getUserMove(Piece board[BOARD_SIZE][BOARD_SIZE]) {
    Coordinate move;
    char fromCol, toCol;

    printf("Choose piece to move: ");
    scanf(" %c%d", &fromCol, &move.row);
    move.row--;

    printf("Choose the new position for %c%d: ", fromCol, move.row + 1);
    scanf(" %c%d", &toCol, &move.col);
    move.col--;

    return move;
}

Coordinate getComputerMove(Piece board[BOARD_SIZE][BOARD_SIZE]) {
    Coordinate move;

    // Implement simple AI logic here
    // For example, make a random move
    do {
        move.row = rand() % BOARD_SIZE;
        move.col = rand() % BOARD_SIZE;
    } while (board[move.row][move.col].player != ' ');

    return move;
}

void makeMove(Piece board[BOARD_SIZE][BOARD_SIZE], Coordinate from, Coordinate to) {
    // Implement move execution logic
    board[to.row][to.col] = board[from.row][from.col];
    board[to.row][to.col].position = to;
    board[from.row][from.col].player = ' ';
}
