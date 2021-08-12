#ifndef QT_INTERPRETER_HPP
#define QT_INTERPRETER_HPP

#include <string>

#include <QObject>
#include <QString>
#include <QGraphicsItem>

#include "interpreter.hpp"

class QtInterpreter: public QObject, private Interpreter{
Q_OBJECT

public:

  QtInterpreter(QObject * parent = nullptr);

signals:

  void drawGraphic(QGraphicsItem * item);

  void info(QString message);

  void error(QString message);

public slots:

  void parseAndEvaluate(QString entry);
  void output(Expression exp);

 private:
 	Interpreter inter;
 	QString toString(QString input);
 	void determineGraphic(Expression exp);
 	void drawGraphics(std::vector<Atom> args);
};

#endif
