#ifndef ADC_PAGE_H
#define ADC_PAGE_H

#include <QDialog>

namespace Ui {
    class ADC_page;
}

class ADC_page : public QDialog
{
    Q_OBJECT

public:
    explicit ADC_page(QWidget *parent = 0);
    ~ADC_page();

private:
    Ui::ADC_page *ui;
};

#endif // ADC_PAGE_H
