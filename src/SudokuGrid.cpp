#include "SudokuGrid.hpp"
#include <QPainter>
#include <QKeyEvent>

// Cell coordinate calculation
// idx ∈ [0,8]
// Every 3 cells form one 3x3 box
// Inner gap: THIN_GAP, box gap: THICK_GAP
int SudokuGrid::cellX(int col) {
    const int box = col / 3;
    const int inBox = col % 3;
    return PADDING + box * (BOX_SPAN + THICK_GAP) + inBox * (CELL_SIZE + THIN_GAP);
}

int SudokuGrid::cellY(int row) {
    const int box = row / 3;
    const int inBox = row % 3;
    return PADDING + box * (BOX_SPAN + THICK_GAP) + inBox * (CELL_SIZE + THIN_GAP);
}

// Constructor
SudokuGrid::SudokuGrid(QWidget *parent) : QWidget(parent) {
    setFixedSize(WIDGET_SZ, WIDGET_SZ);
    setFocusPolicy(Qt::StrongFocus);

    for (size_t r = 0; r < 9; ++r) {
        for (size_t c = 0; c < 9; ++c) {
            auto *cell = new SudokuCell(static_cast<int>(r), static_cast<int>(c), this);
            connect(cell, &SudokuCell::cellClicked, this, &SudokuGrid::onCellClicked);
            cell->move(cellX(static_cast<int>(c)), cellY(static_cast<int>(r)));
            cells_[r][c] = cell;
        }
    }

    loadBoard(Sudoku::DEFAULT_BOARD);
}

// Load new puzzle board
void SudokuGrid::loadBoard(const std::vector<std::vector<int> > &board) {
    initialBoard_ = board;
    selectedRow_ = -1;
    selectedCol_ = -1;

    for (size_t r = 0; r < 9; ++r) {
        for (size_t c = 0; c < 9; ++c) {
            const int val = board[r][c];
            cells_[r][c]->setValue(val, val != 0 ? CellState::Given : CellState::Empty);
            cells_[r][c]->setSelected(false);
            cells_[r][c]->setHighlighted(false);
        }
    }
}

// Apply a solving step
void SudokuGrid::applyStep(const SolveStep &step) {
    auto *cell = cells_[static_cast<size_t>(step.row)][static_cast<size_t>(step.col)];
    if (step.value == 0) {
        cell->setValue(0, CellState::Empty);
    } else {
        // Don't override given cell state
        const bool given = (initialBoard_[static_cast<size_t>(step.row)][static_cast<size_t>(step.col)] != 0);
        cell->setValue(step.value, given ? CellState::Given : CellState::Solved);
    }
}

// Mark all solved cells
void SudokuGrid::markSolved() {
    for (size_t r = 0; r < 9; ++r)
        for (size_t c = 0; c < 9; ++c)
            if (cells_[r][c]->state() != CellState::Given && cells_[r][c]->value() != 0)
                cells_[r][c]->setValue(cells_[r][c]->value(), CellState::Solved);
}

// Clear user inputs
void SudokuGrid::clearUserInput() {
    selectedRow_ = -1;
    selectedCol_ = -1;
    for (size_t r = 0; r < 9; ++r) {
        for (size_t c = 0; c < 9; ++c) {
            cells_[r][c]->setSelected(false);
            cells_[r][c]->setHighlighted(false);
            if (initialBoard_[r][c] == 0)
                cells_[r][c]->setValue(0, CellState::Empty);
        }
    }
}

// Get current board state
std::vector<std::vector<int> > SudokuGrid::getCurrentBoard() const {
    std::vector<std::vector<int> > board(9, std::vector<int>(9, 0));
    for (size_t r = 0; r < 9; ++r)
        for (size_t c = 0; c < 9; ++c)
            board[r][c] = cells_[r][c]->value();
    return board;
}

// Handle cell click
void SudokuGrid::onCellClicked(int row, int col) {
    // Remove previous selection
    if (selectedRow_ != -1)
        cells_[static_cast<size_t>(selectedRow_)][static_cast<size_t>(selectedCol_)]->setSelected(false);

    // Clear all highlights
    for (size_t r = 0; r < 9; ++r)
        for (size_t c = 0; c < 9; ++c)
            cells_[r][c]->setHighlighted(false);

    selectedRow_ = row;
    selectedCol_ = col;
    cells_[static_cast<size_t>(row)][static_cast<size_t>(col)]->setSelected(true);

    updateHighlights(row, col);
    setFocus();
}

// Update highlighted cells
void SudokuGrid::updateHighlights(int row, int col) {
    // Collect cells to highlight
    std::vector<SudokuCell *> toHighlight;
    toHighlight.reserve(27);

    // Same row and column
    for (int i = 0; i < 9; ++i) {
        if (i != col && cells_[static_cast<size_t>(row)][static_cast<size_t>(i)] != nullptr)
            toHighlight.push_back(cells_[static_cast<size_t>(row)][static_cast<size_t>(i)]);
        if (i != row && cells_[static_cast<size_t>(i)][static_cast<size_t>(col)] != nullptr)
            toHighlight.push_back(cells_[static_cast<size_t>(i)][static_cast<size_t>(col)]);
    }

    // Same 3x3 box
    const int br = (row / 3) * 3;
    const int bc = (col / 3) * 3;
    for (int r = br; r < br + 3; ++r) {
        for (int c = bc; c < bc + 3; ++c) {
            if ((r != row || c != col) && cells_[static_cast<size_t>(r)][static_cast<size_t>(c)] != nullptr)
                toHighlight.push_back(cells_[static_cast<size_t>(r)][static_cast<size_t>(c)]);
        }
    }

    // Batch update
    for (auto *cell: toHighlight) {
        cell->setHighlighted(true);
    }
}

// Handle keyboard input
void SudokuGrid::keyPressEvent(QKeyEvent *event) {
    if (selectedRow_ == -1) return;

    auto *cell = cells_[static_cast<size_t>(selectedRow_)][static_cast<size_t>(selectedCol_)];
    const int key = event->key();

    // 1-9: number input (don't touch given cells)
    if (key >= Qt::Key_1 && key <= Qt::Key_9) {
        if (cell->state() != CellState::Given)
            cell->setValue(key - Qt::Key_0, CellState::UserInput);
    }
    // Delete
    else if (key == Qt::Key_Backspace || key == Qt::Key_Delete) {
        if (cell->state() != CellState::Given)
            cell->setValue(0, CellState::Empty);
    }
    // Arrow navigation
    else if (key == Qt::Key_Up && selectedRow_ > 0) onCellClicked(selectedRow_ - 1, selectedCol_);
    else if (key == Qt::Key_Down && selectedRow_ < 8) onCellClicked(selectedRow_ + 1, selectedCol_);
    else if (key == Qt::Key_Left && selectedCol_ > 0) onCellClicked(selectedRow_, selectedCol_ - 1);
    else if (key == Qt::Key_Right && selectedCol_ < 8) onCellClicked(selectedRow_, selectedCol_ + 1);
}

// Paint grid lines and separators
void SudokuGrid::paintEvent(QPaintEvent *) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, false);

    // General background (light gray for thin gaps)
    p.fillRect(rect(), QColor("#B0BEC5"));

    // 3x3 box separator color (dark indigo)
    const QColor boxSep("#3949AB");

    // Vertical box separators (between columns 2-3 and 5-6)
    for (int i = 1; i <= 2; ++i) {
        const int xSep = cellX(i * 3) - THICK_GAP;
        p.fillRect(xSep, PADDING, THICK_GAP, CONTENT, boxSep);
    }

    // Horizontal box separators (between rows 2-3 and 5-6)
    for (int i = 1; i <= 2; ++i) {
        const int ySep = cellY(i * 3) - THICK_GAP;
        p.fillRect(PADDING, ySep, CONTENT, THICK_GAP, boxSep);
    }

    // Outer border
    p.setPen(QPen(boxSep, 3));
    p.drawRect(2, 2, WIDGET_SZ - 5, WIDGET_SZ - 5);
}
