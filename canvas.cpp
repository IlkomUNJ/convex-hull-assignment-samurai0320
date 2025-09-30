#include "canvas.h"

static inline double crossProduct(const QPoint &a, const QPoint &b, const QPoint &c) {
    return ( (b.x() - a.x()) * (c.y() - a.y()) ) -
           ( (b.y() - a.y()) * (c.x() - a.x()) );
}

Canvas::Canvas(QWidget *parent)
    : QWidget{parent} {}

void Canvas::resetData(bool clearDots) {
    if (clearDots) {
        dots.clear();
    }
    UpperHull.clear();
    LowerHull.clear();
    FullConvex.clear();
    Edges.clear();
    iteration = 0;
    update();
}

void Canvas::ResetCanvas() {
    resetData(true);
}

void Canvas::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        dots.push_back(event->pos());
        resetData(false);
    }
}


void Canvas::Slow() {
    Edges.clear();
    FullConvex.clear();
    iteration = 0;

    if (dots.size() < 3) {
        FullConvex = dots;
        update();
        return;
    }

    QVector<QPoint> hullPoints;

    for (int i = 0; i < dots.size(); i++) {
        for (int j = 0; j < dots.size(); j++) {
            if (i == j) continue;
            QPoint p = dots[i];
            QPoint q = dots[j];

            bool isEdge = true;
            for (int k = 0; k < dots.size(); ++k) {
                iteration++;
                if (k == i || k == j) continue;
                QPoint r = dots[k];

                if (((q.x() - p.x()) * (r.y() - p.y()) -
                     (q.y() - p.y()) * (r.x() - p.x())) > 0) {
                    isEdge = false;
                    break;
                }
            }
            if (isEdge) {
                if (!hullPoints.contains(p)) hullPoints.append(p);
                if (!hullPoints.contains(q)) hullPoints.append(q);
                Edges.append(qMakePair(p, q));
            }
        }
    }

    QPoint anchor = *std::min_element(hullPoints.begin(), hullPoints.end(),
                                      [](const QPoint &a, const QPoint &b) {
                                          return (a.x() < b.x()) || (a.x() == b.x() && a.y() < b.y());
                                      });

    std::sort(hullPoints.begin(), hullPoints.end(),
              [anchor](const QPoint &a, const QPoint &b) {
                  int cp = (a.x() - anchor.x()) * (b.y() - anchor.y()) -
                           (a.y() - anchor.y()) * (b.x() - anchor.x());
                  if (cp == 0) {
                      int da = (a.x() - anchor.x()) * (a.x() - anchor.x()) +
                               (a.y() - anchor.y()) * (a.y() - anchor.y());
                      int db = (b.x() - anchor.x()) * (b.x() - anchor.x()) +
                               (b.y() - anchor.y()) * (b.y() - anchor.y());
                      return da < db;
                  }
                  return cp > 0;
              });

    FullConvex = hullPoints;
    update();
}


void Canvas::CalculateConvex() {
    UpperHull.clear();
    LowerHull.clear();
    Edges.clear();
    FullConvex.clear();
    iteration = 0;

    if (dots.size() < 2) {
        return;
    }

    std::sort(dots.begin(), dots.end(), [](const QPoint &p1, const QPoint &p2) {
        if (p1.x() == p2.x())
            return p1.y() < p2.y();
        return p1.x() < p2.x();
    });



    UpperHull.reserve(dots.size());
    UpperHull << dots[0] << dots[1];

    for (int i = 2; i < dots.size(); ++i) {
        UpperHull.append(dots[i]);
        while (UpperHull.size() >= 3) {
            int n = UpperHull.size();
            iteration++;
            if (crossProduct(UpperHull[n - 3], UpperHull[n - 2], UpperHull[n - 1]) <= 0) {
                UpperHull.removeAt(n - 2);
            } else {
                break;
            }
        }
    }

    LowerHull.reserve(dots.size());
    LowerHull << dots.last() << dots[dots.size() - 2];

    for (int i = dots.size() - 3; i >= 0; --i) {
        LowerHull.append(dots[i]);
        while (LowerHull.size() >= 3) {
            iteration++;
            int n = LowerHull.size();
            if (crossProduct(LowerHull[n - 3], LowerHull[n - 2], LowerHull[n - 1]) <= 0) {
                LowerHull.removeAt(n - 2);
            } else {
                break;
            }
        }
    }

    FullConvex = UpperHull;
    for (int i = 1; i + 1 < LowerHull.size(); ++i) {
        FullConvex.append(LowerHull[i]);
    }

    update();
}

void Canvas::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QPen hullPen(Qt::white, 3);
    painter.setPen(hullPen);
    int nHull = FullConvex.size();
    for (int idx = 0; idx < nHull; idx++) {
        QPoint a = FullConvex.at(idx);
        QPoint b = FullConvex.at((idx + 1) % nHull);
        painter.drawLine(a, b);
    }

    QFont font = painter.font();
    font.setPointSize(12);
    font.setBold(true);
    painter.setFont(font);
    painter.drawText(10, 20, "Number of points: " + QString::number(dots.size()));
    painter.drawText(10, 40, "Number of Iterations: " + QString::number(iteration));

    QPen edgePen(Qt::darkGreen, 2, Qt::DashLine);
    painter.setPen(edgePen);
    for (int e = 0; e < Edges.size(); e++) {
        const QPair<QPoint, QPoint> &line = Edges.at(e);
        painter.drawLine(line.first, line.second);
    }

    QPen dotPen(Qt::red, 8, Qt::SolidLine, Qt::RoundCap);
    painter.setPen(dotPen);
    for (QPoint p : dots) {
        painter.drawPoint(p);
    }
}


