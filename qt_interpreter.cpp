//module includes
#include "qt_interpreter.hpp"
#include "message_widget.hpp"
#include "repl_widget.hpp"
#include "tokenize.hpp"
#include "qgraphics_arc_item.hpp"
#include "interpreter_semantic_error.hpp"

//system includes
#include <string>
#include <sstream>
#include <iostream>
#include <cmath>
#include <fstream>

//QT includes
#include <QLineEdit>
#include <QPainter>
#include <QBrush>
#include <QDebug>
#include <QString>
#include <QtMath>
#include <QGraphicsEllipseItem>
#include <QGraphicsItem>
#include <QStyleOptionGraphicsItem>


QtInterpreter::QtInterpreter(QObject * parent): QObject(parent)
{
}

void QtInterpreter::parseAndEvaluate(QString entry)
{
  	std::string parse = entry.toStdString();
  	bool validParse = false;
  	//check to see if the entry is a file name
  	//if so, parse the file
  	if (parse.empty())
  	{
  		return;
  	}
  	if (parse.length() > 4) //check to see if the entry is a file name. if so, parse the file
  	{
  		if (parse.substr(parse.length() - 4, parse.length()) == ".slp")
  		{
  			std::ifstream ifs(parse);
  			if (!ifs.good())
  			{
  				std::cout << "Error: Filename could not be found" << std::endl;
  				return;
  			}
  			validParse = inter.parse(ifs);
  		}
  		else //is a normal Query, take from ReplWidget
  		{
  			std::istringstream iss(parse);
  			validParse = inter.parse(iss);
  		}
  	}
  	else  //is a normal Query, take from ReplWidget
  	{
  		std::istringstream iss(parse);
  		validParse = inter.parse(iss);
  	}

    Expression exp;
    if (validParse) 
    {
      	try 
      	{
          	exp = inter.eval();
          	determineGraphic(exp);
          	output(exp);
      	}
      	catch (const std::exception & ex) 
      	{
        	QString errorMessage = QString::fromStdString(ex.what());
        	emit error(errorMessage);
      	}
    }
    else 
    {
        QString errorMessage = "Error: Cannot Parse";
        emit error(errorMessage);
    }
}

void QtInterpreter::determineGraphic(Expression exp)
{
	inter.setGraphics();
	std::vector<Atom> args = inter.getGraphics();
	if (!args.empty()) //if the ast has draw as its head
	{
		drawGraphics(args);
	}
	else
	{
		output(exp);
    }
}


void QtInterpreter::output(Expression exp)
{
	QString Output;
	QString openP = "(";
	QString closedP = ")";
	if (exp.head.type == NumberType)
	{
		Output = openP + QString::number(exp.head.value.num_value) + closedP;
		emit info(Output);	
	}
	else if (exp.head.type == BooleanType)
	{
		if (exp.head.value.bool_value)
		{
			Output = "(True)";
		}
		else
		{
			Output = "(False)";
		}	
		emit info(Output);	
	}
	else if (exp.head.type == SymbolType)
	{
		//Output = openP + QString::fromStdString(exp.head.value.sym_value) + closedP;
		//emit info(Output);	
	}
	else if (exp.head.type == PointType)
	{		
		QString x = QString::number(exp.head.value.point_value.x);
		QString  y = QString::number(exp.head.value.point_value.y);
		Output = openP + x + "," + y + closedP;
		emit info(Output);
	}
	else if (exp.head.type == LineType)
	{		
		QString x1 = QString::number(exp.head.value.line_value.first.x);
		QString y1 = QString::number(exp.head.value.line_value.first.y);
		QString x2 = QString::number(exp.head.value.line_value.second.x);
		QString y2 = QString::number(exp.head.value.line_value.second.x);
		Output = openP + openP + x1 + "," + y1 + closedP + "," + openP + x2 + "," + y2 + closedP + closedP;
		emit info(Output);	
	}
	else if (exp.head.type == ArcType)
	{	
		QString x1 = QString::number(exp.head.value.arc_value.center.x);
		QString y1 = QString::number(exp.head.value.arc_value.center.y);
		QString x2 = QString::number(exp.head.value.arc_value.start.x);
		QString y2 = QString::number(exp.head.value.arc_value.start.y);
		QString angle = QString::number(exp.head.value.arc_value.span);
		Output = openP + openP + x1 + "," + y1 + closedP + "," + openP + x2 + "," + y2 + closedP + " " + angle + closedP;
		emit info(Output);	
	}
	else if (exp.head.type == NoneType)
	{
		Output = "(None)";
		emit info(Output);	
	}
}


void QtInterpreter::drawGraphics(std::vector<Atom> args)
{
	//loop through the vector and draw all of the points/lines/arcs
	for (int i = 0; i < args.size(); i++) 
	{
		if (args[i].type == PointType)
		{		
			QGraphicsEllipseItem * point = new QGraphicsEllipseItem(args[i].value.point_value.x, 
				args[i].value.point_value.y, 2, 2);
			point->setBrush(QBrush(Qt::black));
            emit drawGraphic(point);
		}
		else if (args[i].type == LineType)
		{		
			QGraphicsLineItem * line = new QGraphicsLineItem(args[i].value.line_value.first.x, args[i].value.line_value.first.y,
        		args[i].value.line_value.second.x, args[i].value.line_value.second.y);                                                       
			emit drawGraphic(line);
		}
		else if (args[i].type == ArcType)
		{
			double x1, x2, y1, y2;
			x1 = args[i].value.arc_value.center.x;
			y1 = args[i].value.arc_value.center.y;
			x2 = args[i].value.arc_value.start.x;
			y2 = args[i].value.arc_value.start.y;
			double spanAngle = qRadiansToDegrees((args[i].value.arc_value.span) * 16);
			QLineF line(QLineF(QPointF(x1, y1), QPointF(x2, y2)));
			double width = line.length() * 2;
			double height = width;
			double startAngle = line.angle() * 16;
			QGraphicsArcItem * arc = new QGraphicsArcItem(x1, y1, width, height);;
			arc->setRect(QRectF(x1 - width / 2, y1 - height / 2, width, height));
			arc->setStartAngle(startAngle);
			arc->setSpanAngle(spanAngle);
			emit drawGraphic(arc);
		}	
	}

}

