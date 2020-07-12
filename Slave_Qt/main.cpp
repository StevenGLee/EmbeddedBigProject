#include <QtGui/QApplication>
#include <QtCore/QTextCodec>
#include "mainwindow.h"
#include "adc_page.h"
#include "qt1.h"
Qt1* camera_page;
ADC_page* adc_page;
MainWindow* main_window;
int main(int argc, char** argv)
{
	QApplication app(argc, argv);

	QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8")) ;
	app.setFont(QFont("wenquanyi", 12));
	//app.setFont(QFont("simsun", 10));

    //MD.show();
        camera_page = new Qt1();
        adc_page = new ADC_page();
        main_window = new MainWindow();
        main_window->show();

	// MD.setDefaultWeight();
	return app.exec();
}
