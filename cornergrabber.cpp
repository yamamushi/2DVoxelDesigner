#include "cornergrabber.h"

CornerGrabber::CornerGrabber(QGraphicsItem *parent,  int corner) :
    QGraphicsItem(parent),
    mouseDownX(0),
    mouseDownY(0),
    _outterborderColor(Qt::black),
    _outterborderPen(),
    _width(6),
    _height(6),
    _corner(corner),
    _mouseButtonState(kMouseReleased)
{
    setParentItem(parent);

    _outterborderPen.setWidth(2);
    _outterborderPen.setColor(_outterborderColor);

    if(_corner == 4)
        _outterborderColor = Qt::white;

   this->setAcceptHoverEvents(true);
}

void CornerGrabber::setMouseState(int s)
{
    _mouseButtonState = s;
}

int CornerGrabber::getMouseState()
{
    return _mouseButtonState;
}

int CornerGrabber::getCorner()
{
    return _corner;
}


// we have to implement the mouse events to keep the linker happy,
// but just set accepted to false since are not actually handling them

void CornerGrabber::mouseMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    event->setAccepted(false);
}

void CornerGrabber::mousePressEvent(QGraphicsSceneDragDropEvent *event)
{
    event->setAccepted(false);
}

void CornerGrabber::mouseReleaseEvent ( QGraphicsSceneMouseEvent * event )
{
    event->setAccepted(true);
}

void CornerGrabber::mousePressEvent ( QGraphicsSceneMouseEvent * event )
{
    event->setAccepted(false);
}

void CornerGrabber::mouseMoveEvent ( QGraphicsSceneMouseEvent * event )
{
    event->setAccepted(false);
}


// change the color on hover events to indicate to the use the object has
// been captured by the mouse

void CornerGrabber::hoverLeaveEvent ( QGraphicsSceneHoverEvent * )
{
    _outterborderColor = Qt::black;
    if(_corner == 4)
        _outterborderColor = Qt::white;

    this->update(0,0,_width,_height);
}

void CornerGrabber::hoverEnterEvent ( QGraphicsSceneHoverEvent * )
{
    _outterborderColor = Qt::red;
    this->update(0,0,_width,_height);
}

QRectF CornerGrabber::boundingRect() const
{
    return QRectF(0,0,_width,_height);
}


void CornerGrabber::paint (QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{

    // fill the box with solid color, use sharp corners
    if(_corner != 4){
        _outterborderPen.setCapStyle(Qt::SquareCap);
        _outterborderPen.setStyle(Qt::SolidLine);
        painter->setPen(_outterborderPen);

        QPointF topLeft (0, 0);
        QPointF bottomRight ( _width, _height);

        QRectF rect (topLeft, bottomRight);

        QBrush brush (Qt::SolidPattern);
        brush.setColor (_outterborderColor);
        painter->fillRect(rect,brush);
    }else{
        QPointF topLeft (0, 0);
        QPointF bottomRight ( _width, _height);

        QRectF rect (topLeft, bottomRight);
        QBrush brush (Qt::SolidPattern);
        brush.setColor (_outterborderColor);
        painter->setBrush(brush);

        QPen pen(Qt::red);
        pen.setWidth(_width>2?_width/2:1);
        painter->setPen(pen);

        painter->drawEllipse(rect);
    }

}

void CornerGrabber::setRad(int r){
    _width = r * 2;
    _height = r * 2;

//    if(_corner == 4){
//        _width *= 2;
//        _height *= 2;
//    }
}
