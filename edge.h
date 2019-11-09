#ifndef EDGE_H
#define EDGE_H

#include <QObject>
#include <QGraphicsItem>
#include <node.h>

class Node;

class Edge : public QObject, public QGraphicsLineItem
{
    Q_OBJECT
public:
    Edge(Node* n1, Node*n2);
    void setScaleFactor(float s);
    void setColor(QColor c);

public slots:
    void updateNode();
private:
    Node*   m_startNode;
    Node*   m_endNode;
    QPen    m_pen;
};

#endif // EDGE_H
