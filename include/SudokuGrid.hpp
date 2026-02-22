#ifndef SUDOKUGRID_HPP
#define SUDOKUGRID_HPP

#include <QWidget>
#include <array>
#include <vector>
#include "SudokuCell.hpp"
#include "Sudoku.hpp"

/**
 * @class SudokuGrid
 * @brief 9x9 Sudoku grid widget with interactive UI
 * @details
 *   - Manages 81 SudokuCell widgets
 *   - Handles user input (keyboard and mouse)
 *   - Draws grid lines and 3x3 box separators
 *   - Size: 506x506 pixels with optimized layout
 */
class SudokuGrid : public QWidget {
    Q_OBJECT

    // Type alias for cell array
    using CellArray = std::array<std::array<SudokuCell*, 9>, 9>;

    // Constants for layout
    static constexpr int CELL_SIZE = 56;       ///< Each cell size in pixels
    static constexpr int THIN_GAP = 2;         ///< Gap between cells
    static constexpr int THICK_GAP = 6;        ///< Gap between 3x3 boxes
    static constexpr int PADDING = 6;          ///< Outer padding
    static constexpr int BOX_SPAN = 3 * CELL_SIZE + 2 * THIN_GAP;  ///< 3x3 box width
    static constexpr int CONTENT = 3 * BOX_SPAN + 2 * THICK_GAP;   ///< Content area
    static constexpr int WIDGET_SZ = CONTENT + 2 * PADDING;         ///< Total widget size

    CellArray cells_{};                          ///< 9x9 grid of cells
    std::vector<std::vector<int>> initialBoard_;  ///< Initial puzzle (immutable)
    int selectedRow_ = -1;                       ///< Currently selected row
    int selectedCol_ = -1;                       ///< Currently selected column

    /**
     * @brief Calculate X coordinate for cell
     * @param col Column index (0-8)
     * @return X coordinate in pixels
     */
    [[nodiscard]] static int cellX(int col);

    /**
     * @brief Calculate Y coordinate for cell
     * @param row Row index (0-8)
     * @return Y coordinate in pixels
     */
    [[nodiscard]] static int cellY(int row);

    /**
     * @brief Update highlighted cells for a selection
     * @param row Selected row
     * @param col Selected column
     * @details Highlights same row, column, and 3x3 box
     */
    void updateHighlights(int row, int col);

protected:
    /**
     * @brief Render grid lines and separators
     * @param event Paint event
     */
    void paintEvent(QPaintEvent *event) override;

    /**
     * @brief Handle keyboard input
     * @param event Key event
     */
    void keyPressEvent(QKeyEvent *event) override;

public:
    /**
     * @brief Constructor
     * @param parent Parent widget
     */
    explicit SudokuGrid(QWidget *parent = nullptr);

    /// Destructor
    ~SudokuGrid() override = default;

    /**
     * @brief Load new puzzle board
     * @param board 9x9 board (0 = empty cell)
     */
    void loadBoard(const std::vector<std::vector<int>>& board);

    /**
     * @brief Apply a single solving step
     * @param step Step containing row, col, and value
     */
    void applyStep(const SolveStep &step);

    /**
     * @brief Mark all solved cells with solved state
     */
    void markSolved();

    /**
     * @brief Clear all user inputs (keep given cells)
     */
    void clearUserInput();

    /**
     * @brief Get current board state
     * @return 9x9 board with all cell values
     */
    [[nodiscard]] std::vector<std::vector<int>> getCurrentBoard() const;

private slots:
    /**
     * @brief Handle cell click
     * @param row Row of clicked cell
     * @param col Column of clicked cell
     */
    void onCellClicked(int row, int col);
};

#endif // SUDOKUGRID_HPP

