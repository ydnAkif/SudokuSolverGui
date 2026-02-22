#include <gtest/gtest.h>
#include "Sudoku.hpp"

/**
 * @class SudokuTest
 * @brief Unit tests for Sudoku solver
 */
class SudokuTest : public ::testing::Test
{
protected:
    Sudoku sudoku;

    /// Simple solvable puzzle
    std::vector<std::vector<int>> solvablePuzzle = {
        {8, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 3, 6, 0, 0, 0, 0, 0},
        {0, 7, 0, 0, 9, 0, 2, 0, 0},
        {0, 5, 0, 0, 0, 7, 0, 0, 0},
        {0, 0, 0, 0, 4, 5, 7, 0, 0},
        {0, 0, 0, 1, 0, 0, 0, 3, 0},
        {0, 0, 1, 0, 0, 0, 0, 6, 8},
        {0, 0, 8, 5, 0, 0, 0, 1, 0},
        {0, 9, 0, 0, 0, 0, 4, 0, 0}};

    /// Invalid puzzle (no solution)
    std::vector<std::vector<int>> unsolvablePuzzle = {
        {1, 1, 0, 0, 0, 0, 0, 0, 0}, // Two 1s in same row
        {0, 0, 3, 6, 0, 0, 0, 0, 0},
        {0, 7, 0, 0, 9, 0, 2, 0, 0},
        {0, 5, 0, 0, 0, 7, 0, 0, 0},
        {0, 0, 0, 0, 4, 5, 7, 0, 0},
        {0, 0, 0, 1, 0, 0, 0, 3, 0},
        {0, 0, 1, 0, 0, 0, 0, 6, 8},
        {0, 0, 8, 5, 0, 0, 0, 1, 0},
        {0, 9, 0, 0, 0, 0, 4, 0, 0}};
};

/// Test: Solving a valid puzzle
TEST_F(SudokuTest, SolveSolvablePuzzle)
{
    Sudoku solver(solvablePuzzle);
    EXPECT_TRUE(solver.solve());

    // Verify no empty cells
    for (int r = 0; r < 9; ++r)
    {
        for (int c = 0; c < 9; ++c)
        {
            EXPECT_NE(solver.getCell(r, c), 0);
        }
    }
}

/// Test: Solving with step recording
TEST_F(SudokuTest, SolveWithSteps)
{
    Sudoku solver(solvablePuzzle);
    std::vector<SolveStep> steps;
    EXPECT_TRUE(solver.solveWithSteps(steps));

    // Should have recorded steps
    EXPECT_GT(steps.size(), 0);
}

/// Test: Detecting unsolvable puzzles
TEST_F(SudokuTest, UnsolvablePuzzleDetection)
{
    Sudoku solver(unsolvablePuzzle);
    EXPECT_FALSE(solver.solve());
}

/// Test: Given cells remain unchanged
TEST_F(SudokuTest, GivenCellsUnchanged)
{
    Sudoku solver(solvablePuzzle);
    int givenValue = solvablePuzzle[0][0]; // 8

    solver.solve();
    EXPECT_EQ(solver.getCell(0, 0), givenValue);
}

/// Test: Cell validation
TEST_F(SudokuTest, CellValidation)
{
    Sudoku solver(solvablePuzzle);

    // Valid move (empty cell, legal number)
    EXPECT_TRUE(solver.isValid(0, 1, 1));

    // Invalid move (different number already in column)
    EXPECT_FALSE(solver.isValid(0, 1, 8)); // 8 already at (0,0)
}

/// Test: Empty puzzle solving
TEST_F(SudokuTest, EmptyPuzzle)
{
    std::vector<std::vector<int>> emptyBoard(9, std::vector<int>(9, 0));
    Sudoku solver(emptyBoard);
    EXPECT_TRUE(solver.solve());

    // Should fill all cells with valid values (1-9)
    for (int r = 0; r < 9; ++r)
    {
        for (int c = 0; c < 9; ++c)
        {
            int val = solver.getCell(r, c);
            EXPECT_GE(val, 1); // >= 1
            EXPECT_LE(val, 9); // <= 9
        }
    }
}

/// Test: Reset functionality
TEST_F(SudokuTest, ResetBoard)
{
    Sudoku solver(solvablePuzzle);
    solver.solve();

    // Reset
    solver.reset(solvablePuzzle);

    // Should be back to initial state
    EXPECT_EQ(solver.getCell(0, 0), 8);
    EXPECT_EQ(solver.getCell(0, 1), 0);
}

/// Test: Given cell marking
TEST_F(SudokuTest, GivenCellMarking)
{
    Sudoku solver(solvablePuzzle);

    EXPECT_TRUE(solver.isGiven(0, 0));  // 8 is given
    EXPECT_FALSE(solver.isGiven(0, 1)); // 0 is not given
}
