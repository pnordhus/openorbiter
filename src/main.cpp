#include "form_main.h"
#include <QApplication>
#include <QTime>


int main(int argc, char* argv[])
{
	srand(QTime::currentTime().msecsTo(QTime()));
	
	QApplication app(argc, argv);
	app.setOrganizationName("OpenOrbiter");
	app.setApplicationName("OpenOrbiter");
	
	FormMain form;
	form.show();
	
	return app.exec();
}
