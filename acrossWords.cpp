// Author: Kevin Bell
// April 25, 2017

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include <time.h>
#include <ctype.h>
#include <iostream>
using namespace std;


//---------------------------------------------------------------------------------
// declares the struct for the linked lists.
struct Node {
	char board[17];
	int moveNumber;
	Node *pNext;
};
// -------------------------------------------------------------------------------------------
// displays name and program information.
void displayName()
{
	printf("Author: Kevin Bell\n");
	printf("Program : #5, AcrossWords\n");
	printf("TA : Moumita, T 10:00-10:50am\n");
	printf("Mar 30, 2017\n");
	printf("\n");
}// end displayName()


 //--------------------------------------------------------------------------------------------
 // displays instructions of the program for the user.
void displayInstructions()
{
	printf("Welcome to the AcrossWord puzzle, where you play against the clock\n");
	printf("to make four board rotations to restore the board back to containing\n");
	printf("four words. Each rotation can be a row letter, or column letter,\n");
	printf("followed by the number 1..3 which is how far to rotate.\n\n");
	printf("When prompted to provide input you may enter :\n");
	printf("Enter 'r' to reset the board to user - defined values.\n");
	printf("Enter 'u' to undo a move.\n");
	printf("Enter 'o' for original board to be shown");
	printf("Enter 'x' to exit the program.\n\n");
}// end displayInstructions()


//--------------------------------------------------------------------------------
// displays the list pointer position
void displayList(Node *pHead)
{
	cout << "   List: ";
	while (pHead != NULL) {
		cout << pHead->moveNumber;
		if (pHead->pNext != NULL)  cout << "->";
		pHead = pHead->pNext;
	}
	cout << endl;
}// end displayList()


//--------------------------------------------------------------------------------
// adds a node to the linked list to keep track of board history
void addNodeToList(Node *&pHead, char board[], int moveNumber)
{
	Node *pTemp = new Node;
	strcpy(pTemp->board, board);
	pTemp->moveNumber = moveNumber;
	pTemp->pNext = pHead;

	pHead = pTemp;
}// end addNodeToList


//--------------------------------------------------------------------------------
// displays both the board and the linked list.
void displayBoardAndList(char board[], Node *pHead)
{
	printf("\n    E F G H\n");
	printf("    -------\n");
	for (int i = 0; i < 16; i++)
	{
		if (i == 0) { printf(" A: "); }
		if (i == 4) { printf(" B: "); }
		if (i == 8) { printf(" C: "); }
		if (i == 12) { printf(" D: "); }
		printf("%c ", board[i]);
		if (i == 3 || i == 7 || i == 11 || i == 15)
		{
			printf("\n");
		}
	}
	displayList(pHead);
	cout << endl;
}// end displayBoardAndList()


//--------------------------------------------------------------------------------
// function to delete the node from the list to undo a move on the board.
void deleteNodeFromList(Node *&pHead, char board[], int &moveNumber) 
{
	if (pHead->pNext == NULL) {
		cout << "*** You cannot undo past the beginning of the game.  Please retry."
			<< "\n";
		return;
	}

	cout << "* Undoing move * \n";
	Node *pTemp = pHead;

	pHead = pHead->pNext;
	delete pTemp;

	strcpy(board, pHead->board);
	moveNumber = pHead->moveNumber;
	displayBoardAndList(board, pHead);
}//end deleteNodeFromList()




 //--------------------------------------------------------------------------------------------
 // reads the dictionary depending on the user's input.
void readDictionaryAndGenerateBoard(char ** &dictionary, int &numOfWords, char board[], char unveilBoard[])
{
	FILE *dictionaryFile;                             // declares the file pointer.
	int i;
	char dictionaryWord[81];                          // temporary array to be used for reading line for line from the dictionary file.

													  // if statement depending on the user's input to decide what dictionary to use.
	dictionaryFile = fopen("smallDictionary.txt", "r"); 

	// if statement that exits the program if the file isn't found.
	if (dictionaryFile == NULL)
	{
		printf("File not found.  Exiting...\n");
		exit(0);
	}

	// while loop to read word for word of the dictionary file to count the number of four letter words.
	while (fscanf(dictionaryFile, "%s", dictionaryWord) != EOF)
	{
		if (strlen(dictionaryWord) == 4)
		{
			numOfWords++;
		}
	}

	rewind(dictionaryFile);  // rewinds the file to be used again.

	dictionary = (char**)malloc(numOfWords * sizeof(char*)); // call to malloc the dictionary

															 // for loop to malloc the dictionary again to malloc each four letter word + the null character.
	for (i = 0; i < numOfWords && fscanf(dictionaryFile, "%s", dictionaryWord) != EOF;)
	{
		if (strlen(dictionaryWord) == 4)
		{
			dictionary[i] = (char*)malloc(5 * sizeof(char));
			strcpy(dictionary[i], dictionaryWord);
			++i;
		}
	}
	printf("There are %d four letter words.\n", numOfWords);


	// ------------------------------------------------------------------------------------------
	// randomly generates a board for the user to play.

	int randomIndex;
	srand((int)time(NULL));
	for (int i = 0; i < 16; i = i + 4)
	{
		randomIndex = ((rand() % numOfWords));
		board[i] = dictionary[randomIndex][0];
		unveilBoard[i] = dictionary[randomIndex][0];
		board[i + 1] = dictionary[randomIndex][1];
		unveilBoard[i + 1] = dictionary[randomIndex][1];
		board[i + 2] = dictionary[randomIndex][2];
		unveilBoard[i + 2] = dictionary[randomIndex][2];
		board[i + 3] = dictionary[randomIndex][3];
		unveilBoard[i + 3] = dictionary[randomIndex][3];
	}
	board[16] = '\0';
	unveilBoard[16] = '\0';
}// end readDictionaryAndGenerateBoard()


 //-----------------------------------------------------------------------------------------------
 // displays the board for the user.
void displayBoard(char board[])
{
	printf("\n    E F G H\n");
	printf("    -------\n");
	for (int i = 0; i < 16; i++)
	{
		if (i == 0) { printf(" A: "); }
		if (i == 4) { printf(" B: "); }
		if (i == 8) { printf(" C: "); }
		if (i == 12) { printf(" D: "); }
		printf("%c ", board[i]);
		if (i == 3 || i == 7 || i == 11 || i == 15) { printf("\n"); }
	}
}// end displayBoard()


 //---------------------------------------------------------------------------------------------
 // resets the board with user input.
void boardReset(char board[], char unveilBoard[])
{
	printf("***You chose 'r' to reset the board.***\n");
	char newBoard[17];
	printf("Please enter 16 characters to change your board: ");
	scanf("%16s", newBoard);
	printf("\n");
	while (strlen(newBoard) < 16 || strlen(newBoard) > 16)
	{
		if (strlen(newBoard) != 16)
		{
			printf("Please enter 16 characters to change your board: ");
			scanf("%s", newBoard);
			newBoard[16] = '\0';
			printf("\n");
		}
	}
	newBoard[16] = '\0';
	printf("Your new board is:\n");
	for (int i = 0; i < 16; i++)
	{
		board[i] = newBoard[i];
	}
	board[16] = '\0';
	displayBoard(board);
}// end boardReset()


 //------------------------------------------------------------------------------------------------
 // displays the original board.
void displayUnveilBoard(char board[], char unveilBoard[])
{
	printf("You chose 'u' as an option.\n");

	printf("\n    E F G H\n");
	printf("    -------\n");
	for (int i = 0; i < 16; i++)
	{
		if (i == 0) { printf(" A: "); }
		if (i == 4) { printf(" B: "); }
		if (i == 8) { printf(" C: "); }
		if (i == 12) { printf(" D: "); }
		printf("%c ", unveilBoard[i]);
		if (i == 3 || i == 7 || i == 11 || i == 15) { printf("\n"); }
	}
	printf("\n");
	printf("Your board is:\n");
	displayBoard(board);
}// end displayUnveilBoard()


 //------------------------------------------------------------------------------------------------
 // function to rotate the given row or column from the user's input.
char rotate(char board[], char rowOrColumn, int distance)
{
	char temp;
	rowOrColumn = toupper(rowOrColumn);
	if (rowOrColumn == 'A')
	{
		for (int i = 0; i < distance; i++)
		{
			temp = board[3];
			board[3] = board[2];
			board[2] = board[1];
			board[1] = board[0];
			board[0] = temp;
		}
	}
	if (rowOrColumn == 'B')
	{
		for (int i = 0; i < distance; i++)
		{
			temp = board[7];
			board[7] = board[6];
			board[6] = board[5];
			board[5] = board[4];
			board[4] = temp;
		}
	}
	if (rowOrColumn == 'C')
	{
		for (int i = 0; i < distance; i++)
		{
			temp = board[11];
			board[11] = board[10];
			board[10] = board[9];
			board[9] = board[8];
			board[8] = temp;
		}
	}
	if (rowOrColumn == 'D')
	{
		for (int i = 0; i < distance; i++)
		{
			temp = board[15];
			board[15] = board[14];
			board[14] = board[13];
			board[13] = board[12];
			board[12] = temp;
		}
	}
	if (rowOrColumn == 'E')
	{
		for (int i = 0; i < distance; i++)
		{
			temp = board[12];
			board[12] = board[8];
			board[8] = board[4];
			board[4] = board[0];
			board[0] = temp;
		}
	}
	if (rowOrColumn == 'F')
	{
		for (int i = 0; i < distance; i++)
		{
			temp = board[7];
			board[7] = board[6];
			board[6] = board[5];
			board[5] = board[4];
			board[4] = temp;
		}
	}
	if (rowOrColumn == 'G')
	{
		for (int i = 0; i < distance; i++)
		{
			temp = board[14];
			board[14] = board[10];
			board[10] = board[6];
			board[6] = board[2];
			board[2] = temp;
		}
	}
	if (rowOrColumn == 'H')
	{
		for (int i = 0; i < distance; i++)
		{
			temp = board[15];
			board[15] = board[11];
			board[11] = board[7];
			board[7] = board[3];
			board[3] = temp;
		}
	}
	return 0;
}// end rotate()


 //-------------------------------------------------------------------------------------------------
 // randomly scrambles the board.
void computerRandomRotation(char board[])
{
	char randomRow1;                            // variable for random row number 1.
	char randomRow2;                            // variable for random row number 2.
	char randomColumn;                          // variable for random column.
	int randomDistance = 1;                     // variable for the random distance the board will be rotated.
	char randomArrayOfTwoRowsOneColumn[3];      // array for the rows to be randomly picked from to be rotated.
	char randomVariable1;
	char randomVariable2;
	char randomVariable3;
	int randomIndex = 0;                        // random index to be picked from the random array.


	srand((int)time(NULL));
	// randomizes row1 and row2.
	randomRow1 = (rand() % (68 - 65 + 1) + 65);
	randomRow2 = (rand() % (68 - 65 + 1) + 65);

	// checks to see if random row1 and row2 are the same.
	while (randomRow1 == randomRow2)
	{
		if (randomRow1 == randomRow2)
		{
			randomRow2 = (rand() % (68 - 65 + 1) + 65);
		}
	}

	// randomizes the column row and distance.
	randomColumn = (rand() % (72 - 69 + 1) + 69);
	randomDistance = (rand() % (3) + 1);

	// sets the random row/col variables to char.
	randomRow1 = char(randomRow1);
	randomRow2 = char(randomRow2);
	randomColumn = char(randomColumn);

	// sets each row/col variable to an index in the random array.
	randomArrayOfTwoRowsOneColumn[0] = randomRow1;
	randomArrayOfTwoRowsOneColumn[1] = randomRow2;
	randomArrayOfTwoRowsOneColumn[2] = randomColumn;

	//---------------------------------------------------------------------------------------------------
	// randomizes the index and sets a random variable.
	randomIndex = (rand() % (3));
	randomVariable1 = randomArrayOfTwoRowsOneColumn[randomIndex];
	randomIndex = (rand() % (3));
	randomVariable2 = randomArrayOfTwoRowsOneColumn[randomIndex];

	// while loop to satisfy the case if the first random variable and the second random variable are the same.
	while (randomVariable1 == randomVariable2)
	{
		if (randomVariable1 == randomVariable2)
		{
			randomIndex = (rand() % (3));
			randomVariable2 = randomArrayOfTwoRowsOneColumn[randomIndex];
		}
	}

	// sets the last random variable.
	randomIndex = (rand() % (3));
	randomVariable3 = randomArrayOfTwoRowsOneColumn[randomIndex];

	// while loop to sastisfy the case if the third variable is the same as either the first or second variables.
	while (randomVariable3 == randomVariable2 || randomVariable3 == randomVariable1)
	{
		if (randomVariable3 == randomVariable2 || randomVariable3 == randomVariable1)
		{
			randomIndex = (rand() % (3));
			randomVariable3 = randomArrayOfTwoRowsOneColumn[randomIndex];
		}
	}
	// rotates the board three times with the variables.
	rotate(board, randomVariable1, randomDistance);
	rotate(board, randomVariable2, randomDistance);
	rotate(board, randomVariable3, randomDistance);
}// end computerRandomRotation()


 //---------------------------------------------------------------------------------------------------------
 // checks for a win.
char checkWin(char board[], char ** &dictionary, int &numOfWords)
{
	// created 4 arrays and a counter to check the dictionary for a win.
	char word1[5];
	char word2[5];
	char word3[5];
	char word4[5];
	int solution = 0;

	// setting each array as a word taken from the board.
	word1[0] = board[0];
	word1[1] = board[1];
	word1[2] = board[2];
	word1[3] = board[3];

	word2[0] = board[4];
	word2[1] = board[5];
	word2[2] = board[6];
	word2[3] = board[7];


	word3[0] = board[8];
	word3[1] = board[9];
	word3[2] = board[10];
	word3[3] = board[11];


	word4[0] = board[12];
	word4[1] = board[13];
	word4[2] = board[14];
	word4[3] = board[15];

	// makes the last position in the word array a null character.
	word1[4] = '\0';
	word2[4] = '\0';
	word3[4] = '\0';
	word4[4] = '\0';

	// for loop to check each word for being a solution of the respected dictionary.
	for (int i = 0; i < numOfWords; i++)
	{
		if (strcmp(word1, dictionary[i]) == 0) { solution++; }
		if (strcmp(word2, dictionary[i]) == 0) { solution++; }
		if (strcmp(word3, dictionary[i]) == 0) { solution++; }
		if (strcmp(word4, dictionary[i]) == 0) { solution++; }
		if (solution >= 4) { return 1; } // returns a win if all four words are valid.
	}
	return 0;
}// end checkWin()


int main()
{
	int moveNumber = 1;
	char **dictionary;        // dynamically allocated 2-D array.
	char board[17];
	char unveilBoard[17];
	int userInput = 0;        // user input to be used to decide what dictionary to use.
	int numOfWords = 0;  // variable to be used to count how many words are in the chosen dictionary.
	int i = 0;
	char rowOrColumn;
	int distance = 0;

	Node *pHead = NULL;

	Node *pTemp;
	Node *pCurrent;

	displayName();            // displays the name of the goat.
	displayInstructions();    // displays the instructions of the game.

	readDictionaryAndGenerateBoard(dictionary, numOfWords, board, unveilBoard);    // reads the file chosen by the user.
	computerRandomRotation(board); // computer scrambles the board for the user to play
	addNodeToList(pHead, board, moveNumber);  // adds the initial node to the list as a header.
	displayBoardAndList(board, pHead);        // displays the board and the list.


	while (checkWin(board, dictionary, numOfWords) != true)
	{
		printf("%d. Enter the row or column to be rotated, and a number:", moveNumber);
		scanf(" %c", &rowOrColumn);           // asks for the user's input for the row or column.
		rowOrColumn = toupper(rowOrColumn);
		if (rowOrColumn == 'o' || rowOrColumn == 'O') { displayUnveilBoard(board, unveilBoard); continue; }
		if (rowOrColumn == 'x' || rowOrColumn == 'X') { printf("Exiting program......\n"); exit(0); }
		if (rowOrColumn == 'r' || rowOrColumn == 'R') { boardReset(board, unveilBoard); continue; }
		if (rowOrColumn == 'u' || rowOrColumn == 'U') { deleteNodeFromList(pHead, board, moveNumber); continue; }
		if (scanf(" %d", &distance) != 1)
		{
			continue;
		}
		rotate(board, rowOrColumn, distance);
		printf("Rotating row/col %c by %d\n", rowOrColumn, distance);
		moveNumber++;
		addNodeToList(pHead, board, moveNumber);
		displayBoardAndList(board, pHead);

	}
	printf("\nCongrats! You've won! Exiting program. . . \n");  // the user wins.
	return 0;
}
