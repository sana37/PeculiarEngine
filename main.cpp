#include <QApplication>
#include "Field.h"

int main(int argc , char** argv)
{
	QApplication app(argc , argv);

	Field* field = Field::getInstance();
	field->open();

	app.exec();

	Field::deleteInstance();

	return 0;
}
