#include "main_window.hpp"

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <regex>

#include <QLayout>
#include <QFont>


#include "message_widget.hpp"
#include "canvas_widget.hpp"
#include "repl_widget.hpp"
#include "interpreter_semantic_error.hpp"

MainWindow::MainWindow(QWidget * parent): MainWindow("", parent)
{
	//create a layout and instantiate all of the Qwidgets
	QBoxLayout *layout = new QVBoxLayout;
    QtInterpreter *interpGUI = new QtInterpreter(this);
    MessageWidget *message = new MessageWidget(this);
    layout->addWidget(message);
    CanvasWidget *canvas = new CanvasWidget(this);
    layout->addWidget(canvas);
    REPLWidget *repl = new REPLWidget(this);
    layout->addWidget(repl);
    this->setLayout(layout);
    this->setMinimumSize(800, 600);

    //connect all of the signals and slots
    connect(repl, &REPLWidget::lineEntered, interpGUI, &QtInterpreter::parseAndEvaluate);
    connect(interpGUI, &QtInterpreter::info, message, &MessageWidget::info);
    connect(interpGUI, &QtInterpreter::error, message, &MessageWidget::error);
    connect(interpGUI, &QtInterpreter::drawGraphic, canvas, &CanvasWidget::addGraphic);
    

}

MainWindow::MainWindow(std::string filename, QWidget * parent): QWidget(parent)
{
	//create a layout and instantiate all of the Qwidgets
    QBoxLayout *layout = new QVBoxLayout;
    QtInterpreter *interpGUI = new QtInterpreter(this);
    MessageWidget *message = new MessageWidget(this);
    layout->addWidget(message);
    CanvasWidget *canvas = new CanvasWidget(this);
    layout->addWidget(canvas);
    REPLWidget *repl = new REPLWidget(this);
    layout->addWidget(repl);
    this->setLayout(layout);
    this->setMinimumSize(800, 600);

    //connect all of the ports
    connect(repl, &REPLWidget::lineEntered, interpGUI, &QtInterpreter::parseAndEvaluate);
    connect(interpGUI, &QtInterpreter::info, message, &MessageWidget::info);
    connect(interpGUI, &QtInterpreter::error, message, &MessageWidget::error);
    connect(interpGUI, &QtInterpreter::drawGraphic, canvas, &CanvasWidget::addGraphic);

    //send the filename to parse and evaluate by lineEntered
	QString program = QString::fromStdString(filename);
	repl->lineEntered(program);
}
