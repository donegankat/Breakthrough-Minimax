/*
 *  ScrapCode.cpp
 *  Breakthrough Minimax
 *
 *  Created by K Donegan on 2/5/13.
 *  Copyright 2013 University of Denver. All rights reserved.
 *
 */


/*
 void runDFS()
 {
 int64_t *depth = new int64_t[64];
 
 for(int i = 0; i < 64; i++)
 {
 depth[i] = -1;
 }
 
 }
 
 void DepthFirstSearch(BoardState s)
 {
 int8_t *depth;
 
 for(int i = 0; i < 64; i++)
 {
 depth[i] = -1;
 }
 
 if(GameOver(s))
 {
 std::cout << "Game over reached. Win for Player " << s.playerTurn << " at depth: " << depth << std::endl;
 delete[] depth;
 return;
 }
 
 BoardState nextState;
 std::vector<PieceAction> ActionsForState = ;
 
 if(s.playerTurn == 1)
 {
 ActionsForState = GetLegalActions1(s);
 }
 
 if(s.playerTurn == 2)
 {
 ActionsForState = GetLegalActions2(s);
 }
 
 for(int i = 0; i < ActionsForState.size(); i++)
 {		
 nextState = ApplyAction(s, ActionsForState.pop_back());
 EvalFunction(nextState);
 depth ++;
 DepthFirstSearch(nextState);
 }
 
 }
 */








/*
 void MaxPlayer(BoardState s, int depth, int worst, int best)
 {
 
 int bestValue = best;
 PieceAction bestMove;
 
 std::vector<PieceAction> MaxMoves = GetLegalActions1(s);
 
 for(int i = 0; i < MaxMoves.size(); i++)
 {
 
 std::cout << "Legal move: " << MaxMoves[i].from << " to: " << MaxMoves[i].to << std::endl;
 
 
 if(EvalPiece(s, MaxMoves[i].to) > bestValue)
 {
 bestValue = EvalPiece(s, MaxMoves[i].to);
 bestMove = PieceAction(MaxMoves[i].from, MaxMoves[i].to);
 
 std::cout << "New Maximum Value: " << bestValue << std::endl;
 }
 }
 
 BoardState nextState;
 nextState = ApplyAction(s, bestMove);
 
 if(GameOver(nextState))
 {
 std::cout << "Game over reached. Win for Player 1 at depth: " << depth << std::endl;
 return;
 }		
 
 std::cout << "Depth: " << depth << std::endl;
 depth ++;
 
 if(depth < 6)
 {
 MinPlayer(nextState, depth, worst, bestValue);
 }
 
 else {
 return;
 }
 
 }
 
 void MinPlayer(BoardState s, int depth, int worst, int best)
 {
 //	static int worstValue = 100;
 int worstValue = worst;
 PieceAction worstMove;
 
 std::vector<PieceAction> MinMoves = GetLegalActions2(s);
 
 for(int i = 0; i < MinMoves.size(); i++)
 {
 
 //		std::cout << "Legal move: " << MinMoves[i].from << " to: " << MinMoves[i].to << std::endl;
 
 if(EvalPiece(s, MinMoves[i].to) < worstValue)
 {
 worstValue = EvalPiece(s, MinMoves[i].to);
 worstMove = PieceAction(MinMoves[i].from, MinMoves[i].to);
 std::cout << "New Minimum Value: " << worstValue << std::endl;
 }
 }	
 BoardState nextState;
 nextState = ApplyAction(s, worstMove);
 
 if(GameOver(nextState))
 {
 std::cout << "Game over reached. Win for Player 2 at depth: " << depth << std::endl;
 //			delete[] depth;
 return;
 }		
 
 //		EvalFunction(nextState);
 //		std::cout << "Depth: " << &depth << std::endl;
 depth ++;
 
 if(depth < 6)
 {
 MaxPlayer(nextState, depth, worstValue, best);
 }
 
 else {
 return;
 }
 
 
 }
 
 */ 

/*
 void EvalFunction(BoardState s)
 {
 static int bestValue = 0;
 static int worstValue = 100;
 
 for(int i = 0; i < 64; i++)
 {
 if(s.board[i] == s.playerTurn)
 {
 if(EvalPiece(s, i) > bestValue)
 {
 bestValue = EvalPiece(s, i);
 }
 
 if(EvalPiece(s, i) < worstValue)
 {
 worstValue = EvalPiece(s, i);
 }
 }
 }
 
 
 //	return 0;
 }
 */

