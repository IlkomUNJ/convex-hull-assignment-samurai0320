#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QVector>
#include <QPair>

class Canvas : public QWidget
{
    Q_OBJECT
public:
    explicit Canvas(QWidget *parent = nullptr);

    void ResetCanvas();
    void Slow();
    void CalculateConvex();

protected:

    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:

    void resetData(bool clearDots = false);


    bool value = true;
    int iteration = 0;
    QVector<QPoint> dots;
    QVector<QPoint> UpperHull;
    QVector<QPoint> LowerHull;
    QVector<QPoint> FullConvex;
    QVector<QPair<QPoint, QPoint>> Edges;
};

#endif // CANVAS_H

