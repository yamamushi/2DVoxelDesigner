#include "edge.h"

Edge::Edge(Node* n1, Node*n2)
{
    setZValue(1);
    m_startNode = nullptr;
    m_endNode = nullptr;

    m_pen.setColor(Qt::blue);
    m_pen.setStyle(Qt::DashDotLine);
    m_pen.setWidth(1);
    setPen(m_pen);

    if(n1 != nullptr && n2 != nullptr)
    {
        m_startNode = n1;
        m_endNode = n2;
        setLine(m_startNode->pos().x(),
                m_startNode->pos().y(),
                m_endNode->pos().x(),
                m_endNode->pos().y());

        QObject::connect(m_startNode, SIGNAL(nodeChanged(Node*)), this, SLOT(updateNode()));
        QObject::connect(m_endNode, SIGNAL(nodeChanged(Node*)), this, SLOT(updateNode()));
    }
}

void Edge::updateNode()
{
    if(m_startNode != nullptr && m_endNode != nullptr)
        setLine(m_startNode->pos().x(),
                m_startNode->pos().y(),
                m_endNode->pos().x(),
                m_endNode->pos().y());
}

void Edge::setScaleFactor(float s)
{
    int pen_width = (int)(1 / s);
    if(pen_width < 1)
        pen_width = 1;
    if(pen_width != m_pen.width()){
        m_pen.setWidth(pen_width);
        setPen(m_pen);
    }
}

void Edge::setColor(QColor c)
{
    m_pen.setColor(c);
    setPen(m_pen);
}
