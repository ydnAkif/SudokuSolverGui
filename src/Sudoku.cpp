#include "Sudoku.hpp"

// Default puzzle (for initial load)
const std::vector<std::vector<int> > Sudoku::DEFAULT_BOARD = {
    {8, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 3, 6, 0, 0, 0, 0, 0},
    {0, 7, 0, 0, 9, 0, 2, 0, 0},
    {0, 5, 0, 0, 0, 7, 0, 0, 0},
    {0, 0, 0, 0, 4, 5, 7, 0, 0},
    {0, 0, 0, 1, 0, 0, 0, 3, 0},
    {0, 0, 1, 0, 0, 0, 0, 6, 8},
    {0, 0, 8, 5, 0, 0, 0, 1, 0},
    {0, 9, 0, 0, 0, 0, 4, 0, 0}
};

// Constructor / reset
Sudoku::Sudoku(const std::vector<std::vector<int> > &inputBoard) {
    reset(inputBoard);
}

void Sudoku::reset(const std::vector<std::vector<int> > &inputBoard) {
    board_ = inputBoard;
    given_.assign(SIZE, std::vector<bool>(SIZE, false));
    for (size_t r = 0; r < SIZE; ++r)
        for (size_t c = 0; c < SIZE; ++c)
            given_[r][c] = (board_[r][c] != 0);
}

// Accessors removed - now inline in header with helper functions
// This eliminates sign-conversion warnings from accessor calls

// Validity check: ensure placing 'num' at (row, col) doesn't violate Sudoku rules
bool Sudoku::isValid(const int row, const int col, const int num) const {
    // Check row and column using helper functions
    for (int i = 0; i < SIZE; ++i) {
        if (i != col && getFromBoard(board_, row, i) == num) return false;
        if (i != row && getFromBoard(board_, i, col) == num) return false;
    }
    // Check 3x3 box
    const int br = (row / GRID_SIZE) * GRID_SIZE;
    const int bc = (col / GRID_SIZE) * GRID_SIZE;
    for (int r = br; r < br + GRID_SIZE; ++r)
        for (int c = bc; c < bc + GRID_SIZE; ++c)
            if ((r != row || c != col) && getFromBoard(board_, r, c) == num) return false;
    return true;
}

// Fast solve (without step tracking)
bool Sudoku::solve() {
    for (int r = 0; r < SIZE; ++r) {
        for (int c = 0; c < SIZE; ++c) {
            if (getFromBoard(board_, r, c) != 0) continue;
            for (int num = 1; num <= 9; ++num) {
                if (!isValid(r, c, num)) continue;
                setToBoard(board_, r, c, num);
                if (solve()) return true;
                setToBoard(board_, r, c, 0); // backtrack
            }
            return false; // no number fits -> backtrack
        }
    }
    return true; // all cells filled
}

// Solve with step recording for animation
bool Sudoku::solveRecursive(std::vector<SolveStep> &steps) {
    for (int r = 0; r < SIZE; ++r) {
        for (int c = 0; c < SIZE; ++c) {
            if (getFromBoard(board_, r, c) != 0) continue;
            for (int num = 1; num <= 9; ++num) {
                if (!isValid(r, c, num)) continue;
                setToBoard(board_, r, c, num);
                steps.push_back({r, c, num}); // record placement
                if (solveRecursive(steps)) return true;
                setToBoard(board_, r, c, 0);
                steps.push_back({r, c, 0}); // record removal (backtrack)
            }
            return false;
        }
    }
    return true;
}

bool Sudoku::solveWithSteps(std::vector<SolveStep> &steps) {
    steps.clear();
    return solveRecursive(steps);
}


