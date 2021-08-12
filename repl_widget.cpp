#include "repl_widget.hpp"

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QLayout>
#include <QKeyEvent>
#include <QDebug>
#include <iostream>

REPLWidget::REPLWidget(QWidget * parent): QWidget(parent)
{
	counter = -1;
	//create a QLineEdit for the REPL box
	slispBox = new QLineEdit();
	slispBox ->setFont(QFont("Arial", 18));

	//create a new layout and lable for the slisp
	QLabel *slispLabel = new QLabel("slisp>");
  	slispLabel->setFont(QFont("Arial", 14));

  	QBoxLayout *layout = new QHBoxLayout;

  	//add the widgets to the layout
  	layout->addWidget(slispLabel);
  	layout->addWidget(slispBox);
  	this->setLayout(layout);

  	//connect the signals and slots
  	connect(slispBox, &QLineEdit::returnPressed, this, &REPLWidget::changed);
  	slispBox->installEventFilter(this);

}

void REPLWidget::changed() 
{
  	QString input = slispBox->text();
  	pastQueries.push_back(input);
  	counter = pastQueries.size();
    emit lineEntered(input);
    slispBox->clear();
}

void REPLWidget::upArrow()
{
	QString input;
  	if (counter > 0)
	{
		counter--;
		input = pastQueries[counter];		
		slispBox->setText(input);	
	}	
	
}

void REPLWidget::downArrow()
{
    QString input;
    if (counter < pastQueries.size() - 1)
    {
        counter++;
        input = pastQueries[counter];
        slispBox->setText(input);
    }	
    else if (counter <= pastQueries.size() - 1)
    {
        counter++;
        QString blank = "";
        slispBox->setText(blank);
    }
}

bool REPLWidget::eventFilter(QObject* obj, QEvent* event)
{
	if (event->type() == QEvent::KeyPress) 
	{
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->key() == Qt::Key_Up)
        {
            upArrow();
        }
        else if (keyEvent->key() == Qt::Key_Down)
        {
            downArrow();
        }
        else
        {
            return QObject::eventFilter(obj, event);
        }

    }
}


