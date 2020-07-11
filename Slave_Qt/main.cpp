#include <QtGui/QApplication>
#include <QtCore/QTextCodec>
#include "mainwindow.h"
int main(int argc, char** argv)
{
	QApplication app(argc, argv);

	QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8")) ;
	app.setFont(QFont("wenquanyi", 12));
	//app.setFont(QFont("simsun", 10));

        //Qt1 MD;
    //MD.show();
        MainWindow main_window;
        main_window.show();
	// MD.setDefaultWeight();
	return app.exec();
}
