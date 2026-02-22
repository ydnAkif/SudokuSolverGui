#include "MainWindow.hpp"
#include "ui_MainWindow.h"
#include <QElapsedTimer>
#include <random>

// Puzzle Collection - 5 example puzzles
const std::vector<std::vector<std::vector<int>>> MainWindow::PUZZLES = {
    // ...existing puzzles...
    {
        {8, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 3, 6, 0, 0, 0, 0, 0},
        {0, 7, 0, 0, 9, 0, 2, 0, 0},
        {0, 5, 0, 0, 0, 7, 0, 0, 0},
        {0, 0, 0, 0, 4, 5, 7, 0, 0},
        {0, 0, 0, 1, 0, 0, 0, 3, 0},
        {0, 0, 1, 0, 0, 0, 0, 6, 8},
        {0, 0, 8, 5, 0, 0, 0, 1, 0},
        {0, 9, 0, 0, 0, 0, 4, 0, 0}
    },
    {
        {5, 3, 0, 0, 7, 0, 0, 0, 0},
        {6, 0, 0, 1, 9, 5, 0, 0, 0},
        {0, 9, 8, 0, 0, 0, 0, 6, 0},
        {8, 0, 0, 0, 6, 0, 0, 0, 3},
        {4, 0, 0, 8, 0, 3, 0, 0, 1},
        {7, 0, 0, 0, 2, 0, 0, 0, 6},
        {0, 6, 0, 0, 0, 0, 2, 8, 0},
        {0, 0, 0, 4, 1, 9, 0, 0, 5},
        {0, 0, 0, 0, 8, 0, 0, 7, 9}
    },
    {
        {0, 0, 0, 2, 6, 0, 7, 0, 1},
        {6, 8, 0, 0, 7, 0, 0, 9, 0},
        {1, 9, 0, 0, 0, 4, 5, 0, 0},
        {8, 2, 0, 1, 0, 0, 0, 4, 0},
        {0, 0, 4, 6, 0, 2, 9, 0, 0},
        {0, 5, 0, 0, 0, 3, 0, 2, 8},
        {0, 0, 9, 3, 0, 0, 0, 7, 4},
        {0, 4, 0, 0, 5, 0, 0, 3, 6},
        {7, 0, 3, 0, 1, 8, 0, 0, 0}
    },
    {
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 3, 0, 8, 5},
        {0, 0, 1, 0, 2, 0, 0, 0, 0},
        {0, 0, 0, 5, 0, 7, 0, 0, 0},
        {0, 0, 4, 0, 0, 0, 1, 0, 0},
        {0, 9, 0, 0, 0, 0, 0, 0, 0},
        {5, 0, 0, 0, 0, 0, 0, 7, 3},
        {0, 0, 2, 0, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 4, 0, 0, 0, 9}
    },
    {
        {0, 2, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 6, 0, 0, 0, 0, 3},
        {0, 7, 4, 0, 8, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 3, 0, 0, 2},
        {0, 8, 0, 0, 4, 0, 0, 1, 0},
        {6, 0, 0, 5, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 0, 7, 8, 0},
        {5, 0, 0, 0, 0, 9, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 4, 0}
    }
};

// Constructor - Initialize UI and connect signals
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui_(std::make_unique<Ui::MainWindow>()) {
    ui_->setupUi(this);

    // Set up animation timer for 60 FPS
    animTimer_ = new QTimer(this);
    animTimer_->setInterval(16);  // 60 FPS (16ms per frame)
    connect(animTimer_, &QTimer::timeout, this, &MainWindow::onAnimStep);

    // Connect button signals to slots
    connect(ui_->solveBtn, &QPushButton::clicked, this, &MainWindow::onSolve);
    connect(ui_->animateBtn, &QPushButton::clicked, this, &MainWindow::onAnimate);
    connect(ui_->newPuzzleBtn, &QPushButton::clicked, this, &MainWindow::onNewPuzzle);
    connect(ui_->clearBtn, &QPushButton::clicked, this, &MainWindow::onClear);
}

// Destructor
MainWindow::~MainWindow() = default;

// Fast Solve - Instantly solve current puzzle and display result
void MainWindow::onSolve() const {
    // Stop any running animation
    if (animTimer_->isActive()) {
        animTimer_->stop();
        ui_->animateBtn->setText("🎬  Animasyon");
    }

    // Get current board and solve it (const reference to avoid copy)
    const auto& board = ui_->grid->getCurrentBoard();
    Sudoku sudoku(board);

    // Measure solving time
    QElapsedTimer et;
    et.start();
    const bool solved = sudoku.solve();
    const qint64 elapsed = et.elapsed();

    if (solved) {
        // Apply solved cells to grid
        for (size_t r = 0; r < 9; ++r)
            for (size_t c = 0; c < 9; ++c)
                if (board[r][c] == 0)
                    ui_->grid->applyStep({static_cast<int>(r), static_cast<int>(c), sudoku.getCell(static_cast<int>(r), static_cast<int>(c))});

        // Display timing
        if (elapsed == 0)
            ui_->timeLabel->setText("⏱  < 1 ms");
        else
            ui_->timeLabel->setText(QString("⏱  %1 ms").arg(elapsed));
        ui_->statusLabel->setText("✅ Çözüldü!");
    } else {
        ui_->statusLabel->setText("❌ Çözülemez (girişi kontrol et)");
    }
}

// Animated Solve - Show step-by-step solving process
void MainWindow::onAnimate() {
    // If already animating, pause it
    if (animTimer_->isActive()) {
        animTimer_->stop();
        ui_->animateBtn->setText("🎬  Animasyon");
        ui_->statusLabel->setText("⏸ Duraklatıldı");
        return;
    }

    // Get current board and solve with step tracking (const reference)
    const auto& board = ui_->grid->getCurrentBoard();
    Sudoku sudoku(board);

    const bool solvable = sudoku.solveWithSteps(solveSteps_);
    if (!solvable) {
        ui_->statusLabel->setText("❌ Çözülemez");
        return;
    }

    // Reset board and start animation
    ui_->grid->loadBoard(board);
    currentStep_ = 0;

    animTimer_->start();
    ui_->animateBtn->setText("⏹  Durdur");
    ui_->statusLabel->setText(QString("🎬 Adım: 1/%1").arg(solveSteps_.size()));
}

// Animation Step - Process next step in animation
void MainWindow::onAnimStep() {
    // Check if animation is complete
    if (currentStep_ >= solveSteps_.size()) {
        animTimer_->stop();
        ui_->grid->markSolved();
        ui_->animateBtn->setText("🎬  Animasyon");
        ui_->statusLabel->setText("✅ Tamamlandı!");
        ui_->timeLabel->setText(QString("⏱  %1 adım").arg(solveSteps_.size()));
        return;
    }

    // Apply current step
    ui_->grid->applyStep(solveSteps_[currentStep_++]);
    ui_->statusLabel->setText(QString("🎬 Adım: %1/%2").arg(currentStep_).arg(solveSteps_.size()));
}

// New Puzzle - Load random example puzzle
void MainWindow::onNewPuzzle() const {
    // Stop any running animation
    if (animTimer_->isActive()) {
        animTimer_->stop();
        ui_->animateBtn->setText("🎬  Animasyon");
    }

    // Select and load random puzzle
    static std::mt19937 rng{std::random_device{}()};
    std::uniform_int_distribution<size_t> dist(0, PUZZLES.size() - 1);
    ui_->grid->loadBoard(PUZZLES[dist(rng)]);

    ui_->timeLabel->setText("⏱  Süre: —");
    ui_->statusLabel->setText("🎲 Yeni puzzle yüklendi");
}

// Clear - Remove all user inputs (keep given cells)
void MainWindow::onClear() const {
    // Stop any running animation
    if (animTimer_->isActive()) {
        animTimer_->stop();
        ui_->animateBtn->setText("🎬  Animasyon");
    }

    // Clear user input
    ui_->grid->clearUserInput();
    ui_->timeLabel->setText("⏱  Süre: —");
    ui_->statusLabel->setText("🗑 Girişler temizlendi");
}

