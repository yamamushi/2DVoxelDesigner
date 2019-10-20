#ifndef NODE_H
#define NODE_H

#include <QObject>
#include <QGraphicsItem>
#include <QPainter>
#include <QDebug>

class Node :  public QObject, public QGraphicsEllipseItem
{
    Q_OBJECT
public:
    Node();
    ~Node();

    void    setCentralPos(QPoint pt);
    QPoint  getCentralPos();
    QPoint  getLocalPos();
    void    setLocalPos(QPoint pt);
    void    setScaleFactor(float s);
    void    setSpacingSize(float s);
    void    setSelectionState(bool f);
    void    setTextVisible(bool f);
    void    setReactor(bool f);
    bool    isReactor();
    bool    isLinkedNode(Node*);

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

private:
    QPoint          m_centralPos;
    float           m_scaleFactor;
    int             m_spacingSize;
    int             m_spacingUnit;
    QGraphicsTextItem *m_TextItem;
    float           m_movableRangeFactor;
    QColor          m_color;
    bool            m_isReactorNode;

signals:
    void nodeChanged(Node*);
    void nodePressed(Node*);

};

#endif // NODE_H
