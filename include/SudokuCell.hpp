#ifndef SUDOKU_CELL_HPP
#define SUDOKU_CELL_HPP

#include <QWidget>

/**
 * @enum CellState
 * @brief Represents the state of a Sudoku cell
 */
enum class CellState {
    Empty, ///< Cell is empty (0)
    Given, ///< Cell was given in initial puzzle
    UserInput, ///< Cell value entered by user
    Solved, ///< Cell value computed by solver
    Backtrack, ///< Cell was removed during backtracking
    Invalid ///< Cell contains invalid value (future use)
};

/**
 * @class SudokuCell
 * @brief Single cell widget in Sudoku grid
 * @details
 *   - Fixed size 56x56 pixels
 *   - Supports visual states (selected, highlighted)
 *   - Emits cellClicked signal on mouse press
 *   - Custom paint for rendering values with colors
 */
class SudokuCell : public QWidget {
    Q_OBJECT

    int row_; ///< Row index (0-8)
    int col_; ///< Column index (0-8)
    int value_; ///< Cell value (0-9)
    CellState state_; ///< Current cell state
    bool selected_; ///< Is cell selected by user
    bool highlighted_; ///< Is cell highlighted (same row/col/box)

protected:
    /**
     * @brief Custom painting for cell rendering
     * @param event Paint event
     */
    void paintEvent(QPaintEvent *event) override;

    /**
     * @brief Handle mouse press to select cell
     * @param event Mouse event
     */
    void mousePressEvent(QMouseEvent *event) override;

public:
    /**
     * @brief Constructor
     * @param row Row index (0-8)
     * @param col Column index (0-8)
     * @param parent Parent widget
     */
    SudokuCell(int row, int col, QWidget *parent = nullptr);

    /// Destructor
    ~SudokuCell() override = default;

    /**
     * @brief Set cell value and state
     * @param val Value (0-9)
     * @param state Cell state
     */
    void setValue(int val, CellState state);

    /**
     * @brief Set cell selection state
     * @param sel true if selected
     */
    void setSelected(bool sel);

    /**
     * @brief Set cell highlight state
     * @param h true if highlighted
     */
    void setHighlighted(bool h);

    /// Get current cell value
    [[nodiscard]] int value() const { return value_; }

    /// Get current cell state
    [[nodiscard]] CellState state() const { return state_; }

    /// Get row index
    [[nodiscard]] int row() const { return row_; }

    /// Get column index
    [[nodiscard]] int col() const { return col_; }

signals:
    /**
     * @brief Emitted when cell is clicked
     * @param row Row index
     * @param col Column index
     */
    void cellClicked(int row, int col);
};

#endif // SUDOKU_CELL_HPP

