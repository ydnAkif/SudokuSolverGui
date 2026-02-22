#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QTimer>
#include <memory>
#include "Sudoku.hpp"

namespace Ui {
class MainWindow;
}

/**
 * @class MainWindow
 * @brief Main application window
 * @details
 *   - Contains Sudoku grid, buttons, and labels
 *   - Handles solving logic (fast and animated)
 *   - Manages puzzle selection and UI updates
 *   - UI definition is in MainWindow.ui (Qt Designer format)
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

private:
    std::unique_ptr<Ui::MainWindow> ui_;  ///< UI pointer (from .ui file)

    // Solving logic
    QTimer *animTimer_{nullptr};                    ///< Animation timer (60 FPS)
    std::vector<SolveStep> solveSteps_;             ///< Recorded solving steps
    size_t currentStep_{0};                         ///< Current step in animation

    // Puzzle collection
    static const std::vector<std::vector<std::vector<int>>> PUZZLES;  ///< Example puzzles

    /**
     * @brief Solve current puzzle instantly
     * @details Shows solution with elapsed time
     */
    void onSolve() const;

    /**
     * @brief Start animated solving
     * @details Shows step-by-step solving process
     */
    void onAnimate();

    /**
     * @brief Load a random puzzle
     */
    void onNewPuzzle() const;

    /**
     * @brief Clear user input (keep given cells)
     */
    void onClear() const;

    /**
     * @brief Process next animation step
     */
    void onAnimStep();

public:
    /**
     * @brief Constructor
     * @param parent Parent widget
     */
    explicit MainWindow(QWidget *parent = nullptr);

    /// Destructor
    ~MainWindow() override;
};

#endif // MAINWINDOW_HPP

