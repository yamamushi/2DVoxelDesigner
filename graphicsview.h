#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QObject>
#include <QGraphicsView>
#include <QKeyEvent>
#include <QPixmap>
#include <QMessageBox>
#include <QScrollBar>
#include <QDebug>

#include "node.h"
#include "edge.h"
#include "imageitem.h"

class GraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    GraphicsView(QWidget *parent = 0);
    ~GraphicsView();

    // EventHandler from MainWindow
    void onNew(int gridSize = 30, int spacingSize = 32);
    void onSave(QString);
    void onOpen(QString);
    void onLoadImage(QString);
    void onClearImage();
    void onUpdateLineColor(QColor);
    void onResetPoint();
    void onUndo();
    void onRedo();

    // Set workspace-property
    int getSpacingSize(){ return m_spacingSize / m_spacingUnit; }
    int getSpacingUnit(){ return m_spacingUnit; }

public slots:
    // From node-signal
    void nodePressed(Node*);
    void nodeChanged(Node*);

    // From MainWindow-signal
    void onReactorGuide(bool);
    void onMaintainAspectRatio(bool);
    void onLockImage(bool);
    void onShowImage(bool);

private:
    void scaleView(qreal);
    void initGraphicsItems();
    void drawGraphicsItems();

protected:
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent* event) override;
    void drawBackground(QPainter *painter, const QRectF &rect) override;

signals:
    // To ReactorPreviewWidget
    void reactorChanged(Node*, Node*, Node*, Node*);

private:
    // Graphics Properties
    float           m_scaleFactor;
    int             m_gridSize;
    int             m_spacingSize;
    int             m_spacingUnit;
    QColor          m_lineColor;

    // Graphics Items
    QList<Node*>    m_nodeList;
    QList<Edge*>    m_edgeList;
    ImageItem*      m_imgItem;
    QString         m_imageFileName;
    Node*           m_selectedNode;
    QList<Node*>    m_reactorNodes;

    // Image Mode
    bool            m_imageMaintainAspectRatio;
    bool            m_imageLocked;
    bool            m_imageShown;

    // Reactor Guide
    bool            m_reactorMode;

    // Pan Scrolling
    bool            m_panScrollMode;
    int             m_panStartX, m_panStartY;

    QList<QGraphicsItem*>   m_historiesBefore;
    QList<QGraphicsItem*>   m_historiesAfter;
};

#endif // GRAPHICSVIEW_H
