#include "message_widget.hpp"

#include <QLabel>
#include <QLayout>
#include <QLineEdit>


MessageWidget::MessageWidget(QWidget *parent) : QWidget(parent) 
{
	
    messageBox = new QLineEdit();
	messageBox ->setFont(QFont("Arial", 18));

  	QLabel *message = new QLabel("Message:");
  	message->setFont(QFont("Arial", 14));

    QBoxLayout *layout = new QHBoxLayout;
  	layout->addWidget(message);
  	layout->addWidget(messageBox);
  	this->setLayout(layout);

  	messageBox->setReadOnly(true);	
}

void MessageWidget::info(QString message)
{
   QPalette *palette = new QPalette();

  	palette->setColor(QPalette::Text, Qt::black);
  	messageBox->setPalette(*palette);
  	messageBox->setText(message);
}

void MessageWidget::error(QString message) 
{
  	QPalette *palette = new QPalette();
  	palette->setColor(QPalette::Highlight,Qt::red);
  	palette->setColor(QPalette::Text,Qt::black);
  	messageBox->setPalette(*palette);
  	messageBox->setText(message);
  	messageBox->selectAll();
}

void MessageWidget::clear() 
{
  	messageBox->clear();
}
