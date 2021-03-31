#include <iostream>
#include <iomanip>
#include <math.h>
#include <random>

using namespace std;
 
struct Figure {
	int row;
	int column;
};

struct Square {
	int row;
	int column;
};

bool UnprotectedFigure(const Figure& endangeredR, const Figure& protectorR, const Figure& myKing)
{
	if (endangeredR.row == protectorR.row || endangeredR.column == protectorR.column)
	{
		return 0;
	}
	else if (abs(endangeredR.row - myKing.row) <= 1 && abs(endangeredR.column - myKing.column) <= 1)
	{
		return 0;
	}
	else return 1;
}

bool freeToStepOn(const Square& temp, const int boardSize, const Figure& myKing, const Figure& rook1, const Figure& rook2)
{


	if (temp.row == rook1.row || temp.column == rook1.column || temp.row >= boardSize || temp.row < 0) {
		return 0;
	}

	if (temp.row == rook2.row || temp.column == rook2.column || temp.column >= boardSize || temp.column < 0) {
		return 0;
	}

	if (abs(temp.column - myKing.column) <= 1 && abs(temp.row - myKing.row) <= 1)
	{
		return 0;
	}

	return 1;
}

bool safeMove(const Figure& computersKing, const Figure& beforeK, const Figure& before1, const Figure& before2, const Figure& myKing, const Figure& R1, const Figure& R2)
{


	if (abs(computersKing.column - myKing.column) < 2 && abs(computersKing.row - myKing.row) < 2)
	{
		cout << "Invalid move." << endl;
		return 0;
	}


	if (abs(beforeK.row - myKing.row) > 1 || abs(beforeK.column - myKing.column) > 1)
	{
		cout << "Invalid move." << endl;
		return 0;
	}

	if ((before1.row != R1.row && before1.column != R1.column) || (before2.row != R2.row && before2.column != R2.column))
	{
		cout << "Invalid move." << endl;
		return 0;
	}

	if ((before1.row == R2.row) && ((R2.column<before1.column && R2.column > R1.column) || (R2.column > before1.column&& R2.column < R1.column)))
	{
		cout << "Invalid move." << endl;
		return 0;
	}

	if ((before1.column == R2.column) && ((R2.row<before1.row && R2.row > R1.row) || (R2.row > before1.row&& R2.row < R1.row)))
	{

		cout << "Invalid move." << endl;
		return 0;
	}

	if ((before2.row == R1.row) && ((R1.column<before2.column && R1.column > R2.column) || (R1.column > before2.column&& R1.column < R2.column)))
	{
		cout << "Invalid move." << endl;
		return 0;
	}

	if ((before2.column == R1.column) && ((R1.row<before2.row && R1.row > R2.row) || (R1.row > before2.row&& R1.row < R2.row)))
	{
		cout << "Invalid move." << endl;
		return 0;
	}

	return 1;

}

int generateRandomNumber(int from, int to) {
	random_device dev;
	mt19937 rng(dev());
	uniform_int_distribution<mt19937::result_type> dist6(from, to);

	return dist6(rng);
}

void randomize(Figure& something, int max)
{
	something.row = generateRandomNumber(0, max - 1);
	something.column = generateRandomNumber(0, max - 1);
}

void generateRandomFigures(int boardSize, Figure& computersKing, Figure& myKing, Figure& rook1, Figure& rook2)
{
	do {
		randomize(myKing, boardSize);
		randomize(rook1, boardSize);
		randomize(rook2, boardSize);
		randomize(computersKing, boardSize);

		do
		{
			randomize(myKing, boardSize);
			randomize(rook1, boardSize);
			randomize(rook2, boardSize);
			randomize(computersKing, boardSize);

		} while (computersKing.row == rook1.row || computersKing.row == rook2.row || computersKing.column == rook1.column || computersKing.column == rook2.column || abs(myKing.row - computersKing.row <= 1) && abs(myKing.column - computersKing.column <= 1));


	} while ((myKing.row == rook1.row && myKing.column == rook1.column) || (myKing.row == rook2.row && myKing.column == rook2.column) ||
		(myKing.row == computersKing.row && myKing.column == computersKing.column) || (rook1.column == rook2.column && rook1.row == rook2.row) ||
		(computersKing.row == rook1.row && computersKing.column == rook1.column) || (computersKing.row == rook2.row && computersKing.column == rook2.column));
}

void fillBoard(char* board[], int size, Figure compKing, Figure myKing, Figure R1, Figure R2)
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			board[i][j] = '-';
		}
	}


	if (R1.column != -1 && R1.row != -1) {

		board[R1.column][R1.row] = '1';
	}

	if (R2.column != -1 && R2.row != -1) {
		board[R2.column][R2.row] = '2';
	}

	board[myKing.column][myKing.row] = 'M';
	board[compKing.column][compKing.row] = 'C';

}

void indexRow(int BoardSize, int width)
{
	cout << endl << "    ";
	int Width = width;
	for (int j = 0; j < BoardSize; j++) {
		cout << setw(width) << j;
	}
	cout << endl << "    ";

	for (int j = 0; j < BoardSize; j++) {
		cout << setw(width) << "---";
	}
	cout << endl;


}

void coutBoard(char** board, int size)
{
	long width = 4;

	indexRow(size, width);

	for (int i = 0; i < size; i++) {
		cout << setw(3) << i << "|";// creates index column
		for (int j = 0; j < size; j++) {
			cout << setw(width) << board[j][i];
		}
		cout << endl;
	}
	cout << endl;

}

int movesCounter = 0;

void playersMove(int boardSize, Figure& computersKing, Figure& myKing, Figure& rook1, Figure& rook2)
{

	
	movesCounter++;

	Figure beforeK = myKing;
	Figure before1 = rook1;
	Figure before2 = rook2;


	char piece = 0;
	int x = -1;
	int y = -1;

	cout << "Make your move. /figure, row, column/" << endl;



	//validation:
	do {
		cin.clear();
		cin.ignore(1000, '\n');
		cout << "Enter a valid figure:  ";
		cin >> piece;

	} while (piece != 'M' && piece != 'm' && piece != '1' && piece != '2');


	do {
		cout << "Enter a valid y:  ";

		cin.clear();
		cin.ignore(1000, '\n');
		cin >> x;

	} while (x<0 || x>boardSize);

	do {
		cout << "Enter a valid x:  ";

		cin.clear();
		cin.ignore(1000, '\n');
		cin >> y;


	} while (y<0 || y>boardSize);


	switch (piece) {
	case 'M': {
		myKing.row = x;
		myKing.column = y;
		break;
	}
	case 'm': {
		myKing.row = x;
		myKing.column = y;
		break;
	}
	case '1': {
		rook1.row = x;
		rook1.column = y;
		break;
	}
	case '2': {
		rook2.row = x;
		rook2.column = y;
		break;
	}

	}



	if (safeMove(computersKing, beforeK, before1, before2, myKing, rook1, rook2) == 0)
	{

		playersMove(boardSize, computersKing, beforeK, before1, before2);
		myKing = beforeK;
		rook1 = before1;
		rook2 = before2;
	}


}

void computersMove(bool& doYouWin, int boardSize, Figure& computersKing, Figure& myKing, Figure& rook1, Figure& rook2)
{

	doYouWin = 0;
	Square UpLeft = { computersKing.row - 1 , computersKing.column - 1 };
	Square UpMid = { computersKing.row - 1 , computersKing.column };
	Square UpRight = { computersKing.row - 1 , computersKing.column + 1 };
	Square Left = { computersKing.row, computersKing.column - 1 };
	Square Right = { computersKing.row, computersKing.column + 1 };
	Square DownLeft = { computersKing.row + 1 , computersKing.column - 1 };
	Square DownMid = { computersKing.row + 1 , computersKing.column };
	Square DownRight = { computersKing.row + 1 , computersKing.column + 1 };

	if ((abs(rook1.row - computersKing.row) <= 1 && abs(rook1.column - computersKing.column) <= 1) && UnprotectedFigure(rook1, rook2, myKing))
	{
		computersKing.row = rook1.row;
		computersKing.column = rook1.column;

		rook1.row = -1;
		rook1.column = -1;

	}

	else if ((abs(rook2.row - computersKing.row) <= 1 && abs(rook2.column - computersKing.column) <= 1) && UnprotectedFigure(rook2, rook1, myKing))
	{
		computersKing.row = rook2.row;
		computersKing.column = rook2.column;
		rook2.row = -1;
		rook2.column = -1;
	}


	else if (freeToStepOn(UpLeft, boardSize, myKing, rook1, rook2))
	{
		computersKing.row = UpLeft.row;
		computersKing.column = UpLeft.column;
	}
	else if (freeToStepOn(UpMid, boardSize, myKing, rook1, rook2))
	{
		computersKing.row = UpMid.row;
		computersKing.column = UpMid.column;
	}
	else if (freeToStepOn(UpRight, boardSize, myKing, rook1, rook2))
	{
		computersKing.row = UpRight.row;
		computersKing.column = UpRight.column;
	}
	else if (freeToStepOn(Left, boardSize, myKing, rook1, rook2))
	{
		computersKing.row = Left.row;
		computersKing.column = Left.column;
	}

	else if (freeToStepOn(Right, boardSize, myKing, rook1, rook2))
	{
		computersKing.row = Right.row;
		computersKing.column = Right.column;
	}
	else if (freeToStepOn(DownLeft, boardSize, myKing, rook1, rook2))
	{
		computersKing.row = DownLeft.row;
		computersKing.column = DownLeft.column;
	}
	else if (freeToStepOn(DownMid, boardSize, myKing, rook1, rook2))
	{
		computersKing.row = DownMid.row;
		computersKing.column = DownMid.column;
	}
	else if (freeToStepOn(DownRight, boardSize, myKing, rook1, rook2))
	{
		computersKing.row = DownRight.row;
		computersKing.column = DownRight.column;
	}

	else
	{
		doYouWin = 1;
	}


}

void Game(char* board[], int boardSize, Figure computersKing, Figure myKing, Figure rook1, Figure rook2) {
	


	bool YouWin = 0;

	generateRandomFigures(boardSize, computersKing, myKing, rook1, rook2);

	fillBoard(board, boardSize, computersKing, myKing, rook1, rook2);
	coutBoard(board, boardSize);

	cout << "M - your king\nC - computers king\n1,2 - your rooks\n";

	while (1) {

		playersMove(boardSize, computersKing, myKing, rook1, rook2);
		system("cls");


		fillBoard(board, boardSize, computersKing, myKing, rook1, rook2);

		coutBoard(board, boardSize);

		char pause;
		cout << "Enter anything to continue..." << endl;
		cin >> pause;


		computersMove(YouWin, boardSize, computersKing, myKing, rook1, rook2);

		system("cls");

		fillBoard(board, boardSize, computersKing, myKing, rook1, rook2);
		coutBoard(board, boardSize);

		if (YouWin == 1) {
			system("cls");
			cout << "You won! :D"<<endl;
			cout << "And with only " << movesCounter << " moves! Congratulations!";
			break;
		}


	}


}


int main() {

	while (1) {

		

		cout << endl <<endl<< "        SIMPLE GAME OF CHESS " << endl << '\n';
		cout << "        Start a new Game/ S " << endl;
		cout << "        Change board size/ C " << endl;
		cout << "        Quit/Q" << endl;



		Figure computersKing = { 0,0 };
		Figure myKing = { 0,0 };
		Figure rook1 = { 0,0 };
		Figure rook2 = { 0,0 };


		int boardSize = 25;

		char** board = new char* [boardSize];
		for (int i = 0; i < boardSize; ++i)
		{
			board[i] = new char[boardSize];
		}

		char choice = 0;


		do {

			if (choice != 0) cout << "Invalid choice!" << endl;
			cin >> choice;


		} while (choice != 'q' && choice != 'Q' && choice != 's' && choice != 'S' && choice != 'c' && choice != 'C');


		//chooses to quit the game
		if (choice == 'Q' || choice == 'q')
		{
			cout << "bye" << endl;
			return 0;
		}
		//chooses new game
		if (choice == 's' || choice == 'S')
		{
			boardSize = 8;
			system("cls");
			Game(board, boardSize, computersKing, myKing, rook1, rook2);
		}
		//chooses to change board size
		if (choice == 'c' || choice == 'C')
		{
			cout << "Choose board size  (minimum 6x6, maximum 25x25)" << endl;


			do {

				cout << "Choose size: ";
				cin >> boardSize;

			} while (boardSize < 6 || boardSize>25);


			Game(board, boardSize, computersKing, myKing, rook1, rook2);

		}

		for (int i = 0; i < boardSize; ++i)
		{
			delete[] board[i];
		}
		delete[] board;
	}
}

