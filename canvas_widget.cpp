#include "canvas_widget.hpp"

#include <QWidget>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QLayout>
#include <iostream>

CanvasWidget::CanvasWidget(QWidget * parent): QWidget(parent)
{
    scene = new QGraphicsScene(this);
    QGraphicsView *graphicsLayout = new QGraphicsView(scene, this);
    QBoxLayout *canvasLayout = new QVBoxLayout;
    canvasLayout->addWidget(graphicsLayout);
    this->setLayout(canvasLayout);
}

void CanvasWidget::addGraphic(QGraphicsItem * item)
{
    scene->addItem(item);
}
