#include "SudokuCell.hpp"
#include <QPainter>
#include <QMouseEvent>

SudokuCell::SudokuCell(int row, int col, QWidget *parent)
    : QWidget(parent), row_(row), col_(col) {
    setFixedSize(56, 56);
    setCursor(Qt::PointingHandCursor);
    setAttribute(Qt::WA_OpaquePaintEvent);
}

void SudokuCell::setValue(int val, CellState state) {
    value_ = val;
    state_ = state;
    update();
}

void SudokuCell::setSelected(bool sel) {
    if (selected_ == sel) return;
    selected_ = sel;
    update();
}

void SudokuCell::setHighlighted(bool h) {
    if (highlighted_ == h) return;
    highlighted_ = h;
    update();
}

void SudokuCell::paintEvent(QPaintEvent *) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, false);

    // Background color based on state
    QColor bg;
    if (selected_)
        bg = QColor("#FFF176");  // bright yellow - selected
    else if (highlighted_)
        bg = QColor("#E8EAF6");  // light indigo - highlighted
    else
        bg = QColor("#FAFAFA");  // off-white - normal

    p.fillRect(rect(), bg);

    // Draw cell value
    if (value_ != 0) {
        // Static font cache (created once, reused every frame)
        static QFont font = []() {
#if defined(Q_OS_MAC)
            QFont f("SF Pro Text", 21, QFont::Bold);
#elif defined(Q_OS_WIN)
            QFont f("Segoe UI", 21, QFont::Bold);
#else
            QFont f("Ubuntu", 21, QFont::Bold);
#endif
            f.setStyleHint(QFont::SansSerif);
            return f;
        }();
        p.setFont(font);

        // Color based on cell state
        QColor textColor;
        switch (state_) {
            case CellState::Given:
                textColor = QColor("#1565C0");  // deep blue
                break;
            case CellState::UserInput:
                textColor = QColor("#37474F");  // dark blue-gray
                break;
            case CellState::Solved:
                textColor = QColor("#2E7D32");  // deep green
                break;
            case CellState::Backtrack:
                textColor = QColor("#E53935");  // red
                break;
            case CellState::Invalid:
                textColor = QColor("#E53935");  // red
                break;
            default:
                textColor = QColor("#212121");
        }
        p.setPen(textColor);
        p.drawText(rect(), Qt::AlignCenter, QString::number(value_));
    }

    // Light border
    p.setPen(QPen(QColor("#CFD8DC"), 1));
    p.drawRect(0, 0, width() - 1, height() - 1);
}

void SudokuCell::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton)
        emit cellClicked(row_, col_);
}


