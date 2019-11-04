#ifndef DRAWTEXT_H
#define DRAWTEXT_H

#include <QGraphicsTextItem>

class drawText : public QGraphicsTextItem
{
     Q_OBJECT
public:
    enum { Type = UserType + 3 };
    drawText(QGraphicsItem *parent = 0);


    int type() const override { return Type; }

signals:
    void lostFocus(drawText *item);
    void selectedChange(QGraphicsItem *item);

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
    void focusOutEvent(QFocusEvent *event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif // DRAWTEXT_H
