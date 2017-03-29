#include <QApplication>
#include "Field.h"

int main(int argc , char** argv)
{
	QApplication app(argc , argv);

	Field field;
	field.open();

	app.exec();

	return(0);
}
