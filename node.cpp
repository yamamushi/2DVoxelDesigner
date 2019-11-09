#include "node.h"

Node::Node():
    m_TextItem(nullptr)
{
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);

    m_isReactorNode = false;
    m_movableRangeFactor = 1.5;
    m_scaleFactor = 1;
    m_spacingUnit = 10;
    m_spacingSize = 32;
    m_centralPos = QPoint(INT_MIN,INT_MIN);
    m_color = Qt::blue;

    // Draw Item
    setPen(QPen(Qt::transparent));
    setRect(-3, - 3, 6, 6);
    setSelectionState(false);

    m_historyPositionBefore.clear();
    m_historyPositionAfter.clear();
    m_historyPositionBefore.push_back(QPoint(0,0));
}

Node::~Node()
{

}

void Node::setCentralPos(QPoint pt){
    m_centralPos = pt / m_spacingUnit;
}

QPoint Node::getCentralPos(){
    return m_centralPos;
}

QPoint Node::getLocalPos(){
    if(m_centralPos == QPoint(INT_MIN,INT_MIN))
        m_centralPos = pos().toPoint() / m_spacingUnit;

    QPoint pt = pos().toPoint() / m_spacingUnit;
    return pt - m_centralPos;
}

void Node::setLocalPos(QPoint pt){
    if(m_centralPos == QPoint(INT_MIN,INT_MIN))
        m_centralPos = pos().toPoint() / m_spacingUnit;

    setPos((pt + m_centralPos) * m_spacingUnit);
}

void Node::resetPosition(){
    setLocalPos(QPoint(0,0));
//    m_historyPositionBefore.clear();
//    m_historyPositionAfter.clear();
//    m_historyPositionBefore.push_back(QPoint(0,0));

    m_historyPositionBefore.push_back(QPoint(0,0));
    m_historyPositionAfter.clear();
    if(m_historyPositionBefore.length() > 30)
        m_historyPositionBefore.pop_front();
}

void Node::undo(){
    if(m_historyPositionBefore.length() > 1){
        QPoint pt = m_historyPositionBefore.last();
        m_historyPositionBefore.pop_back();
        m_historyPositionAfter.push_front(pt);
        setLocalPos(m_historyPositionBefore.last());
    }
}

void Node::redo(){
    if(m_historyPositionAfter.length() > 0){
        QPoint pt = m_historyPositionAfter.first();
        qDebug() << pt;
        m_historyPositionAfter.pop_front();
        m_historyPositionBefore.push_back(pt);
        setLocalPos(pt);
    }
}

void Node::setScaleFactor(float s){
    m_scaleFactor = 1 / s;

    if(m_TextItem != nullptr){
        if(m_TextItem->isVisible()) // Selected state
            setScale(m_scaleFactor * 1.4);
    }else
        setScale(m_scaleFactor);
}

void Node::setSpacingSize(float s)
{
    m_spacingSize = s / m_spacingUnit;
}

QVariant Node::itemChange(GraphicsItemChange change, const QVariant &value)
{
    int x,y;

    switch (change) {
    case ItemPositionHasChanged:
        x = getLocalPos().x();
        y = getLocalPos().y();

        if(getLocalPos().x() > m_spacingSize * m_movableRangeFactor)
            x = m_spacingSize * m_movableRangeFactor;
        if(getLocalPos().x() < -m_spacingSize * m_movableRangeFactor)
            x = -m_spacingSize * m_movableRangeFactor;
        if(getLocalPos().y() > m_spacingSize * m_movableRangeFactor)
            y = m_spacingSize * m_movableRangeFactor;
        if(getLocalPos().y() < -m_spacingSize * m_movableRangeFactor)
            y = -m_spacingSize * m_movableRangeFactor;

        if(m_TextItem != nullptr)
            m_TextItem->setHtml("<div style='background-color:yellow;border-color:black;'>" + QString("( %1, %2 )").arg(abs(x)).arg(abs(y)) + "</div>");

        setLocalPos(QPoint(x, y));
        emit nodeChanged(this);

        break;
    default:
        break;
    };

    return QGraphicsItem::itemChange(change, value);
}

void Node::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    setSelectionState(true);

    emit nodePressed(this);
    emit nodeChanged(this);

    QGraphicsItem::mousePressEvent(event);
}

void Node::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    m_historyPositionBefore.push_back(getLocalPos());
    m_historyPositionAfter.clear();
    if(m_historyPositionBefore.length() > 30)
        m_historyPositionBefore.pop_front();

    QGraphicsItem::mouseReleaseEvent(event);
}

void Node::setSelectionState(bool f)
{
    if(f){
        if(m_isReactorNode)
            setBrush(QBrush(Qt::darkGreen));
        else
            setBrush(QBrush(Qt::red));

        if(m_TextItem == nullptr){
            m_TextItem = new QGraphicsTextItem(this);
            m_TextItem->setPos(3, -20);
            m_TextItem->setHtml("<div style='background-color:yellow;border-color:black;'>" + QString("( %1, %2 )").arg(abs(getLocalPos().x())).arg(abs(getLocalPos().y())) + "</div>");
        }

        setTextVisible(true);
        setScale(m_scaleFactor * 1.4);
        setZValue(2);
    }else{
        if(m_isReactorNode){
            setBrush(QBrush(Qt::green));
            setScale(m_scaleFactor * 1.4);
        }else{
            setBrush(QBrush(Qt::blue));
            setScale(m_scaleFactor * 1);
        }

        if(m_TextItem != nullptr)
            delete [] m_TextItem;
        m_TextItem = nullptr;

        setTextVisible(false);
        setZValue(1);
    }
}

void Node::setTextVisible(bool f)
{
    if(m_TextItem != nullptr)
        m_TextItem->setVisible(f);
    if(!f)
        setZValue(1);
}

void Node::setReactor(bool f)
{
    m_isReactorNode = f;
    setSelectionState(f);
}

bool Node::isReactor()
{
    return m_isReactorNode;
}

bool Node::isLinkedNode(Node *n){
    if(m_centralPos.x() != n->getCentralPos().x() &&
            m_centralPos.y() != n->getCentralPos().y())
        return false;

    if(m_centralPos.x() == n->getCentralPos().x()){
        if(abs(m_centralPos.y() - n->getCentralPos().y()) == m_spacingSize)
            return true;
    }else if(m_centralPos.y() == n->getCentralPos().y()){
        if(abs(m_centralPos.x() - n->getCentralPos().x()) == m_spacingSize)
            return true;
    }
    return false;
}
