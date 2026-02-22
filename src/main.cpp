#include <QApplication>
#include "MainWindow.hpp"

/**
 * @brief Application entry point
 * @param argc Argument count
 * @param argv Argument values
 * @return Application exit code
 *
 * Initializes Qt application with Sudoku Solver window and starts event loop
 */
int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName("Sudoku Solver");
    app.setApplicationVersion("2.0");

    MainWindow window;
    window.show();

    return app.exec();
}
