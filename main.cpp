#include <QApplication>
#include "IntegratedWindow.h"

int main(int argc , char** argv)
{
	QApplication* app = new QApplication(argc , argv);
	IntegratedWindow* window = new IntegratedWindow();

	app->exec();

	delete window;
	delete app;
	return(0);
}
