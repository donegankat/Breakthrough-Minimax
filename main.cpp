#include <iostream>
#include <vector>
#include <string>
//#include "PieceAttributes.h"
//#include "PieceAttributes.cpp"


struct BoardState
{
	int board[64];
	int playerTurn;
};

struct PieceAction
{
	int from, to;
	
	PieceAction()
	{
		from = 0;
		to = 0;
	}
	
	PieceAction(int f, int t)
	{
		from = f;
		to = t;
	}
};

struct Player
{
	bool maxPlayer;
};


BoardState GetStartState();
uint32_t GetHashFromState(BoardState s);
BoardState GetStateFromHash(uint32_t hash);
uint32_t GetMaxHashValue();
void BruteForceHashValidate();
int GetLineNum(int a);
int GetColumnNum(int a);
bool moveForward(BoardState s, int index);
bool moveDiagonalRight(BoardState s, int index);
bool moveDiagonalLeft(BoardState s, int index);
bool isCaptureRight(BoardState s, int index);
bool isCaptureLeft(BoardState s, int index);
std::vector<PieceAction> GetLegalActions1(BoardState s);
std::vector<PieceAction> GetLegalActions2(BoardState s);
std::vector<PieceAction> GetPieceActions(BoardState s, int index);
int GetNumActions1(BoardState s);
int GetNumActions2(BoardState s);
BoardState ApplyAction(BoardState s, PieceAction a);
BoardState UndoAction(BoardState s, PieceAction a);
bool isLegalAction(BoardState s, int fromIndex, int toIndex);
int GetIndex(int x, int y);
int GetColumn(int index);
int GetRow(int index);
void PrintBoard(BoardState currentBoard);
int GameOver(BoardState s);
void SwitchTurn(BoardState s);
void Play(BoardState s);

int EvalFunction(BoardState s);
void MaxPlayer(BoardState s, int depth, int worst, int best);
void MinPlayer(BoardState s, int depth, int worst, int best);
BoardState GetBoardState(BoardState s);
int BasicEval(BoardState s, int index);
int EvalPiece(BoardState s, int index);
int EvalMinPiece(BoardState s, int index);
bool GetWin(BoardState s, int index);
bool GetLoss(BoardState s, int index);
int GetNumPieces1(BoardState s);
int GetNumPieces2(BoardState s);

void Minimax_MaxPlayer(BoardState s, int currentDepth);//, int depth);
void Minimax_MinPlayer(BoardState s, int currentDepth);//, int depth);
//PieceAction Minimax_MinMove(BoardState s, int myDepth);
void Minimax_MinMove(BoardState s, int myDepth);

PieceAction Minimax_MaxMove(BoardState s, int myDepth);
void Minimax();

void HumanTurn(BoardState s, int currentDepth);
int ConvertChar(char x);




static int bestValue = 0;
static int depth = -1;
static int worstValue = 10000;


int depthInc()
{
//	static int depth = 0;
	return depth ++;
}

int SetBestValue(int val)
{
//	static int best = val;
	bestValue = val;
	return bestValue;
}


int SetWorstValue(int val)
{
//	static int worst = val;
	worstValue = val;
	return worstValue;
}

void HumanTurn(BoardState s, int currentDepth)
{
	s.playerTurn = 1;
	char x, toX;
	int y, toY;
	int index, toIndex;
	
	std::cout << "Player " << s.playerTurn << std::endl;
	std::cout << "Enter the coordinates of the piece you want to move:" << std::endl;
	std::cin >> x >> y;
	
	int xLoc = ConvertChar(x);
	index = GetIndex(xLoc, y - 1);
	
	std::cout << "Index: " << index << std::endl;
	
	if(s.board[index] != s.playerTurn)
	{
		std::cout << "You don't have a piece there." << std::endl;
		return;
	}
	
	std::vector<PieceAction> moves = GetPieceActions(s, index);
	
	if(moves.size() == 0)
	{
		std::cout << "That piece has no legal moves." << std::endl;
		return;
	}
	
	std::cout << "The piece at: " << x << " " << y << " can move to: " << std::endl;
	
	for (int i = 0; i < moves.size(); i++)//std::vector<PieceAction>::size_type i = 0; i < moves.size(); ++i)
	{
		//char moveFromColumn = 'A' + GetColumn(moves[i].from);
		//int moveFromRow = GetRow(moves[i].from);
		char moveToColumn = 'A' + GetColumn(moves[i].to);
		int moveToRow = GetRow(moves[i].to);
		std::cout << moveToColumn << " " << moveToRow << std::endl;
	}
	
	std::cout << std::endl;
	
	std::cout << "Enter the coordinates you want to move to:" << std::endl;
	std::cin >> toX >> toY;
	
	int toXLoc = ConvertChar(toX);
	toIndex = GetIndex(toXLoc, toY - 1);
	int legalMoveNum = -1;
	
	for(int i = 0; i < moves.size(); i++)
	{
		if(moves[i].to == toIndex)
		{
			legalMoveNum = i;
			break;
		}
	}
	
	if(legalMoveNum == -1)
	{
		std::cout << "Illegal action." << std::endl;
		return;
	}
	
	s = ApplyAction(s, moves[legalMoveNum]);
	PrintBoard(s);
	
	if(GameOver(s) == 1)
	{
		std::cout << "Player 1 wins!" << std::endl;
		return;
	}
	
	if(GameOver(s) == 2)
	{
		std::cout << "Player 2 wins!" << std::endl;
		return;
	}
	
	Minimax_MinMove(s, currentDepth ++);
}


void Minimax_MaxPlayer(BoardState s, int currentDepth)
{
	s.playerTurn = 1;
	if(currentDepth < 10)
	{
	std::cout << "Starting Player 1's turn at depth " << currentDepth << std::endl;
	
	if(GameOver(s) == 1)
	{
		std::cout << "Game Over. Player 1 Wins." << std::endl;
		return;
	}
	
	std::vector<PieceAction> MaxMoves = GetLegalActions1(s);
	PieceAction BestMove;
	BoardState newBoard;
	int currentScore;
	
	for(int i = 0; i < MaxMoves.size(); i++)
	{
		s.playerTurn = 1;
		currentScore = EvalPiece(s, MaxMoves[i].to);
		
		std::cout << "Current Max Eval Score: " << EvalPiece(s, MaxMoves[i].to) << std::endl;
		
		if(currentScore > bestValue)
		{
			bestValue = currentScore;//SetBestValue(currentScore);
			BestMove = PieceAction(MaxMoves[i].from, MaxMoves[i].to);
			
			newBoard = ApplyAction(s, BestMove);

			PrintBoard(newBoard);
			
			Minimax_MinPlayer(newBoard, currentDepth ++);
			std::cout << "New Maximum Value: " << bestValue << std::endl;
		}
		
//		BoardState next, previous;
//		currentDepth ++;
		
	/*	if(currentDepth >= 5)
		{
			std::cout << "Reached depth 5." << std::endl;
			previous = UndoAction(s, BestMove);
			PrintBoard(previous);
			return;
//			Minimax_MinPlayer(previous, currentDepth - 1);
		}
	*/	
//		else {
//		s.playerTurn = 1;
//			next = ApplyAction(s, BestMove);
//			PrintBoard(newBoard);
//			Minimax_MinPlayer(newBoard, currentDepth);
	//	}
		
	}
	}
	
	else {
		
		std::cout << "Reached depth " << currentDepth << std::endl;
//		previous = UndoAction(s, BestMove);
//		PrintBoard(previous);
		return;
	}
}

void Minimax_MinPlayer(BoardState s, int currentDepth)
{
	s.playerTurn = 2;
	if(currentDepth < 10)
	{
		
		std::cout << "Starting Player 2's turn at depth " << currentDepth << std::endl;
		
		if(GameOver(s) == 2)
		{
			std::cout << "Game Over. Player 2 Wins." << std::endl;
			return;
		}
		
		std::vector<PieceAction> MinMoves = GetLegalActions2(s);
		PieceAction WorstMove;
		BoardState newBoard;
		int currentScore;
		
		for(int i = 0; i < MinMoves.size(); i++)
		{
			s.playerTurn = 2;
			currentScore = EvalPiece(s, MinMoves[i].to);
			
			std::cout << "Current Min Eval Score: " << EvalPiece(s, MinMoves[i].to) << std::endl;
			
			if(currentScore < worstValue)
			{
				worstValue = SetWorstValue(currentScore);
				WorstMove = PieceAction(MinMoves[i].from, MinMoves[i].to);
				
				newBoard = ApplyAction(s, WorstMove);
				
				std::cout << "New Minimum Value: " << worstValue << std::endl;
				
				Minimax_MaxPlayer(newBoard, currentDepth ++);
			}
			
		}
	}
	else {
		std::cout << "Reached depth " << currentDepth << std::endl;
		return;
	}
	
}

static PieceAction best_move, worst_move;

PieceAction EvalGame(BoardState s, PieceAction MinMaxMove)
{
//	if(s.playerTurn == 1)
//	{
		return PieceAction(s.board[MinMaxMove.from], s.board[MinMaxMove.to]);
//	}

	
	//return PieceAction(EvalPiece(s, minmax.from), EvalPiece(s, minmax.to));
}

//PieceAction Minimax(BoardState s)
void Minimax()
{
	BoardState theGame = GetStartState();
	Minimax_MaxMove(theGame, 0);
}

//{
//	return Minimax_MaxMove(s, 0);
//}

PieceAction Minimax_MaxMove(BoardState s, int myDepth)
{
	if(GameOver(s) == 1 || myDepth >= 5)
	{
		return EvalGame(s, best_move);
	}
	
	best_move = PieceAction();
	PieceAction move, temp;
	std::vector<PieceAction> Max = GetLegalActions1(s);
	
	for(int i = 0; i < Max.size(); i++)
	{
	//	myDepth ++;
		temp = PieceAction(Max[i].from, Max[i].to);
//		move = Minimax_MinMove(ApplyAction(s, temp), myDepth ++);
		
		if(EvalPiece(s, move.to) > EvalPiece(s, best_move.to))
		{
			best_move = move;
		}
			
	}
	
	return best_move;
}

/*
 PieceAction Minimax_MinMove(BoardState s, int myDepth)
{
	s.playerTurn = 2;
	
	if(GameOver(s) == 2 || myDepth >= 5)
	{
		return EvalGame(s, worst_move);
	}
	
	worst_move = PieceAction();
	PieceAction move, temp;
	std::vector<PieceAction> Min = GetLegalActions2(s);
	
	for(int i = 0; i < Min.size(); i++)
	{
		temp = PieceAction(Min[i].from, Min[i].to);
		move = Minimax_MaxMove(ApplyAction(s, temp), myDepth ++);
		
		if(EvalPiece(s, move.to) < EvalPiece(s, worst_move.to))
		{
			worst_move = move;
		}
		
	}
	
	return worst_move;
}
*/

void Minimax_MinMove(BoardState s, int myDepth)
{
	s.playerTurn = 2;
	
	if(GameOver(s) == 2 || myDepth >= 5)
	{
		return;
	}
	
	worst_move = PieceAction();
	PieceAction move, temp;
	BoardState newBoard;
	std::vector<PieceAction> Min = GetLegalActions2(s);
	
	for(int i = 0; i < Min.size(); i++)
	{
		temp = PieceAction(Min[i].from, Min[i].to);
		//move = Minimax_MinMove(ApplyAction(s, temp), myDepth ++);
		newBoard = ApplyAction(s, temp);
		Minimax_MinMove(newBoard, myDepth ++);
		
		if(EvalPiece(s, move.to) < EvalPiece(s, worst_move.to))
		{
			worst_move = move;
		}
		
	}
	
//	return worst_move;
}


int EvalPiece(BoardState s, int index)
{
	int score = 0;
//	int max = 0;
	
	for(int i = 0; i < 64; i++)
	{
		if(s.board[i] == 0)
		{
			continue;
		}
		
		if(s.board[i] == 1 && s.playerTurn == 2)
		{
			continue;
		}
		
		if(s.board[i] == 2 && s.playerTurn == 1)
		{
			continue;
		}
	
		if(GetWin(s, i))
		{
			score += 1000;
		}
		
		if(GetLoss(s, i))
		{
			score -= 1000;
		}
		
		//	score += ((GetNumPieces1(s) - GetNumPieces2(s)) * 2);
		//	score += ((GetNumActions1(s) - GetNumActions2(s)) * 2);
		
/*		if(index == 40 || index == 48)
		{
			score += 20;
		}
		
		if(index == 16 || index == 8)
		{
			score -= 20;
		}
 */
	//	score += GetLineNum(index);
		score += GetColumnNum(index);
//		if(score > max)
//		{
//			max = score;
//		}
	}
	
	return score;
}

int EvalMinPiece(BoardState s, int index)
{
	int score = 0;
	int min = 0;
	
	for(int i = 0; i < 64; i++)
	{
		if(s.board[i] == 0)
		{
			continue;
		}
		
		if(GetWin(s, i))
		{
			score -= 1000;
		}
		
		if(GetLoss(s, i))
		{
			score += 1000;
		}
		
		//	score += ((GetNumPieces1(s) - GetNumPieces2(s)) * 2);
		//	score += ((GetNumActions1(s) - GetNumActions2(s)) * 2);
		
		/*		if(index == 40 || index == 48)
		 {
		 score += 20;
		 }
		 
		 if(index == 16 || index == 8)
		 {
		 score -= 20;
		 }
		 */
		score -= GetLineNum(index);
		score -= GetColumnNum(index);
		if(score < min)
		{
			min = score;
		}
	}
	
	return min;
	

}
/*	if(GetWin(s, index) == 1)
	{
		score += 1000;
	}
	
	if(GetWin(s, index) == 2)
	{
		score -= 1000;
	}
	
//	score += ((GetNumPieces1(s) - GetNumPieces2(s)) * 2);
//	score += ((GetNumActions1(s) - GetNumActions2(s)) * 2);
	
	if(index == 40)
	{
		score += 20;
	}

	if(index == 16)
	{
		score -= 20;
	}
	
	std::cout << "Evaluation Score: " << score << std::endl;
	
/*	if(score > bestValue)
	{
		bestValue = score;
	}
	
	if(score < worstValue)
	{
		worstValue = score;
	}
*/	
//	return score;
	
	
/*	
 if(GetCanCapture(s, index))
	{
		if(!GetCanBeCounterCaptured(s, index))
		{
			score += 75;
		}
		
		else {
			score += 50;
		}
	}
 */

bool GetWin(BoardState s, int index)
{
	if(moveForward(s, index))
	{
		if(s.playerTurn == 1 && index - 8 >= 0 && index - 8 < 8)
		{
			return 1;
		}
		
//		if(s.playerTurn == 2 && index + 8 >= 56 && index + 8 < 64)
//		{
//			return 2;
//		}
	}
	
	if(moveDiagonalRight(s, index))
	{
		if(s.playerTurn == 1 && index - 7 >= 0 && index - 7 < 8)
		{
			return true;
		}
		
//		if(s.playerTurn == 2 && index + 7 >= 56 && index + 7 < 64)
//		{
//			return 2;
//		}
	}
	
	if(moveDiagonalLeft(s, index))
	{
		if(s.playerTurn == 1 && index - 9 >= 0 && index - 9 < 8)
		{
			return true;
		}
		
//		if(s.playerTurn == 2 && index + 9 >= 56 && index + 9 < 64)
//		{
//			return 2;
//		}
	}
	
	return 0;
}

bool GetLoss(BoardState s, int index)
{
	if(moveForward(s, index))
	{
		if(s.playerTurn == 2 && index + 8 >= 56 && index + 8 < 64)
		{
			return true;
		}
	}
	
	if(moveDiagonalRight(s, index))
	{
		
		if(s.playerTurn == 2 && index + 7 >= 56 && index + 7 < 64)
		{
			return true;
		}
	}
	
	if(moveDiagonalLeft(s, index))
	{
		
		if(s.playerTurn == 2 && index + 9 >= 56 && index + 9 < 64)
		{
			return true;
		}
	}
	
	return false;
	
}


BoardState GetStartState()
{
	BoardState StartState;
	StartState.playerTurn = 1;
	
	for(int i = 0; i < 64; i++)
	{
		StartState.board[i] = 0;
	}
	
	for(int i = 0; i < 16; i++)
	{
		StartState.board[i] = 2;
	}
	
	for(int i = 48; i < 64; i++)
	{
		StartState.board[i] = 1;
	}
	
	return StartState;
}

BoardState GetBoardState(BoardState s)
{
	BoardState theState;
	
	for(int i = 0; i < 64; i++)
	{
		theState.board[i] = s.board[i];
	}
	
	return theState;
}

uint32_t GetHashFromState(BoardState s)
{
	uint32_t myHash = 0;
	int currentBit = 0;
	
	for (int i = 0; i < 64; i++)
	{
		if (s.board[i] == s.playerTurn)
		{
			//Vertical bar is 'logical OR'
			myHash |= 1 << currentBit;
		}
		currentBit++;
	}	
	
	return myHash;
}

BoardState GetStateFromHash(uint32_t hash)
{
	BoardState currentState;
	
/*	int bit_pos = 0;
	for(int i = 0; i < 64; i++)
	{
		if ((hash>>bit_pos)&1) {
			currentState.board[i] = 1;
		} else {
			currentState.board[i] = 0;
		}
		bit_pos++;
	}
	else {
		currentState.board[i] = 1;
	}
*/	
	return currentState;
}

uint32_t GetMaxHashValue()
{
	//5x6 board
	//return 0x3FFFFFFF;
	
	//7x8 board
	//return 0xFFFFFFFF;
	
	//8x8 board
	return 0xFFFFFFFF;
}

void BruteForceHashValidate()
{
	for (uint32_t h = 0; h < GetMaxHashValue(); ++h)
		if (h != GetHashFromState(GetStateFromHash(h)))
			std::cout << "Problem with hash " << h << std::endl;
}

int GetLineNum(int a)
{
	
	return a / 8;
	
}

int GetColumnNum(int a)
{
	
	return (a % 8);
	
}

bool moveForward(BoardState s, int index)
{
	if(s.playerTurn == 1 && index - 8 >= 0 && s.board[index] == 1 && s.board[index - 8] == 0 && GetColumnNum(index) == GetColumnNum(index - 8))
	{
		return true;
	}
	
	if(s.playerTurn == 2 && index + 8 < 64 && GetColumnNum(index) == GetColumnNum(index + 8) && s.board[index] == 2 && s.board[index + 8] == 0)
	{
		return true;
	}
	
	return false;
}

bool moveDiagonalRight(BoardState s, int index)
{
	if(s.playerTurn == 1 && index - 7 >= 0 && s.board[index] == 1 && s.board[index - 7] != 1 && GetColumnNum(index) == GetColumnNum(index - 7) - 1 && GetLineNum(index - 7) >= 0  && GetLineNum(index) > GetLineNum(index - 7))
	{
		return true;
	}
	
	if(s.playerTurn == 2 && index + 7 < 64  && s.board[index] == 2 && s.board[index + 7] != 2 && GetColumnNum(index) == GetColumnNum(index + 7) + 1 && GetLineNum(index + 7) < 8  && GetLineNum(index) < GetLineNum(index + 7))
	{
		return true;
	}
	
	return false;
}

bool moveDiagonalLeft(BoardState s, int index)
{
	
	if(s.playerTurn == 1 && index - 9 >= 0 && s.board[index] == 1 && s.board[index - 9] != 1 && GetColumnNum(index) == GetColumnNum(index - 9) + 1 && GetLineNum(index) > GetLineNum(index - 9))
	{
		return true;
	}
	
	if(s.playerTurn == 2 && index + 9 < 64 && s.board[index] == 2 && s.board[index + 9] != 2 && GetColumnNum(index) == GetColumnNum(index + 9) - 1 && GetLineNum(index) < GetLineNum(index + 9))
	{
		return true;
	}
	
	return false;
}

bool isCaptureRight(BoardState s, int index)
{
	if(s.playerTurn == 1 && s.board[index - 7] == 2)
	{
		return true;
	}
	
	if(s.playerTurn == 2 && s.board[index + 7] == 1)
	{
		return true;
	}
	
	return false;
}

bool isCaptureLeft(BoardState s, int index)
{
	if(s.playerTurn == 1 && s.board[index - 9] == 2)
	{
		return true;
	}
	
	if(s.playerTurn == 2 && s.board[index + 9] == 1)
	{
		return true;
	}
	
	return false;
}

std::vector<PieceAction> GetLegalActions1(BoardState s)
{
	std::vector<PieceAction> legalMoves;
	PieceAction forward, diagRight, diagLeft;

	for(int index = 0; index < 64; index++)
	{
		if(s.board[index] != 1)
		{
			continue;
		}
		
		if(moveDiagonalLeft(s, index))
		{
			diagLeft = PieceAction(index, index - 9);
			legalMoves.push_back(diagLeft);
			std::cout << "Move: " << index << " to: " << index - 9 << std::endl;

		}
		
		if(moveForward(s, index))
		{
			forward = PieceAction(index, index - 8);
			legalMoves.push_back(forward);
			std::cout << "Move: " << index << " to: " << index - 8 << std::endl;

		}
		
		if(moveDiagonalRight(s, index))
		{
			diagRight = PieceAction(index, index - 7);
			legalMoves.push_back(diagRight);
			std::cout << "Move: " << index << " to: " << index - 7 << std::endl;

		}
	}
	
	return legalMoves;
}

std::vector<PieceAction> GetLegalActions2(BoardState s)
{
	std::vector<PieceAction> legalMoves;
	PieceAction forward, diagRight, diagLeft;
	
	for(int index = 0; index < 64; index++)
	{
		if(s.board[index] != 2)
		{
			continue;
		}
		
		if(moveDiagonalRight(s, index))
		{
			diagRight = PieceAction(index, index + 7);
			legalMoves.push_back(diagRight);
			std::cout << "Move: " << index << " to: " << index + 7 << std::endl;
		}		
		
		if(moveForward(s, index))
		{
			forward = PieceAction(index, index + 8);
			legalMoves.push_back(forward);
			std::cout << "Move: " << index << " to: " << index + 8 << std::endl;
		}
		
		if(moveDiagonalLeft(s, index))
		{
			diagLeft = PieceAction(index, index + 9);
			legalMoves.push_back(diagLeft);
			std::cout << "Move: " << index << " to: " << index + 9 << std::endl;
		}	
	}
	
	return legalMoves;
}

std::vector<PieceAction> GetPieceActions(BoardState s, int index)
{
	std::vector<PieceAction> legalMoves;
	PieceAction forward, diagRight, diagLeft;
	
	if(s.playerTurn == 1)
	{
		if(moveDiagonalRight(s, index))
		{
			diagRight = PieceAction(index, index - 7);
			legalMoves.push_back(diagRight);
		}		
		
		if(moveForward(s, index))
		{
			forward = PieceAction(index, index - 8);
			legalMoves.push_back(forward);
		}
		
		if(moveDiagonalLeft(s, index))
		{
			diagLeft = PieceAction(index, index - 9);
			legalMoves.push_back(diagLeft);
		}	
	}
	
	else {
		
		if(moveDiagonalRight(s, index))
		{
			diagRight = PieceAction(index, index + 7);
			legalMoves.push_back(diagRight);
		}		
		
		if(moveForward(s, index))
		{
			forward = PieceAction(index, index + 8);
			legalMoves.push_back(forward);
		}
		
		if(moveDiagonalLeft(s, index))
		{
			diagLeft = PieceAction(index, index + 9);
			legalMoves.push_back(diagLeft);
		}	
	}
	
	
	return legalMoves;
}

int GetNumActionsForState(BoardState s)
{
	if(s.playerTurn == 1)
	{
		return GetNumActions1(s);
	}
	
	else {
		return GetNumActions2(s);
	}
	
	return 0;
}
		
int GetNumActions1(BoardState s)
{
	return GetLegalActions1(s).size();
}

int GetNumActions2(BoardState s)
{
	return GetLegalActions2(s).size();
}

BoardState ApplyAction(BoardState s, PieceAction a)
{
	if(s.playerTurn == 1)
	{
		s.board[a.from] = 0;
		s.board[a.to] = 1;
	}
	
	if(s.playerTurn == 2)
	{
		s.board[a.from] = 0;
		s.board[a.to] = 2;
	}
	
	return s;
}

BoardState UndoAction(BoardState s, PieceAction a)
{
	if(s.playerTurn == 1)
	{
		s.board[a.from] = 1;
		s.board[a.to] = 0;
	}
	
	if(s.playerTurn == 2)
	{
		s.board[a.from] = 2;
		s.board[a.to] = 0;
	}
	
	return s;
}

int GetIndex(int x, int y)
{
	return y * 8 + x - 1;
}

int GetColumn(int index)
{
	return index % 8;
}

int GetRow(int index)
{
	return index / 8 + 1;
}

void PrintBoard(BoardState currentBoard)
{
	int line = 1;
	
	std::cout << "   A B C D E F G H" << std::endl;
	std::cout << line << "| ";
	
	for(int i = 0; i < 64; i++)
	{
		if(i == 8 * line)
		{
			std::cout << std::endl;
			line ++;
			std::cout << line << "| ";
		}
		if (currentBoard.board[i] == 1) {
			std::cout << "1 ";
		}
		else if (currentBoard.board[i] == 2) {
			std::cout << "2 ";
		}
		
		else {
			std::cout << "O ";
		}
	}
	
	std::cout << "\n";
	
//	std::vector<PieceAction> moves = GetLegalActions2(currentBoard);
	
	//	for (std::vector<PieceAction>::size_type i = 0; i < moves.size(); ++i)
	//		std::cout << "Move from: " << moves[i].from	<< " to: " << moves[i].to << std::endl;
	
	std::cout << "\n";
}
	   
int GetNumPiecesForState(BoardState s)
{	
	if(s.playerTurn == 1)
	{
		return GetNumPieces1(s);
	}
	
	else {
		return GetNumPieces2(s);
	}
	
	return 0;
}

int GetNumPieces1(BoardState s)
{
	int numPieces = 0;
	
	for(int i = 0; i < 64; i++)
	{
		if(s.board[i] == 1)
		{
			numPieces ++;
		}
	}
	
	return numPieces;
}

int GetNumPieces2(BoardState s)
{
	int numPieces = 0;
	
	for(int i = 0; i < 64; i++)
	{
		if(s.board[i] == 2)
		{
			numPieces ++;
		}
	}
	
	return numPieces;
}

int GameOver(BoardState s)
{
	if(GetNumPieces1(s) == 0)
	{
		std::cout << "Player 1 is out of pieces." << std::endl;
		return 2;
	}
	
	if(GetNumPieces2(s) == 0)
	{
		std::cout << "Player 2 is out of pieces." << std::endl;
		return 1;
	}
	
	if(s.playerTurn == 1)
	{
		for(int i = 0; i < 8; i++)
		{
			if(s.board[i] == 1)
			{
				std::cout << "Player 1 reached the end." << std::endl;
				return 1;
			}
			   
		}
	}
	
	if(s.playerTurn == 2)
	{
		for(int i = 56; i < 64; i++)
		{
			if(s.board[i] == 2)
			{
				std::cout << "Player 2 reached the end." << std::endl;
				return 2;
			}
			
		}
	}
	
	return 0;		   
}

void SwitchTurn(BoardState s)
{
	if(s.playerTurn == 1)
	{
		s.playerTurn = 2;
	}
	
	else {
		s.playerTurn = 1;
	}
	
	Play(s);
}

int ConvertChar(char x)
{
	switch(x)
	{
		case 'A':
			return 1;
		case 'B':
			return 2;
		case 'C':
			return 3;
		case 'D':
			return 4;
		case 'E':
			return 5;
		case 'F':
			return 6;
		case 'G':
			return 7;
		case 'H':
			return 8;
	}
	
	return 0;
}

void Play(BoardState s)
{
	char x, toX;
	int y, toY;
	int index, toIndex;
	
	std::cout << "Player " << s.playerTurn << std::endl;
	std::cout << "Enter the coordinates of the piece you want to move:" << std::endl;
	std::cin >> x >> y;
	
	int xLoc = ConvertChar(x);
	index = GetIndex(xLoc, y - 1);
	
	std::cout << "Index: " << index << std::endl;
	
	if(s.board[index] != s.playerTurn)
	{
		std::cout << "You don't have a piece there." << std::endl;
		return;
	}
	
	std::vector<PieceAction> moves = GetPieceActions(s, index);
	
	if(moves.size() == 0)
	{
		std::cout << "That piece has no legal moves." << std::endl;
		return;
	}
	
	std::cout << "The piece at: " << x << " " << y << " can move to: " << std::endl;
	
	for (int i = 0; i < moves.size(); i++)//std::vector<PieceAction>::size_type i = 0; i < moves.size(); ++i)
	{
		//char moveFromColumn = 'A' + GetColumn(moves[i].from);
		//int moveFromRow = GetRow(moves[i].from);
		char moveToColumn = 'A' + GetColumn(moves[i].to);
		int moveToRow = GetRow(moves[i].to);
		std::cout << moveToColumn << " " << moveToRow << std::endl;
	}
	
	std::cout << std::endl;
	
	std::cout << "Enter the coordinates you want to move to:" << std::endl;
	std::cin >> toX >> toY;
	
	int toXLoc = ConvertChar(toX);
	toIndex = GetIndex(toXLoc, toY - 1);
	int legalMoveNum = -1;
	
	for(int i = 0; i < moves.size(); i++)
	{
		if(moves[i].to == toIndex)
		{
			legalMoveNum = i;
			break;
		}
	}
	
	if(legalMoveNum == -1)
	{
		std::cout << "Illegal action." << std::endl;
		return;
	}
	
	s = ApplyAction(s, moves[legalMoveNum]);
	PrintBoard(s);
		
	if(GameOver(s) == 1)
	{
		std::cout << "Player 1 wins!" << std::endl;
		return;
	}
		
	if(GameOver(s) == 2)
	{
		std::cout << "Player 2 wins!" << std::endl;
		return;
	}
		
	SwitchTurn(s);
}

















int main () {
	BoardState game = GetStartState();
	game.playerTurn = 1;
	PrintBoard(game);
	Minimax_MaxPlayer(game, 0);
//		HumanTurn(game, 0);
//	runSearch();
//	Play(game);
	
//	PieceAction action = PieceAction(49, 40);
//	game = ApplyAction(game, action);
//	PrintBoard(game);
	
	
    return 0;
}
