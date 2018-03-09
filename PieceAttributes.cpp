/*
 *  PieceAttributes.cpp
 *  Breakthrough
 *
 *  Created by K Donegan on 2/4/13.
 *  Copyright 2013 University of Denver. All rights reserved.
 *
 */

//#include "PieceAttributes.h"
//#include "main.cpp"

struct PieceInfo
{
	int index;
	
	int canWin;
	int canWinIn3;
	bool inDanger;
	int canCounterCapture;
	bool isBlocking;
	int mobility;
};

/*
int GetCanWin(BoardState s, int index)
{
	if(moveForward(s, index))
	{
		if(s.playerTurn == 1 && index - 8 >= 0 && index - 8 < 8)
		{
			return 1;
		}
		
		if(s.playerTurn == 2 && index + 8 >= 56 && index + 8 < 64)
		{
			return 2;
		}
	}
	
	if(moveDiagonalRight(s, index))
	{
		if(s.playerTurn == 1 && index - 7 >= 0 && index - 7 < 8)
		{
			return 1;
		}
		
		if(s.playerTurn == 2 && index + 7 >= 56 && index + 7 < 64)
		{
			return 2;
		}
	}
	
	if(moveDiagonalLeft(s, index))
	{
		if(s.playerTurn == 1 && index - 9 >= 0 && index - 9 < 8)
		{
			return 1;
		}
		
		if(s.playerTurn == 2 && index + 9 >= 56 && index + 9 < 64)
		{
			return 2;
		}
	}
	
	return 0;
}
*/

bool GetInDanger(BoardState s, int index)
{
	if(s.playerTurn == 1)
	{
		if(s.board[index - 9] == 2 || s.board[index - 7] == 2)
		{
			return true;
		}
	}
	
	if(s.playerTurn == 2)
	{
		if(s.board[index + 9] == 1 || s.board[index + 7] == 1)
		{
			return true;
		}
	}
	
	return false;
}

int GetCanCapture(BoardState s, int index)
{
	if(s.playerTurn == 1)
	{
		if(s.board[index - 9] == 2 && s.board[index - 7] == 2)
		{
			return 1;
		}
		
		if(s.board[index - 9] == 2)
		{
			return 2;
		}
		
		if(s.board[index - 7] == 2)
		{
			return 3;
		}
	}
	
	if(s.playerTurn == 2)
	{
		if(s.board[index + 9] == 1 && s.board[index + 7] == 1)
		{
			return 4;
		}
		
		if(s.board[index + 9] == 1)
		{
			return 5;
		}
		
		if(s.board[index + 7] == 1)
		{
			return 6;
		}
	}
	
	return 0;
}

int GetMobility(BoardState s, int index)
{
	int mobilityValue = 0;
	
	if(moveForward(s, index))
	{
		mobilityValue ++;
	}
	
	if(moveDiagonalLeft(s, index))
	{
		mobilityValue ++;
	}
	
	if(moveDiagonalRight(s, index))
	{
		mobilityValue ++;
	}
	
	return mobilityValue;
}

bool GetIsBlocking(BoardState s, int index)
{
	if(s.playerTurn == 1 && s.board[index - 8] == 2)
	{
		return true;
	}
	
	if(s.playerTurn == 2 && s.board[index + 8] == 1)
	{
		return true;
	}
	
	return false;
}

bool GetCanBeCounterCaptured(BoardState s, int index)
{
	if(GetCanCapture(s, index) == 1)
	{
		if(GetCanCapture(s, index - 9) > 0 && GetCanCapture(s, index - 9) <= 3)
		{
			return true;
		}
		
		if(GetCanCapture(s, index - 7) > 0 && GetCanCapture(s, index - 7) <= 3)
		{
			return true;
		}
	
	else {
		if(GetCanCapture(s, index) == 2 && GetCanCapture(s, index - 9))
		{
			return true;
		}
		
		if(GetCanCapture(s, index) == 3 && GetCanCapture(s, index - 7))
		{
			return true;
		}
	}
	}
	
	if(GetCanCapture(s, index) == 4)
	{
		if(GetCanCapture(s, index + 9) > 0 && GetCanCapture(s, index + 9) <= 3)
		{
			return true;
		}
		
		if(GetCanCapture(s, index + 7) > 0 && GetCanCapture(s, index + 7) <= 3)
		{
			return true;
		}
		
		else {
			if(GetCanCapture(s, index) == 5 && GetCanCapture(s, index + 9))
			{
				return true;
			}
			
			if(GetCanCapture(s, index) == 6 && GetCanCapture(s, index + 7))
			{
				return true;
			}
		}
	}
	
	return false;
}