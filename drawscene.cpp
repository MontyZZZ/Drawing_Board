#include "drawscene.h"
#include <QDebug>
#include <QEvent>
#include <QGraphicsSceneMouseEvent>
#include <QPixmap>
#include <QTextDocument>
#include <QTextCursor>

DrawScene::DrawScene(QObject *parent)
    : QGraphicsScene(parent)
{
    setItemIndexMethod(QGraphicsScene::NoIndex);
    m_isDraw = false;
    m_type = ShapeType::None;
    m_pix = new QGraphicsPixmapItem();
    m_drawPen.setColor(Qt::black);

    //openFile("null");
//    m_rrrect = new QGraphicsRectItem(0,0,150,150);
//    //qDebug() << this->mapToItem(QPoint(111, 0));
//    m_rrrect->setFlag(QGraphicsItem::ItemIsMovable, true);//可以拖动
//    m_rrrect->setFlag(QGraphicsItem::ItemIsSelectable, true);//可以选中
//    addItem(m_rrrect);


//    reect = new Rect(0,0,150,150);
//    addItem(reect);


    m_line = nullptr;

}

DrawScene::~DrawScene()
{

}

void DrawScene::newScene()
{
    clear();
    setBackgroundBrush(QBrush(Qt::white));
}

void DrawScene::openFile(const QString &file)
{
    QPixmap pix;
    pix.load(file);

    m_pix->setPixmap(pix);
    this->addItem(m_pix);
}

void DrawScene::addNewLine(QPointF pt)
{
    m_line = new QGraphicsLineItem;
    m_line->setLine(pt.x(), pt.y(), pt.x(), pt.y());
    QPen pen;
    pen.setColor(m_drawPen.color());
    m_line->setPen(pen);
    addItem(m_line);
}

QGraphicsLineItem *DrawScene::currentLine()
{
    return m_line;
}

void DrawScene::lineAddPt(QPointF pt)
{
    m_line->setLine(m_pressPt.x(), m_pressPt.y(), pt.x(), pt.y());
}

void DrawScene::addNewPen(QPointF pt)
{
    m_penPath.clear();
    m_penPath.moveTo(pt);
    m_pen = new QGraphicsPathItem;
    QPen pen;
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    pen.setWidth(4);
    pen.setColor(m_drawPen.color());
    m_pen->setPen(pen);
    m_pen->setPath(m_penPath);
    addItem(m_pen);
}

QGraphicsPathItem *DrawScene::currentPen()
{
    return m_pen;
}

void DrawScene::penAddPt(QPointF pt)
{
    m_penPath.lineTo(pt);
    m_pen->setPath(m_penPath);
}

void DrawScene::addNewEraser(QPointF pt)
{
    m_eraserPath.clear();
    m_eraserPath.moveTo(pt);
    m_eraser = new QGraphicsPathItem;
    QPen pen;
    pen.setCapStyle(Qt::FlatCap);
    pen.setJoinStyle(Qt::MiterJoin);
    pen.setColor(Qt::white);
    pen.setWidth(6);
    m_eraser->setPen(pen);
    m_eraser->setPath(m_eraserPath);
    addItem(m_eraser);
}

QGraphicsItem *DrawScene::currentEraser()
{
    return m_eraser;
}

void DrawScene::eraserAddPt(QPointF pt)
{
    m_eraserPath.lineTo(pt);
    m_eraser->setPath(m_eraserPath);
}

void DrawScene::addNewText(QPointF pt)
{
    m_text = new drawText();
    //m_text->setFont(myFont);
    m_text->setTextInteractionFlags(Qt::TextEditorInteraction);
    m_text->setZValue(1000.0);
    m_text->setDefaultTextColor(Qt::black);
    m_text->setPos(pt);
    connect(m_text, &drawText::lostFocus,
            this, &DrawScene::editorLostFocus);
//    connect(m_text, &drawText::selectedChange,
//            this, &DrawScene::itemSelected);
    addItem(m_text);
    m_type = ShapeType::None;
    m_isDraw = false;

}

void DrawScene::addNewRect(QPointF pt)
{
    m_rect = new QGraphicsRectItem;
    m_pressPt = pt;
    QPen pen;
    pen.setColor(m_drawPen.color());
    m_rect->setPen(pen);

    addItem(m_rect);
}

QGraphicsRectItem *DrawScene::currentRect()
{
    return m_rect;
}

void DrawScene::addNewEllipse(QPointF pt)
{
    m_ellips = new QGraphicsEllipseItem;
    m_pressPt = pt;
    QPen pen;
    pen.setColor(m_drawPen.color());
    m_ellips->setPen(pen);
    addItem(m_ellips);
}

QGraphicsEllipseItem *DrawScene::currentEllipse()
{
    return m_ellips;
}

void DrawScene::addNewArrow()
{

}

QPointF DrawScene::pressPt()
{
    return m_pressPt;

}

void DrawScene::editorLostFocus(drawText *item)
{
    QTextCursor cursor = item->textCursor();
    cursor.clearSelection();
    item->setTextCursor(cursor);

    if (item->toPlainText().isEmpty()) {
        removeItem(item);
        item->deleteLater();
    }
}

void DrawScene::setType(DrawScene::ShapeType type)
{
    m_type = type;
}

void DrawScene::setPenColor(QColor color)
{
    m_drawPen.setColor(color);
}

QPen DrawScene::getPen()
{
    return m_drawPen;
}

void DrawScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    m_isDraw = true;

    QPointF pt = mouseEvent->scenePos();
    m_pressPt = pt;

    switch (m_type) {
    case ShapeType::Line:
    {
        addNewLine(pt);
        break;
    }
    case ShapeType::Pen:
    {
        addNewPen(pt);
    }
        break;
    case ShapeType::Eraser:
    {
        addNewEraser(pt);
    }
        break;
    case ShapeType::Text:
    {
        addNewText(pt);
    }
        break;
    case ShapeType::Rect:
    {
        addNewRect(pt);
    }
        break;
    case ShapeType::Ellipse:
    {
        addNewEllipse(pt);
    }
        break;
    case ShapeType::Arrow:
    {

    }
        break;
    default:
        break;

    }

    QGraphicsScene::mousePressEvent(mouseEvent);
}

void DrawScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QPointF pt = mouseEvent->scenePos();
    emit statusPos(mouseEvent->scenePos());

    if (!m_isDraw)
        return;

    //QPointF pt = mouseEvent->scenePos();

    switch (m_type) {
    case ShapeType::Line:
    {
        lineAddPt(pt);
    }
        break;
    case ShapeType::Pen:
    {
        penAddPt(pt);
    }
        break;
    case ShapeType::Eraser:
    {
        eraserAddPt(pt);
    }
        break;
    case ShapeType::Rect:
    {
        QRectF rectF;
        if (pt.x() > m_pressPt.x())
        {
            rectF.setTopLeft(m_pressPt);
            rectF.setBottomRight(pt);
        }
        else
        {
            rectF.setTopLeft(pt);
            rectF.setBottomRight(m_pressPt);
        }

        currentRect()->setRect(rectF);
    }
        break;
    case ShapeType::Ellipse:
    {
        QRectF rectF;
        if (pt.x() > m_pressPt.x())
        {
            rectF.setTopLeft(m_pressPt);
            rectF.setBottomRight(pt);
        }
        else
        {
            rectF.setTopLeft(pt);
            rectF.setBottomRight(m_pressPt);
        }

        currentEllipse()->setRect(rectF);
    }
        break;
    case ShapeType::Arrow:
    {

    }
        break;
    default:
        break;
    }
}

void DrawScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    m_isDraw = false;

    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

bool DrawScene::event(QEvent *event)
{
//    qDebug() << event->type();
    QGraphicsScene::event(event);
//    if (event->type() == QEvent::GraphicsSceneMouseMove) {
//        QGraphicsSceneMouseEvent *mouseEvent = static_cast<QGraphicsSceneMouseEvent* >(event);
//        qDebug() << mouseEvent->scenePos();
//        emit statusPos(mouseEvent->scenePos());
//    }
    return true;
}



