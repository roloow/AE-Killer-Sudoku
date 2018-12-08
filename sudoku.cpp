#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <set>
#include <algorithm>
#include <iterator>
#include <cstdlib>
#include <ctime>
#include <cassert>
#include <climits>


using namespace std;

typedef vector<int> Board;
typedef pair<int, board> DNA;
typedef vector<DNA> Population;
typedef Population::iterator PopIter; // No idea
typedef Population::cons_iterator PopROIter; // No idea

const int PROBLEM_SIZE = 3;
const int NONET_SIZE = PROBLEM_SIZE * PROBLEM_SIZE;
const int BOARD_SIZE = NONET_SIZE * NONET_SIZE;

int getAt (const Board& board, int row, int col);
void setAt(Board& board, int row, int col, int newVal);
int blockNum(int row, int col);
int indexOf(int row, int col);
void timestamp(ostream& ostr);
int randNum(int low, int high);
Board loadBoard(string filename);

int main(){}

Board loadBoard(string filename){
	
}
