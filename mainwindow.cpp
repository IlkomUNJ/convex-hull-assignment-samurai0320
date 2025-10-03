#include "mainwindow.h"
#include "canvas.h"
#include <QGridLayout>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Convex Hull Visualizer");
    resize(950, 650);

    QWidget *container = new QWidget(this);
    QGridLayout *layout = new QGridLayout(container);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->setSpacing(10);


    canvas = new Canvas(this);


    QPushButton *btnConvex = new QPushButton("Convex Hull (Fast)", this);
    QPushButton *btnSlow = new QPushButton("Convex Hull (Slow)", this);
    QPushButton *btnClear = new QPushButton("Reset Canvas", this);


    QString style = "QPushButton { padding: 6px; font-weight: bold; }";
    btnConvex->setStyleSheet(style);
    btnSlow->setStyleSheet(style);
    btnClear->setStyleSheet(style);


    layout->addWidget(canvas, 0, 0, 1, 3);
    layout->addWidget(btnConvex, 1, 0);
    layout->addWidget(btnSlow,   1, 1);
    layout->addWidget(btnClear,  1, 2);

    setCentralWidget(container);


    connect(btnConvex, &QPushButton::clicked, canvas, &Canvas::CalculateConvex);
    connect(btnSlow,   &QPushButton::clicked, canvas, &Canvas::Slow);
    connect(btnClear,  &QPushButton::clicked, canvas, &Canvas::ResetCanvas);
}
