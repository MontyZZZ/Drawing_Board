#ifndef DRAWSCENE_H
#define DRAWSCENE_H

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsItem>

#include "drawtext.h"

class DrawScene : public QGraphicsScene
{
     Q_OBJECT
public:
    enum ShapeType
    {
        None,
        Line,
        Pen,
        Eraser,
        Text,
        Rect,
        Ellipse,
        Arrow
    };

    DrawScene(QObject *parent = nullptr);
    ~DrawScene();

    void newScene();
    void openFile(const QString& file);


    void addNewLine(QPointF pt);
    QGraphicsLineItem* currentLine();
    void lineAddPt(QPointF pt);

    void addNewPen(QPointF pt);
    QGraphicsPathItem* currentPen();
    void penAddPt(QPointF pt);


    void addNewEraser(QPointF pt);
    QGraphicsItem* currentEraser();
    void eraserAddPt(QPointF pt);

    void addNewText(QPointF pt);

    void addNewRect(QPointF pt);
    QGraphicsRectItem* currentRect();

    void addNewEllipse(QPointF pt);
    QGraphicsEllipseItem* currentEllipse();
    void addNewArrow();


    QPointF pressPt();

    void editorLostFocus(drawText *item);

    void setType(ShapeType type);

    void setPenColor(QColor color);

    QPen getPen();
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) override;

    bool event(QEvent *event) override;

signals:
    void statusPos(QPointF pt);


private:
    bool                   m_isDraw;
    ShapeType              m_type;
    QGraphicsPixmapItem*   m_pix;
    QGraphicsRectItem*     m_rrrect;

    //Rect* reect;

    QGraphicsLineItem*    m_line;
    QGraphicsPathItem*    m_pen;
    //eraserItem*          m_eraser;
    QGraphicsPathItem*    m_eraser;

    QPainterPath          m_penPath;
    QPainterPath          m_eraserPath;

    drawText*    m_text;
    QGraphicsRectItem*    m_rect;
    QGraphicsEllipseItem* m_ellips;

    QPointF               m_pressPt;

    QPen                  m_drawPen;

};

#endif // DRAWSCENE_H
