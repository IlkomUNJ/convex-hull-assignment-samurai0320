#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "canvas.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private:
    Canvas *canvas;
    QPushButton *slowButton;
    QPushButton *fastButton;
    QPushButton *clearButton;
};

#endif // MAINWINDOW_H
