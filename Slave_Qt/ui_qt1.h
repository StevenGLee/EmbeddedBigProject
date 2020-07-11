/********************************************************************************
** Form generated from reading UI file 'qt1.ui'
**
** Created: Fri Jul 10 13:49:23 2020
**      by: Qt User Interface Compiler version 4.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QT1_H
#define UI_QT1_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Qt1
{
public:
    QLabel *lb_time;
    QLabel *lb_pic;
    QLabel *label;
    QLabel *lb_num;
    QLabel *lb_sum;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QRadioButton *rb_manual;
    QRadioButton *rb_auto;
    QComboBox *comboBox;
    QPushButton *pb_open_cam;
    QPushButton *pb_save_img;
    QPushButton *pb_prev;
    QPushButton *pb_next;
    QPushButton *pb_del;
    QPushButton *pb_open;
    QPushButton *pb_exit;

    void setupUi(QDialog *Qt1)
    {
        if (Qt1->objectName().isEmpty())
            Qt1->setObjectName(QString::fromUtf8("Qt1"));
        Qt1->resize(480, 272);
        lb_time = new QLabel(Qt1);
        lb_time->setObjectName(QString::fromUtf8("lb_time"));
        lb_time->setGeometry(QRect(10, 0, 335, 20));
        lb_time->setAlignment(Qt::AlignCenter);
        lb_pic = new QLabel(Qt1);
        lb_pic->setObjectName(QString::fromUtf8("lb_pic"));
        lb_pic->setGeometry(QRect(10, 20, 335, 251));
        lb_pic->setScaledContents(false);
        lb_pic->setAlignment(Qt::AlignCenter);
        label = new QLabel(Qt1);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(410, 150, 16, 21));
        lb_num = new QLabel(Qt1);
        lb_num->setObjectName(QString::fromUtf8("lb_num"));
        lb_num->setGeometry(QRect(390, 150, 21, 21));
        lb_sum = new QLabel(Qt1);
        lb_sum->setObjectName(QString::fromUtf8("lb_sum"));
        lb_sum->setGeometry(QRect(420, 150, 21, 20));
        layoutWidget = new QWidget(Qt1);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(360, 200, 81, 61));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        rb_manual = new QRadioButton(layoutWidget);
        rb_manual->setObjectName(QString::fromUtf8("rb_manual"));

        verticalLayout->addWidget(rb_manual);

        rb_auto = new QRadioButton(layoutWidget);
        rb_auto->setObjectName(QString::fromUtf8("rb_auto"));

        verticalLayout->addWidget(rb_auto);

        comboBox = new QComboBox(Qt1);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));
        comboBox->setGeometry(QRect(360, 170, 111, 21));
        pb_open_cam = new QPushButton(Qt1);
        pb_open_cam->setObjectName(QString::fromUtf8("pb_open_cam"));
        pb_open_cam->setGeometry(QRect(360, 0, 111, 31));
        pb_save_img = new QPushButton(Qt1);
        pb_save_img->setObjectName(QString::fromUtf8("pb_save_img"));
        pb_save_img->setGeometry(QRect(360, 30, 111, 31));
        pb_prev = new QPushButton(Qt1);
        pb_prev->setObjectName(QString::fromUtf8("pb_prev"));
        pb_prev->setGeometry(QRect(360, 60, 51, 31));
        pb_next = new QPushButton(Qt1);
        pb_next->setObjectName(QString::fromUtf8("pb_next"));
        pb_next->setGeometry(QRect(420, 60, 51, 31));
        pb_del = new QPushButton(Qt1);
        pb_del->setObjectName(QString::fromUtf8("pb_del"));
        pb_del->setGeometry(QRect(360, 90, 111, 31));
        pb_open = new QPushButton(Qt1);
        pb_open->setObjectName(QString::fromUtf8("pb_open"));
        pb_open->setGeometry(QRect(360, 120, 111, 31));
        pb_exit = new QPushButton(Qt1);
        pb_exit->setObjectName(QString::fromUtf8("pb_exit"));
        pb_exit->setGeometry(QRect(443, 200, 31, 61));

        retranslateUi(Qt1);

        QMetaObject::connectSlotsByName(Qt1);
    } // setupUi

    void retranslateUi(QDialog *Qt1)
    {
        lb_time->setText(QString());
        lb_pic->setText(QString());
        label->setText(QApplication::translate("Qt1", "/", 0, QApplication::UnicodeUTF8));
        lb_num->setText(QString());
        lb_sum->setText(QString());
        rb_manual->setText(QApplication::translate("Qt1", "Manual", 0, QApplication::UnicodeUTF8));
        rb_auto->setText(QApplication::translate("Qt1", "Auto", 0, QApplication::UnicodeUTF8));
        pb_open_cam->setText(QApplication::translate("Qt1", "Open cam", 0, QApplication::UnicodeUTF8));
        pb_save_img->setText(QApplication::translate("Qt1", "Save Img", 0, QApplication::UnicodeUTF8));
        pb_prev->setText(QApplication::translate("Qt1", "Prev", 0, QApplication::UnicodeUTF8));
        pb_next->setText(QApplication::translate("Qt1", "Next", 0, QApplication::UnicodeUTF8));
        pb_del->setText(QApplication::translate("Qt1", "Delete", 0, QApplication::UnicodeUTF8));
        pb_open->setText(QApplication::translate("Qt1", "Open", 0, QApplication::UnicodeUTF8));
        pb_exit->setText(QApplication::translate("Qt1", "Exit", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(Qt1);
    } // retranslateUi

};

namespace Ui {
    class Qt1: public Ui_Qt1 {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QT1_H
