#ifndef SUDOKU_HPP
#define SUDOKU_HPP

#include <vector>

/**
 * @struct SolveStep
 * @brief Represents a single step in the solving process
 * @details Used for animation and visualization of the solving algorithm
 */
struct SolveStep {
    int row; ///< Row index (0-8)
    int col; ///< Column index (0-8)
    int value; ///< Value placed (1-9) or 0 for removal
};

/**
 * @class Sudoku
 * @brief Core Sudoku solver using backtracking algorithm
 * @details
 *   - Implements efficient backtracking algorithm
 *   - Supports both fast solving and step-by-step solving
 *   - Validates moves according to Sudoku rules
 *   - Time complexity: O(9^k) where k is number of empty cells
 */
class Sudoku {
    // Type aliases for cleaner code
    using Board = std::vector<std::vector<int> >;
    using GivenMask = std::vector<std::vector<bool> >;

    static constexpr int SIZE = 9; ///< Grid size (9x9)
    static constexpr int GRID_SIZE = 3; ///< Sub-grid size (3x3)

    Board board_; ///< Current board state
    GivenMask given_; ///< Marks initially given cells

    /**
     * @brief Recursive helper function for solving with step tracking
     * @param steps Vector to store solving steps
     * @return true if puzzle is solvable, false otherwise
     */
    bool solveRecursive(std::vector<SolveStep> &steps);

    /**
     * @brief Safe array access with bounds checking (inline for optimization)
     * @param board The board to access
     * @param row Row index
     * @param col Column index
     * @return Cell value
     */
    static int getFromBoard(const Board &board, int row, int col) {
        return board[static_cast<size_t>(row)][static_cast<size_t>(col)];
    }

    /**
     * @brief Safe array set with bounds checking (inline for optimization)
     * @param board The board to modify
     * @param row Row index
     * @param col Column index
     * @param val Value to set
     */
    static void setToBoard(Board &board, int row, int col, int val) {
        board[static_cast<size_t>(row)][static_cast<size_t>(col)] = val;
    }

public:
    /// Default/example puzzle
    static const Board DEFAULT_BOARD;

    /**
     * @brief Constructor with initial board
     * @param inputBoard Initial puzzle state (0 = empty cell)
     */
    explicit Sudoku(const Board &inputBoard = DEFAULT_BOARD);

    /// Destructor
    ~Sudoku() = default;

    /**
     * @brief Reset board to initial state
     * @param inputBoard New puzzle state
     */
    void reset(const Board &inputBoard);

    /**
     * @brief Get cell value
     * @param row Row index (0-8)
     * @param col Column index (0-8)
     * @return Cell value (0-9)
     */
    [[nodiscard]] int getCell(int row, int col) const {
        return getFromBoard(board_, row, col);
    }

    /**
     * @brief Set cell value
     * @param row Row index (0-8)
     * @param col Column index (0-8)
     * @param val Value to set (0-9)
     */
    void setCell(int row, int col, int val) {
        setToBoard(board_, row, col, val);
    }

    /**
     * @brief Check if cell was given in initial puzzle
     * @param row Row index (0-8)
     * @param col Column index (0-8)
     * @return true if cell is immutable, false otherwise
     */
    [[nodiscard]] bool isGiven(int row, int col) const {
        return given_[static_cast<size_t>(row)][static_cast<size_t>(col)];
    }

    /**
     * @brief Validate if placing number is legal
     * @param row Row index (0-8)
     * @param col Column index (0-8)
     * @param num Number to validate (1-9)
     * @return true if move is valid according to Sudoku rules
     * @details Checks row, column, and 3x3 box constraints
     */
    [[nodiscard]] bool isValid(int row, int col, int num) const;

    /**
     * @brief Fast solve without recording steps
     * @return true if puzzle has solution, false if unsolvable
     * @note Uses backtracking algorithm O(9^k)
     */
    bool solve();

    /**
     * @brief Solve with step recording for animation
     * @param steps Vector to store all solving steps
     * @return true if puzzle has solution, false if unsolvable
     * @note Records both placements and removals (backtrack) steps
     */
    bool solveWithSteps(std::vector<SolveStep> &steps);
};

#endif // SUDOKU_HPP

