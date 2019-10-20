#include "graphicsview.h"

GraphicsView::GraphicsView(QWidget *parent)
    : QGraphicsView(parent)
{
    // Set background of drawing-panel
    setStyleSheet("background-color:rgb(210,210,210);");

    // Graphics Properties
    m_scaleFactor = 1;
    m_spacingUnit = 10;
    m_gridSize = 30;                                    // Default : 30 * 30
    m_spacingSize = 32 * m_spacingUnit;                 // Default : 32 * 32
    m_lineColor = Qt::blue;

    m_imgItem = nullptr;
    initGraphicsItems();

    // Image Mode
    m_imageMaintainAspectRatio = true;
    m_imageLocked = false;
    m_imageShown = true;

    // Reactor Guide
    m_reactorMode = false;

    // Pan Scrolling
    m_panScrollMode = false;

    // Create graphics scene
    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    setScene(scene);
    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);

    // Create new workspace
    onNew();

    // Set initial aspect ratio
    scaleView(0.08);
}

GraphicsView::~GraphicsView(){

}

void GraphicsView::initGraphicsItems()
{
    // Graphics Items
    m_nodeList.clear();
    m_edgeList.clear();
    onClearImage();
    m_selectedNode = nullptr;
    m_reactorNodes.clear();
}

void GraphicsView::wheelEvent(QWheelEvent* event)
{
    if (event->modifiers().testFlag(Qt::ControlModifier))
    {
        return QGraphicsView::wheelEvent(event);
    }else if (event->modifiers().testFlag(Qt::ShiftModifier))
    {
        return QGraphicsView::wheelEvent(event);
    }else if (event->modifiers().testFlag(Qt::AltModifier))
    {
        return QGraphicsView::wheelEvent(event);
    }else{
        // Do a wheel-based zoom about the cursor position
        double angle = event->angleDelta().y();
        double factor = pow(1.0015, angle);

        auto targetViewportPos = event->pos();
        auto targetScenePos = mapToScene(event->pos());

        //scale(factor, factor);
        scaleView(factor);
        centerOn(targetScenePos);
        QPointF deltaViewportPos = targetViewportPos - QPointF(viewport()->width() / 2.0, viewport()->height() / 2.0);
        QPointF viewportCenter = mapFromScene(targetScenePos) - deltaViewportPos;
        centerOn(mapToScene(viewportCenter.toPoint()));
    }
}

void GraphicsView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        QGraphicsItem* item = itemAt(event->pos());
        if(item != nullptr){
            if(!item->flags().testFlag(QGraphicsItem::ItemIsMovable)){
                m_panScrollMode = true;
                m_panStartX = event->x();
                m_panStartY = event->y();
                setCursor(Qt::ClosedHandCursor);
            }
        }else {
            m_panScrollMode = true;
            m_panStartX = event->x();
            m_panStartY = event->y();
            setCursor(Qt::ClosedHandCursor);
        }
    }
    QGraphicsView::mousePressEvent(event);
}

void GraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && m_panScrollMode)
    {
        m_panScrollMode = false;
        setCursor(Qt::ArrowCursor);
    }
    QGraphicsView::mouseReleaseEvent(event);
}

void GraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    if (m_panScrollMode)
    {
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - (event->x() - m_panStartX));
        verticalScrollBar()->setValue(verticalScrollBar()->value() - (event->y() - m_panStartY));
        m_panStartX = event->x();
        m_panStartY = event->y();
    }
    QGraphicsView::mouseMoveEvent(event);

}

void GraphicsView::drawBackground(QPainter *painter, const QRectF &rect)
{
    QRectF sceneRect = this->sceneRect();

    QRect viewport_rect(0, 0, viewport()->width(), viewport()->height());
    QRectF visible_scene_rect = mapToScene(viewport_rect).boundingRect();

    painter->fillRect(visible_scene_rect,QBrush(QColor(210, 210, 210)));

    if(m_scaleFactor > 0.2)
    {
        QPen pen(Qt::lightGray);
        pen.setStyle(Qt::DashDotDotLine);
        painter->setPen(pen);

        for (int x=0; x<=sceneRect.right(); x += m_spacingUnit){
            if(x == 0){
                if(x > visible_scene_rect.left() && x < visible_scene_rect.right())
                    painter->drawLine(x, sceneRect.top(), x, sceneRect.bottom());
            }else{
                if(x > visible_scene_rect.left() && x < visible_scene_rect.right())
                    painter->drawLine(x, sceneRect.top(), x, sceneRect.bottom());
                if(-x > visible_scene_rect.left() && -x < visible_scene_rect.right())
                    painter->drawLine(-x, sceneRect.top(), -x, sceneRect.bottom());
            }
        }

        for (int y=0; y<=sceneRect.bottom(); y += m_spacingUnit){
            if(y == 0){
                if(y > visible_scene_rect.top() && y < visible_scene_rect.bottom())
                    painter->drawLine(sceneRect.left(), y, sceneRect.right(), y);
            }else{
                if(y > visible_scene_rect.top() && y < visible_scene_rect.bottom())
                    painter->drawLine(sceneRect.left(), y, sceneRect.right(), y);
                if(-y > visible_scene_rect.top() && -y < visible_scene_rect.bottom())
                    painter->drawLine(sceneRect.left(), -y, sceneRect.right(), -y);
            }
        }
    }
}

void GraphicsView::scaleView(qreal scaleFactor)
{
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.025 || factor > 2)
        return;

    m_scaleFactor = factor;

    // Scale graphics items
    for(int i = 0 ; i < m_nodeList.count(); i++){
        m_nodeList.at(i)->setScaleFactor(m_scaleFactor);
    }
    for(int i = 0 ; i < m_edgeList.count(); i++){
        m_edgeList.at(i)->setScaleFactor(m_scaleFactor);
    }
    if(m_imgItem != nullptr)
        m_imgItem->setScaleFactor(m_scaleFactor);

    scale(scaleFactor, scaleFactor);
}

void GraphicsView::onNew(int gridSize, int spacingSize){

    // Set properties
    m_gridSize = gridSize;
    m_spacingSize = spacingSize * m_spacingUnit;

    // Initialize all graphics variables
    initGraphicsItems();

    // Clear graphcis scene (Removed all graphics items)
    scene()->clear();
    scene()->setSceneRect(-m_gridSize * m_spacingSize / 2, -m_gridSize * m_spacingSize / 2, m_gridSize * m_spacingSize, m_gridSize * m_spacingSize);

    // Setup node data
    for (int x = -m_gridSize/ 2; x <= m_gridSize/ 2; x ++){
        for (int y = -m_gridSize/ 2; y <= m_gridSize/ 2; y ++){

            Node* node = new Node();
            int x_pos = x * m_spacingSize;
            int y_pos = y * m_spacingSize;

            node->setPos(x_pos,y_pos);
            node->setCentralPos(QPoint(x_pos, y_pos));
            node->setSpacingSize(m_spacingSize);
            m_nodeList << node;

            QObject::connect(node, SIGNAL(nodePressed(Node*)), this, SLOT(nodePressed(Node*)));
            QObject::connect(node, SIGNAL(nodeChanged(Node*)), this, SLOT(nodeChanged(Node*)));
        }
    }

    drawGraphicsItems();
}

void GraphicsView::onSave(QString fn){
    QFile file(fn);
    file.open(QIODevice::WriteOnly);

    if(file.isOpen())
    {
        QTextStream outStream(&file);
        outStream << m_gridSize << endl;
        outStream << m_spacingSize << endl;
        outStream << m_nodeList.count() << endl;

        foreach(Node* node, m_nodeList){
            outStream << node->getLocalPos().x() << " " << node->getLocalPos().y() << endl;
        }

        if(m_imgItem != nullptr){
            outStream << m_imageFileName << endl;
            outStream << m_imgItem->getImageX() << " " << m_imgItem->getImageY() << " " << m_imgItem->getImageWith() << " " << m_imgItem->getImageHeight() << " " << m_imgItem->getImageAngle() << endl;
        }
        file.close();
    }
}

void GraphicsView::onOpen(QString fn){
    QFile file(fn);
    file.open(QIODevice::ReadOnly);

    if(file.isOpen())
    {
        initGraphicsItems();
        scene()->clear();

        int cnt, local_x, local_y;

        QTextStream inStream(&file);
        inStream >> m_gridSize;
        inStream >> m_spacingSize;
        inStream >> cnt;
        scene()->setSceneRect(-m_gridSize * m_spacingSize / 2, -m_gridSize * m_spacingSize / 2, m_gridSize * m_spacingSize, m_gridSize * m_spacingSize);

        for (int x = -m_gridSize/ 2; x <= m_gridSize/ 2; x ++){
            for (int y = -m_gridSize/ 2; y <= m_gridSize/ 2; y ++){

                inStream >> local_x >> local_y;

                Node* node = new Node();
                int x_pos_origin = x * m_spacingSize;
                int y_pos_origin = y * m_spacingSize;

                int x_pos = x * m_spacingSize + local_x * m_spacingUnit;
                int y_pos = y * m_spacingSize + local_y * m_spacingUnit;

                node->setPos(x_pos,y_pos);
                node->setCentralPos(QPoint(x_pos_origin, y_pos_origin));
                node->setSpacingSize(m_spacingSize);
                m_nodeList << node;

                QObject::connect(node, SIGNAL(nodePressed(Node*)), this, SLOT(nodePressed(Node*)));
                QObject::connect(node, SIGNAL(nodeChanged(Node*)), this, SLOT(nodeChanged(Node*)));
            }
        }

        drawGraphicsItems();

        //inStream >> m_imageFileName;
        m_imageFileName = inStream.readLine();

        // Because of "endl"
        if(m_imageFileName.trimmed().isEmpty())
            m_imageFileName = inStream.readLine();

        if(!m_imageFileName.trimmed().isEmpty()){
            if(QFile::exists(m_imageFileName)){
                int img_x, img_y, img_w, img_h, img_a;
                inStream >> img_x >> img_y >> img_w >> img_h >> img_a;

                QPixmap pixmap(m_imageFileName);
                m_imgItem = new ImageItem(pixmap);
                m_imgItem->setPos(img_x,img_y);
                m_imgItem->setImageSize(img_w, img_h);
                m_imgItem->setImageAngle(img_a);

                m_imgItem->setScaleFactor(m_scaleFactor);

                scene()->addItem(m_imgItem);
            }else{
                QMessageBox::information(this, "Loading a image", "Can't find a file " + m_imageFileName,
                                                QMessageBox::Ok);
            }
        }

        file.close();
    }
}

void GraphicsView::onLoadImage(QString fn){
    onClearImage();

    QPixmap pixmap(fn);
    m_imgItem = new ImageItem(pixmap);
    m_imgItem->setPos(0,0);
    m_imgItem->setScaleFactor(m_scaleFactor);
    m_imgItem->lockImage(m_imageLocked);
    m_imgItem->maintainAspectRatio(m_imageMaintainAspectRatio);
    m_imgItem->setVisible(m_imageShown);
    scene()->addItem(m_imgItem);

    m_imageFileName = fn;
}

void GraphicsView::onClearImage()
{
    if(m_imgItem != nullptr)
        delete[] m_imgItem;
    m_imgItem = nullptr;
    m_imageFileName = "";
}

void GraphicsView::drawGraphicsItems(){
    int index;
    for(int i = 0 ; i < m_nodeList.count(); i++){
        // Right node
        index = i + 1;
        if(index >= 0 && index < m_nodeList.count() && i % (m_gridSize + 1) != m_gridSize){
            // Create edge-item
            Edge* e = new Edge(m_nodeList.at(i), m_nodeList.at(index));
            e->setScaleFactor(m_scaleFactor);
            e->setColor(m_lineColor);
            m_edgeList << e;
            // Add edge-item in scene
            scene()->addItem(e);
        }

        // Bottom node
        index = i + m_gridSize + 1;
        if(index >= 0 && index < m_nodeList.count()){
            // Create edge-item
            Edge* e = new Edge(m_nodeList.at(i), m_nodeList.at(index));
            e->setScaleFactor(m_scaleFactor);
            e->setColor(m_lineColor);
            m_edgeList << e;
            // Add edge-item in scene
            scene()->addItem(e);
        }

        // Add node-item in scene
        m_nodeList.at(i)->setScaleFactor(m_scaleFactor);
        scene()->addItem(m_nodeList.at(i));
    }
}

void GraphicsView::onUpdateLineColor(QColor color){
    m_lineColor = color;
    for(int i = 0 ; i < m_edgeList.count(); i++){
        m_edgeList.at(i)->setColor(color);
    }
}

void GraphicsView::onMaintainAspectRatio(bool f){
    m_imageMaintainAspectRatio = f;
    if(m_imgItem != nullptr)
        m_imgItem->maintainAspectRatio(m_imageMaintainAspectRatio);
}

void GraphicsView::onLockImage(bool f)
{
    m_imageLocked = f;
    if(m_imgItem != nullptr)
        m_imgItem->lockImage(m_imageLocked);
}

void GraphicsView::onShowImage(bool f)
{
    m_imageShown = f;
    if(m_imgItem != nullptr)
        m_imgItem->setVisible(m_imageShown);
}

void GraphicsView::nodePressed(Node *n)
{
    if(m_reactorMode){
        // Reactor Mode
        if(m_reactorNodes.length() < 4){
            if(m_reactorNodes.length() > 0){
                if(m_reactorNodes.last() != nullptr){
                    if(!m_reactorNodes.last()->isLinkedNode(n)){
                        for(int i = 0 ; i < m_reactorNodes.length(); i++){
                            if(m_reactorNodes.at(i) != nullptr)
                                m_reactorNodes.at(i)->setReactor(false);
                        }
                        m_reactorNodes.clear();
                    }
                }
            }
            // Reactor Node
            m_reactorNodes << n;
            n->setReactor(true);
        }
    }

    if(m_selectedNode != n)
        if(m_selectedNode != nullptr)
            m_selectedNode->setSelectionState(false);
    m_selectedNode = n;
}

void GraphicsView::nodeChanged(Node *n)
{
    if(n != nullptr){
        if(n->isReactor()){
            if(m_reactorNodes.length() == 4)
            {
                emit reactorChanged(m_reactorNodes.at(0), m_reactorNodes.at(1), m_reactorNodes.at(2), m_reactorNodes.at(3));
            }
        }
    }
}

void GraphicsView::onReactorGuide(bool f){

    for(int i = 0 ; i < m_reactorNodes.length() ; i++){
        if(m_reactorNodes.at(i) != nullptr)
            m_reactorNodes.at(i)->setReactor(false);
    }

    m_reactorNodes.clear();
    m_reactorMode = f;

    if(m_selectedNode != nullptr)
        m_selectedNode->setSelectionState(false);
}

