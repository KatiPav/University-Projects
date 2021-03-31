#include <iostream>
#include < stdlib.h >
using namespace std;



void drawBoard (char* boardPositions[]) {


	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 11; j++) {
			if(j == 2 && i == 0) cout<<boardPositions[0][0];//first row
			if (j == 5 && i == 0) cout << boardPositions[1][0];
			if (j == 8 && i == 0) cout << boardPositions[2][0];
			if (j == 2 && i == 2) cout << boardPositions[0][1];//second row
			if (j == 5 && i == 2) cout << boardPositions[1][1];
			if (j == 8 && i == 2) cout << boardPositions[2][1];
			if (j == 2 && i == 4) cout << boardPositions[0][2];//third row
			if (j == 5 && i == 4) cout << boardPositions[1][2];
			if (j == 8 && i == 4) cout << boardPositions[2][2];
			if (j % 3 == 0 && j != 0 && j != 9) {
				cout << "|";
				continue;
			}
			if (i == 1 || i == 3) {
				cout << " ---|---|---";
				break;
			}
			cout << ' ';
		}
		cout << "\n";
	}


}

char** createEmptyBoard() {
	char** board = new char*[3];
	for (int i = 0; i < 3; i++)
	{
		board[i] = new char[3];
		for (int j = 0; j < 3; j++) {
			board[i][j] = ' ';
		}
	}
	return board;
}

void mark(bool turn, int column, int row, char* board[]) {
	if (turn == 0) {//it is computers turn
		if(board[column][row] == ' ')	board[column][row] = 'x';
	}
	if (turn == 1) {//it is players turn
		while (board[column][row] != ' ') {
			cout << "This square is already marked! Try again:\n";
			cin >> row;
			cin >> column;
		}
			board[column][row] = 'o';
	}
	system("CLS");
	drawBoard(board);
}

bool isBoardFull(char* board[]) {
	
	for (size_t i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (board[i][j] == ' ') return 0;
		}
	}
	return 1;

}

bool isBoardEmpty(char* board[]) {

	for (size_t i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (board[i][j] != ' ') return 0;
		}
	}
	return 1;

}

bool winningMove(char* board[]) {
	//check if winning move is possible

	//horizontally
	int xCount = 0;
	for (int j = 0; j < 3; j++)
	{
		int i = 0;
		for (; i < 3; i++)
		{
			xCount += board[j][i];

			//240+32 = 272//ascii for x is 120 and 32 for space
		}
		if (xCount == 272) {
			mark(0, j, 0, board);
			mark(0, j, 1, board);
			mark(0, j, 2, board);
			return 1;

		}
		else xCount = 0;

	}

	//vertically
	for (int j = 0; j < 3; j++)
	{
		int i = 0;


		for (; i < 3; i++)
		{
			xCount += board[i][j];

			//240+32 = 272//ascii for x is 120 and 32 for space
		}
		if (xCount == 272) {
			mark(0, 0, j, board);
			mark(0, 1, j, board);
			mark(0, 2, j, board);
			return 1;

		}
		else xCount = 0;

	}


	//diagonally
	if (board[0][0] + board[1][1] + board[2][2] == 272) {
		mark(0, 0, 0, board);
		mark(0, 1, 1, board);
		mark(0, 2, 2, board);
		return 1;
	}

	if (board[0][2] + board[1][1] + board[2][0] == 272) {
		mark(0, 0, 2, board);
		mark(0, 1, 1, board);
		mark(0, 2, 0, board);
		return 1;
	}


	return 0;
}

bool preventOponentFromWinning(char* board[]) {
	//check if the opponent has a winning move 
	int oCount = 0;

	//horizontally
	for (int j = 0; j < 3; j++)
	{
		int i = 0;
		for (; i < 3; i++)
		{
			oCount += board[j][i];

			//222+32 = 254//ascii for o is 111 and 32 for space
		}
		if (oCount == 254) {
			if (board[j][0] == ' ') mark(0, j, 0, board);
			if (board[j][1] == ' ')mark(0, j, 1, board);
			if (board[j][2] == ' ')mark(0, j, 2, board);
			return 1;

		}
		else oCount = 0;

	}

	//vertically
	for (int j = 0; j < 3; j++)
	{
		int i = 0;
		for (; i < 3; i++)
		{
			oCount += board[i][j];

			//222+32 = 254//ascii for o is 111 and 32 for space
		}
		if (oCount == 254) {
			if (board[0][j] == ' ') mark(0, 0, j, board);
			if (board[1][j] == ' ')mark(0, 1, j, board);
			if (board[2][j] == ' ')mark(0, 2, j, board);
			return 1;

		}
		else oCount = 0;

	}


	//diagonally
	if (board[0][0] + board[1][1] + board[2][2] == 254) {
		mark(0, 0, 0, board);
		mark(0, 1, 1, board);
		mark(0, 2, 2, board);
		return 1;
	}

	if (board[0][2] + board[1][1] + board[2][0] == 254) {
		mark(0, 0, 2, board);
		mark(0, 1, 1, board);
		mark(0, 2, 0, board);
		return 1;
	}


	return 0;
}

void computersTurn(char* board[], bool &hasComputerWon) {
	

	hasComputerWon = winningMove(board);
	if (hasComputerWon) return;
	if (preventOponentFromWinning(board)) return;

	

	//default moves

	else if (board[0][0] == ' ') {
		mark(0, 0, 0, board);
		return;
	}
	else if ((board[2][0] == 'o') ^ (board[0][2] == 'o')) {
		mark(0, 2, 2, board);
		return;
	}
	else if (board[1][1] == ' ' && board[2][2] == ' ' ) {
		mark(0, 1, 1, board);
		return;
	}
	else if (board[0][2] == ' ' && board[0][1] == ' ') {
		mark(0, 0, 2, board);
		return;
	}
	else if (board[2][0] == ' ' && board[1][0] == ' ') {
		mark(0, 2, 0, board);
		return;
	}
	else for (size_t i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++) {
			if (board[i][j] == ' ') {
				mark(0, i, j, board);
				return;
			}
		}
	}



}


void youLose(bool &continuePlaying) {
	

	cout << "Oh, you lost. Better luck next time.\n";
	cout << "Wanna try again ? y / n\n";
	char choice = ' ';
	cin >> choice;
	if (choice == 'y' || choice == 'Y') {
		continuePlaying = 1;
	}

	else if (choice == 'n' || choice == 'N') continuePlaying = 0;
	else {
		cout << "I'll take that as a no.";
		continuePlaying = 0;
	}
	
	return;

}

void even(bool &continuePlaying) {
	cout << "Well we are even. :D\n";
	cout << "Wanna try again ? y / n\n";
	char choice = ' ';
	cin >> choice;
	if (choice == 'y' || choice == 'Y') {
		continuePlaying = 1;

	}

	else if (choice == 'n' || choice == 'N') continuePlaying = 0;
	else {
		cout << "I'll take that as a no.";
		continuePlaying = 0;
	}
	return;
}

int main()
{
	bool hasComputerWon = 0;
	bool continuePlaying = 1;
	char** board = createEmptyBoard();
	int inputRow = -1;
	int inputColumn = -1;


		cout << "I go first. Write the row and the column starting from 0.\n";
		cout << "Start? /k\n";
		char choice = 0;
		cin >> choice;


			while (choice != 'k') {
				system("CLS");
				if (choice != 'k') cout << "\nwhat? press k to continue" << endl;
				cin >> choice;

			};



		while (1) {

			computersTurn(board, hasComputerWon);


			if (hasComputerWon) {

				youLose(continuePlaying);

				if (continuePlaying == 0) break;
				else {
					board = createEmptyBoard();
					hasComputerWon = 0;
					computersTurn(board, hasComputerWon);
				}
			}
			if (isBoardFull(board)) {

				even(continuePlaying);
				if (continuePlaying == 0) break;
				else {
					board = createEmptyBoard();
					hasComputerWon = 0;
					computersTurn(board, hasComputerWon);
				}

			}
			cout << "Your turn now (row column): \n";

			cin >> inputRow;
			cin >> inputColumn;
			mark(1, inputColumn, inputRow, board);

		}

		cout << "\nThe game is over.";
	
	
	for (size_t i = 0; i < 3; i++)
	{
		delete[] board[i];
	}
	delete [] board;
}


