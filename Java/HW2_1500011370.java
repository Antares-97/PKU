import java.util.*;
import java.io.*;
////////
//产生方块的函数是一个 void 类型,且由一个 randomseed 产生,由此看来我们无法直接获取当前下落的 piece 类型 只能通过读取整个网格来获取信息
//搞错了 直接通过 piece 来访问当前下落的方块, 通过 getpiece 函数也可以得到 整个网盘结构由 board, 或 getboard 来得到
//算法的思路是: 由网盘当前状态来得到那6个参数; 通过当前下落方块类型,遍历其操作类型和放置的位置,得出其最佳位置位,判断最佳位置位与当前位置的差距,选择最能使之接近的操作作为返回值
//有一个重要的问题: 到底是每一个随机方块出来之后就立马进行一次判断,找出最优位置和操作; 还是每一次刷新都要进行一次查找最优操作?
//答案是后者, 我们看到 movepiece 函数当中每一次都会对 piece 的状态进行修改
//引入了评价函数, AI 算法可能就是对每个方块的各种形态,对每种可能的下放位置重新计算评价函数,找到最大的值返回操作类型
//目前问题:怎么操控它落在最终位置? 然后进行评价函数计算?
//有效网盘是10x20的一个格盘,其实还是要搞清楚怎么判断的
//对四种操作分别进行分析:1.笔直落下;2.向左移动;3.向右移动;4.旋转之后,再找最佳点;比较4种操作最佳的评价函数,返回对应的操作
public class HW2_1500011370 extends Tetris 
{
	// enter your student id here
	public String id = new String("1500011370");
	
	public int RowsEliminated(boolean tempBoard[][])
	{
		int eliminate = 0;
		for (int j = 0; j < h; j++)
		{
			int count = 0;
			for (int i = 0; i < w; i++)
			{
				count += 1;
				if(tempBoard[j][i] == false)
				{
					break;
				}
			}
			if (count == w && tempBoard[j][count-1])
			{
				eliminate += 1;
				for (int i = 0; i < w; i++)
				{
					tempBoard[j][i] = false;
				} //消去的那一列清除
				for (int z = j; z < h-1; z++)
				{
					for(int i = 0; i < w; i++)
					{
						tempBoard[z][i] = tempBoard[z+1][i];
					}
				} //y以上的全部下降一格
				for (int i = 0; i < w; i++)
				{
					tempBoard[h-1][i] = false;
				}
			}
		}
		return eliminate;
	}
	
	public int GetRowTransitions(boolean tempBoard[][])		//行变换个数, 定义见网页说明
	{

		int RowTransition = 0;
		for(int j=0;j<h;j++)
		{
			int currentTransition=0;
			boolean prevColor=true;
			for(int i=0;i<w;i++)			//从左到右扫描一行, 每改变一次颜色就将行变换数加一
			{
				if(tempBoard[j][i]!=prevColor)
				{
					currentTransition++;
					prevColor=tempBoard[j][i];
				}
			}
			if(prevColor==false)
			{
				currentTransition++;
			}
			RowTransition+=currentTransition;
		}
		return RowTransition;
	}
	
	public int GetColumnTransitions(boolean tempBoard[][])
	{
		int ColumnTransition = 0;
		for(int i=0;i<w;i++)
		{
			int currentTransition=0;
			boolean prevColor=true;
			for(int j=0;j<h;j++)			//从左到右扫描一行, 每改变一次颜色就将行变换数加一
			{
				if(tempBoard[j][i]!=prevColor)
				{
					currentTransition++;
					prevColor=tempBoard[j][i];
				}
			}
			if(prevColor==false)
			{
				currentTransition++;
			}
			ColumnTransition+=currentTransition;
		}
		return ColumnTransition;
	}
	
	public int GetHoleNumbers(boolean tempBoard[][])
	{
		int Emptyholes=0;
		for(int j=0;j<h;j++)
		{
			for(int i=0;i<w;i++)
			{
				if(j<h-1)
				{
					if(tempBoard[j][i]==false&&tempBoard[j+1][i]==true)
					{
						Emptyholes+=1;
					}
				}
			}
		}
		return Emptyholes;
	}
	
	public int GetWellDepth(boolean tempBoard[][])
	{
		int i=0,j=0;
		int wellDepth=0, currentDepth=0;
		int totalWellDepth=0;
		for(;j<h;j++)		//最左边一列的井数
		{
			if(tempBoard[j][0]==false&&tempBoard[j][1]==true)
			{
				currentDepth++;
				wellDepth+=currentDepth;
			}
			else
			{
				currentDepth=0;
			}
		}
		totalWellDepth+=wellDepth;
		currentDepth=0;
		wellDepth=0;
		
		for(j=0;j<h;j++)		//最右边一列的井数
		{
			if(tempBoard[j][w-1]==false&&tempBoard[j][w-2]==true)
			{
				currentDepth++;
				wellDepth+=currentDepth;
			}
			else
			{
				currentDepth=0;
			}
		}
		totalWellDepth+=wellDepth;
		currentDepth=0;
		wellDepth=0;
		
		for(i=1;i<w-1;i++)
		{
			for(j=0;j<h;j++)		
			{
				if(tempBoard[j][i]==false&&tempBoard[j][i-1]==true&&tempBoard[j][i+1]==true)
				{
					currentDepth++;
					wellDepth+=currentDepth;
				}
				else
				{
					currentDepth=0;
				}
			}
			totalWellDepth+=wellDepth;
			currentDepth=0;
			wellDepth=0;
		}
		return totalWellDepth;
	}
	
	public int GetHeight(boolean tempBoard[][])
	{
		int height=0;
		for(int j=0;j<h;j++)
		{
			for(int i=0;i<w;i++)
			{
				if(tempBoard[j][i]==true&&j>=height) height=j+1;
			}
		}
		return height;
	}
	
	public double Evaluate(boolean __board[][])	//根据给定局面和方块类型与位置计算评价函数
	{
		int RowsEliminated=RowsEliminated(__board);
		int RowsTransition=GetRowTransitions(__board);
		int ColumnTransition=GetColumnTransitions(__board);
		int HoleNum=GetHoleNumbers(__board);
		int WellDepth=GetWellDepth(__board);
		int Height=GetHeight(__board);
		
		double CurrentEvaluate= (3.4181268101392694) * RowsEliminated          // 消行个数
			    + (-3.217888286848775) * RowsTransition          // 行变换
			    + (-9.348695305445199) * ColumnTransition        // 列变化
			    + (-10.899265427351652) * HoleNum                 // 空洞个数
			    + (-3.385597224726362) * WellDepth			    //井深
				+ (-9.014950192030519) * Height;
		
		//System.out.printf("%f\n", CurrentEvaluate);
		return CurrentEvaluate;
	}
	
	public int[] Get_Piece_X_range(boolean piece[][])
	{
		int min_x=4;
		int max_x=0;
		int x_range[]=new int [2];
		for(int j=0;j<4;j++)
		{
			for(int i=0;i<4;i++)
			{
				if(piece[j][i]==false) continue;
				if(i<min_x) min_x=i;
				if(i>max_x) max_x=i;
			}
		}
		x_range[0]=-min_x;					//当最左边的方块顶到左边墙时候的 piece_x
		x_range[1]=w-1-max_x;				//当最右边的方块顶到右边墙时候的 piece_x
		return x_range;
	}
	
	public boolean overlap(boolean _board[][],boolean _piece[][], int x,int y)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int i = 0; i < 4; i++)
			{
				if (_piece[j][i]==true && y  < j) return true;
				if (y < j || x+i < 0 || x+i > w -1) continue;
				if(_piece[j][i] && _board[y-j][x+i]) return true;
			}
		}
		return false;
	}
	
	public boolean[][] Get_New_Board(boolean _board[][],boolean _piece[][], int x)	// 传回 方块从 x 处往下落之后的 board
	{
		int y = h - 1;
		for (; y >= 0; y--)
		{
			if(overlap(_board,_piece,x,y))
			{
				y++;
				break;
			}						//找到 piece 的下落位置
		}
		boolean[][] tmp = new boolean[h][w];
		// 复制_board到tmp
		for (int j = 0; j < h; j++) 
		{
			for (int i = 0; i < w; i++) 
			{
				tmp[j][i] = _board[j][i];
			}
		}
		//将掉下来的_piece加到board上去，但不改变传入的board
		for (int j = 0; j < 4; j++)
		{
			for (int i = 0; i < 4; i++)
			{
				if (y-j < 0 || x+i < 0 || x+i > w-1)
				{
					continue;
				}
				if (_piece[j][i]) tmp[y-j][x+i] = true;
			}
		}
		return tmp;
	}
	
	public int[] Get_Best_Strategy(boolean board[][],boolean piece[][], int piece_x, int piece_y)
	{
		int Best_Strategy[]=new int [2];// BS[0]为旋转情况, BS[1]为最佳位置
		double BestEvaluator=-9999999999.99,Evaluator;	//当前状态下的评价函数
		int xrange[]=Get_Piece_X_range(piece);		//获取当前方块 x 状态
		boolean piece1[][]=new boolean[4][4];		//旋转1次
		boolean piece2[][]=new boolean[4][4];		//旋转2次
		boolean piece3[][]=new boolean[4][4];		//旋转3次
		
		for (int j = 0; j < 4; j++) 
		{
			for (int i = 0; i < 4; i++) 
			{
				if (piece[j][i]) board[piece_y-j][piece_x+i] = false;
			}
		}	//在board中移去piece
		
		for(int j=0;j<4;j++)
		{
			for(int i=0;i<4;i++)
			{
				piece1[j][i]=piece[i][3-j];
			}
		}
		for(int j=0;j<4;j++)
		{
			for(int i=0;i<4;i++)
			{
				piece2[j][i]=piece1[i][3-j];
			}
		}
		for(int j=0;j<4;j++)
		{
			for(int i=0;i<4;i++)
			{
				piece3[j][i]=piece2[i][3-j];
			}
		}
		
		for(int i=xrange[1];i>=xrange[0];i--)			//对没有旋转的 piece, 对其每个可能的落点,都进行评分
		{
			 boolean tempBoard[][]=new boolean[h][w];
			 tempBoard=Get_New_Board(board,piece,i);		//哇这里错了!!! 这里放的是 piece
			 Evaluator=Evaluate(tempBoard);
			 if(Evaluator>BestEvaluator)
			 {
				 BestEvaluator=Evaluator;
				 Best_Strategy[0]=0;
				 Best_Strategy[1]=i;
			 }
		}
		
		xrange=Get_Piece_X_range(piece1);
		for(int i=xrange[1];i>=xrange[0];i--)			//对旋转1次的 piece1, 对其每个可能的落点,都进行评分
		{
			 boolean tempBoard[][]=Get_New_Board(board,piece1,i);		// 这里要放 piece1 !!!
			 Evaluator=Evaluate(tempBoard);
			 if(Evaluator>BestEvaluator)
			 {
				 BestEvaluator=Evaluator;
				 Best_Strategy[0]=1;
				 Best_Strategy[1]=i;
			 }
		}
		
		xrange=Get_Piece_X_range(piece2);
		for(int i=xrange[1];i>=xrange[0];i--)			//对旋转2次的 piece2, 对其每个可能的落点,都进行评分
		{
			 boolean tempBoard[][]=Get_New_Board(board,piece2,i);		//放 piece2
			 Evaluator=Evaluate(tempBoard);
			 if( Evaluator>BestEvaluator)
			 {
				 BestEvaluator=Evaluator;
				 Best_Strategy[0]=2;
				 Best_Strategy[1]=i;
			 }
		}
		
		xrange=Get_Piece_X_range(piece3);
		for(int i=xrange[1];i>=xrange[0];i--)			//对旋转3次的 piece, 对其每个可能的落点,都进行评分
		{
			 boolean tempBoard[][]=Get_New_Board(board,piece3,i);		//放 piece3
			 Evaluator=Evaluate(tempBoard);
			 if( Evaluator>BestEvaluator)
			 {
				 BestEvaluator=Evaluator;
				 Best_Strategy[0]=3;
				 Best_Strategy[1]=i;
			 }
		}		
		return Best_Strategy;				//晕,放的方块一直都是 piece 所以才不好啊!
	}
	
	public PieceOperator robotPlay() 				//覆盖了 Tetris 当中的 robotPlay 函数, 函数每次对局面作出决策,返回一个操作类型
	{	
		boolean tempPiece[][]=getPiece();
		boolean tempBoard[][]=getBoard();
		int Piece_x=getPieceX();
		int Piece_y=getPieceY();
		int Best_strategy[]=new int [2];				//接收最佳策略

		Best_strategy=Get_Best_Strategy(tempBoard,tempPiece,Piece_x,Piece_y);
		if (Best_strategy[0] > 0)
		{
			return PieceOperator.Rotate;
		}
		if (Best_strategy[1] > Piece_x)
		{
			return PieceOperator.ShiftRight;
		}
		if (Best_strategy[1] < Piece_x)
		{
			return PieceOperator.ShiftLeft;
		}
		
		return PieceOperator.Drop;
	}
}