#include <QApplication>
#include <QtGui>
#include <QtUiTools/QtUiTools>
#include <QtWidgets/QWidget>
#include "graphics/graphics.h"
#include "Project.h"
using namespace tng;

int main(int argc, char **argv)
{
	QApplication app(argc, argv);
	ProjectMain* projectMain = new ProjectMain();
	projectMain->Init();
	projectMain->show();
	app.exec();
	delete projectMain;
}