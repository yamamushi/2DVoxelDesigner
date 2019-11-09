#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    m_node1 = m_node2 = m_node3 = m_node4 = nullptr;

    m_spacingSize = 32;
    m_spacingUnit = 10;
}

Widget::~Widget()
{
    delete ui;
}

void Widget::drawReactorPreview(Node* node1, Node* node2, Node* node3, Node* node4)
{
    if(node1 != nullptr && node2 != nullptr && node3 != nullptr && node4 != nullptr){
        QPoint pt_center1 = node1->getCentralPos();
        QPoint pt_center2 = node2->getCentralPos();
        QPoint pt_center3 = node3->getCentralPos();
        QPoint pt_center4 = node4->getCentralPos();

        int min_x = INT_MAX;
        int max_x = INT_MIN;
        int min_y = INT_MAX;
        int max_y = INT_MIN;

        if(pt_center1.x() < min_x)
            min_x = pt_center1.x();
        if(pt_center1.x() > max_x)
            max_x = pt_center1.x();
        if(pt_center1.y() < min_y)
            min_y = pt_center1.y();
        if(pt_center1.y() < max_y)
            max_y = pt_center1.y();

        if(pt_center2.x() < min_x)
            min_x = pt_center2.x();
        if(pt_center2.x() > max_x)
            max_x = pt_center2.x();
        if(pt_center2.y() < min_y)
            min_y = pt_center2.y();
        if(pt_center2.y() > max_y)
            max_y = pt_center2.y();

        if(pt_center3.x() < min_x)
            min_x = pt_center3.x();
        if(pt_center3.x() > max_x)
            max_x = pt_center3.x();
        if(pt_center3.y() < min_y)
            min_y = pt_center3.y();
        if(pt_center3.y() > max_y)
            max_y = pt_center3.y();

        if(pt_center4.x() < min_x)
            min_x = pt_center4.x();
        if(pt_center4.x() > max_x)
            max_x = pt_center4.x();
        if(pt_center4.y() < min_y)
            min_y = pt_center4.y();
        if(pt_center4.y() > max_y)
            max_y = pt_center4.y();

        if(node1->getCentralPos() == QPoint(min_x, min_y))
            m_node1 = node1;
        else if(node2->getCentralPos() == QPoint(min_x, min_y))
            m_node1 = node2;
        else if(node3->getCentralPos() == QPoint(min_x, min_y))
            m_node1 = node3;
        else if(node4->getCentralPos() == QPoint(min_x, min_y))
            m_node1 = node4;
        else
            m_node1 = nullptr;

        if(node1->getCentralPos() == QPoint(max_x, min_y))
            m_node2 = node1;
        else if(node2->getCentralPos() == QPoint(max_x, min_y))
            m_node2 = node2;
        else if(node3->getCentralPos() == QPoint(max_x, min_y))
            m_node2 = node3;
        else if(node4->getCentralPos() == QPoint(max_x, min_y))
            m_node2 = node4;
        else
            m_node2 = nullptr;

        if(node1->getCentralPos() == QPoint(max_x, max_y))
            m_node3 = node1;
        else if(node2->getCentralPos() == QPoint(max_x, max_y))
            m_node3 = node2;
        else if(node3->getCentralPos() == QPoint(max_x, max_y))
            m_node3 = node3;
        else if(node4->getCentralPos() == QPoint(max_x, max_y))
            m_node3 = node4;
        else
            m_node3 = nullptr;

        if(node1->getCentralPos() == QPoint(min_x, max_y))
            m_node4 = node1;
        else if(node2->getCentralPos() == QPoint(min_x, max_y))
            m_node4 = node2;
        else if(node3->getCentralPos() == QPoint(min_x, max_y))
            m_node4 = node3;
        else if(node4->getCentralPos() == QPoint(min_x, max_y))
            m_node4 = node4;
        else
            m_node4 = nullptr;
    }else{
        m_node1 = m_node2 = m_node3 = m_node4 = nullptr;
    }
    update();
}

void Widget::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    if(m_node1 == nullptr || m_node2 == nullptr || m_node3 == nullptr || m_node4 == nullptr){
        painter.setWindow(0,0,10,10);

        painter.setBrush(QBrush(QColor(182, 182, 182)));
        painter.setPen(Qt::transparent);
        painter.drawRect(0,0,10,10);
        return ;
    }

    // Set window's property
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setWindow(m_node1->getCentralPos().x() * m_spacingUnit - m_spacingSize * m_spacingUnit * 2,
                      m_node1->getCentralPos().y() * m_spacingUnit - m_spacingSize * m_spacingUnit * 2,
                      m_spacingSize * m_spacingUnit * 5,
                      m_spacingSize * m_spacingUnit * 5);

    // Draw background
    painter.setBrush(QBrush(QColor(182, 182, 182)));
    painter.drawRect(m_node1->getCentralPos().x() * m_spacingUnit - m_spacingSize * m_spacingUnit * 2,
                     m_node1->getCentralPos().y() * m_spacingUnit - m_spacingSize * m_spacingUnit * 2,
                     m_spacingSize * m_spacingUnit * 5,
                     m_spacingSize * m_spacingUnit * 5);

    //    qDebug() << m_node1->getCentralPos().x() * m_spacingUnit - m_spacingSize * m_spacingUnit * 2 << ","
    //             << m_node1->getCentralPos().y() * m_spacingUnit - m_spacingSize * m_spacingUnit * 2 << ","
    //             << m_spacingSize * m_spacingUnit * 5 << ","
    //             << m_spacingSize * m_spacingUnit * 5;

    //    qDebug() << m_node1->pos().toPoint() << ","
    //             << m_node2->pos().toPoint() << ","
    //             << m_node3->pos().toPoint() << ","
    //             << m_node4->pos().toPoint() ;

    // Draw polygon of reactor guide with blue color
    painter.setPen(QPen(Qt::transparent));

    painter.setBrush(QBrush(Qt::red));
    QPolygon polygon1;
    polygon1 << m_node1->pos().toPoint()
            << m_node1->getCentralPos() * m_spacingUnit + QPoint(- m_spacingSize * m_spacingUnit, 0)
            << m_node1->getCentralPos() * m_spacingUnit + QPoint(- m_spacingSize * m_spacingUnit, - m_spacingSize * m_spacingUnit)
            << m_node1->getCentralPos() * m_spacingUnit + QPoint(0, - m_spacingSize * m_spacingUnit);
    painter.drawPolygon(polygon1);

    QPolygon polygon2;
    polygon2 << m_node2->pos().toPoint()
            << m_node2->getCentralPos() * m_spacingUnit + QPoint(0, - m_spacingSize * m_spacingUnit)
            << m_node2->getCentralPos() * m_spacingUnit + QPoint(m_spacingSize * m_spacingUnit, - m_spacingSize * m_spacingUnit)
            << m_node2->getCentralPos() * m_spacingUnit + QPoint(m_spacingSize * m_spacingUnit, 0);
    painter.drawPolygon(polygon2);

    QPolygon polygon3;
    polygon3 << m_node3->pos().toPoint()
            << m_node3->getCentralPos() * m_spacingUnit + QPoint(m_spacingSize * m_spacingUnit, 0)
            << m_node3->getCentralPos() * m_spacingUnit + QPoint(m_spacingSize * m_spacingUnit, m_spacingSize * m_spacingUnit)
            << m_node3->getCentralPos() * m_spacingUnit + QPoint(0, m_spacingSize * m_spacingUnit);
    painter.drawPolygon(polygon3);

    QPolygon polygon4;
    polygon4 << m_node4->pos().toPoint()
            << m_node4->getCentralPos() * m_spacingUnit + QPoint(0, m_spacingSize * m_spacingUnit)
            << m_node4->getCentralPos() * m_spacingUnit + QPoint(- m_spacingSize * m_spacingUnit, m_spacingSize * m_spacingUnit)
            << m_node4->getCentralPos() * m_spacingUnit + QPoint(- m_spacingSize * m_spacingUnit, 0);
    painter.drawPolygon(polygon4);

    painter.setBrush(QBrush(Qt::green));
    QPolygon polygon;
    polygon << m_node1->pos().toPoint()
            << m_node2->pos().toPoint()
            << m_node3->pos().toPoint()
            << m_node4->pos().toPoint();
    painter.drawPolygon(polygon);
}
