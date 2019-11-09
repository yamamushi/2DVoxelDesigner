#ifndef IMAGEITEM_H
#define IMAGEITEM_H

#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneMouseEvent>
#include <QColor>
#include <QPainter>
#include <QPen>
#include <QPointF>
#include "cornergrabber.h"

class ImageItem : public QGraphicsItem
{
public:
    ImageItem(QPixmap pix);
    void    setScaleFactor(float f);
    void    setImageSize(int w, int h);
    void    setImageAngle(int alpha);
    int     getImageWith();
    int     getImageHeight();
    int     getImageX();
    int     getImageY();
    int     getImageAngle();
    void    lockImage(bool f);
    void    maintainAspectRatio(bool);
private:
    virtual QRectF boundingRect() const; ///< must be re-implemented in this class to provide the diminsions of the box to the QGraphicsView
    virtual void paint (QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget); ///< must be re-implemented here to pain the box on the paint-event
    virtual void hoverEnterEvent ( QGraphicsSceneHoverEvent * event ); ///< must be re-implemented to handle mouse hover enter events
    virtual void hoverLeaveEvent ( QGraphicsSceneHoverEvent * event ); ///< must be re-implemented to handle mouse hover leave events

    virtual bool sceneEventFilter ( QGraphicsItem * watched, QEvent * event ) ;

    void setCornerPositions();


    QColor _outterborderColor; ///< the hover event handlers will toggle this between red and black

    int     _gridSpace;
    qreal   _width;
    qreal   _height;

    CornerGrabber*  _corners[4];// 0,1,2,3  - starting at x=0,y=0 and moving clockwise around the box, 4- center->rotate

    QPixmap _pixmap;
    float   _cornerRad;
    float   _scaleFactor;

    bool    _locked;
    bool    _maintainAspectRatio;
    qreal   _aspectRatio;
};

#endif // IMAGEITEM_H
