#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPainter>

#include "node.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();
    void setSpacingSize(int s){ m_spacingSize = s; }
    void setSpacingUnit(int s){ m_spacingUnit = s; }
public slots:
    void drawReactorPreview(Node* node1 = nullptr, Node* node2 = nullptr, Node* node3 = nullptr, Node* node4 = nullptr);
protected:
    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;

private:
    Ui::Widget *ui;

    int             m_spacingSize;
    int             m_spacingUnit;
    Node            *m_node1, *m_node2, *m_node3, *m_node4;
};

#endif // WIDGET_H
