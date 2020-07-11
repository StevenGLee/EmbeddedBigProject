#include <iostream>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <QPainter>
#include <QFileDialog>
#include <QFontDialog>
#include <QColorDialog>
#include <QLineEdit>
#include <QTimer>
#include <pthread.h>
#include <QProcess>
#include <QMessageBox>
#include <QInputDialog>
#include <QDebug>
#include <fstream>
#include <QDateTime>
//#include <QImage>
#include "qt1.h"
#include "dlinklist.c"		//����dlinklist.h������ʾundefined reference

static int i=0;
static int num = 0;
//DLIST* Qt1:: p=NULL;
DLIST *p;
DLIST *q;
DLIST head;
QStringList strList;
void insert_dlinklist(DLIST *d,char *s);
//=====just for you to learn how to use comboBox=======
void Qt1::InitBox()
{
    comboBox->clear();
    //QStringList strList;
    strList<<"aaa"<<"bbb"<<"ccc";
    comboBox->addItems(strList);
}

void Qt1::fun_refresh_label()
{
        //lb_sum->setText(comboBox->currentText());
}
//=====just for you to learn how to use comboBox=======

Qt1::Qt1(QWidget *parent):QDialog(parent)
{
  	setupUi(this);
        this->setWindowFlags(Qt::FramelessWindowHint);
//  rb_manual->setChecked(true);
        isCapOpen = false;
        isToSave = false;
        m_image = NULL;
	rb_manual->setDisabled(true);
        rb_auto->setDisabled(true);
	pb_prev->setDisabled(true);
	pb_next->setDisabled(true);
        comboBox->setDisabled(true);
        pb_save_img->setDisabled(true);
    pb_del->setDisabled(true);
//=====just for you to learn how to use comboBox=======
        //InitBox();
        comboBox->clear();
	connect(&t3,SIGNAL(timeout()),this,SLOT(fun_refresh_label())); 
	t3.start(100);
//=====just for you to learn how to use comboBox=======
	connect(rb_auto,SIGNAL(clicked()),this,SLOT(fun_cntl())); 
	connect(rb_manual,SIGNAL(clicked()),this,SLOT(fun_cntl())); 
	connect(pb_prev,SIGNAL(clicked()),this,SLOT(fun_prev())); 
	connect(pb_next,SIGNAL(clicked()),this,SLOT(fun_pic())); 
	connect(pb_del,SIGNAL(clicked()),this,SLOT(fun_delete())); 	
	connect(pb_open,SIGNAL(clicked()),this,SLOT(fun_open())); 	
  	connect(&t1,SIGNAL(timeout()),this,SLOT(fun_time()));
        connect(&t2,SIGNAL(timeout()),this,SLOT(fun_pic()));
        connect(&t4, SIGNAL(timeout()), this, SLOT(fun_refresh_pic()));
        connect(pb_save_img, SIGNAL(clicked()),this,SLOT(fun_take_photo()));
        connect(pb_open_cam, SIGNAL(clicked()), this,SLOT(fun_cap_open()));
        connect(comboBox, SIGNAL(currentIndexChanged(int)),this, SLOT(fun_show_image(int)));
        connect(pb_exit,SIGNAL(clicked()),this,SLOT(fun_exit()));

  	t1.start(1000);						//������ʱ�������1s��ÿ��1s�ᷢ��һ���ź�����ʵ�����߱�
	init_dlinklist(&head);
        width = 640;
        height = 480;
        myCamera = new Camera("/dev/video0", width, height, 0);
        frameBufRGB = new unsigned char[width * height * 3];
        frameBufYUV = new unsigned char[width * height * 2];
}
void Qt1::fun_refresh_pic()
{
   if(!myCamera->GetBuffer(frameBufYUV))
   {
       std::cout<< "Get Camera Buf error!\n";
       return;
   }
   //TODO:process_image() function is now empty.
   myCamera->process_image(frameBufYUV, frameBufRGB);
   showCapPhoto();

}

void Qt1::fun_exit()
{
    close();
}

void Qt1::fun_show_image(int index)
{
    int asdfghjkl=0;
    p=&head;
    for(asdfghjkl=0; asdfghjkl<index+1; asdfghjkl++)
        p=p->next;
    i=index+1;
    display_pic();

}

void Qt1::fun_cap_open()
{
	if (isCapOpen == 0)
	{
                t4.start(50);//定时器如何绑定
		isCapOpen = 1;
		myCamera->OpenDevice();
                pb_open_cam->setText("Close Cam");
                pb_save_img->setDisabled(false);
	}
	else
	{
		//摄像头保存为灰按钮？？？
                isCapOpen = 0;
		myCamera->CloseDevice();
		fun_clean_pixmap();
                t4.stop();
                pb_save_img->setDisabled(true);
                pb_open_cam->setText("Open Cam");

	}
	
	//TODO: Use myCamera->OpenDevice() to open camera, and myCamera->CloseDevice() to close it.
    //You should think of it in two cases: camera closed and camera opened.
    //When you open camera, how to refresh image? Tips:use timer to trigge it.



}
void Qt1::fun_clean_pixmap()
{
    QPixmap p;
    lb_pic->setPixmap(p);
}



void Qt1::fun_take_photo()
{
    printf("Calling fun_take_photo()\n");
        num += 1;
        QString filepath = "./images/"; //"/mnt/usb/images/";
        printf("File path: %s\n", filepath.toStdString().c_str());
        QDateTime d=QDateTime::currentDateTime();
        filepath += d.toString("MM-dd_hh-mm-ss");
        printf("File path: %s\n", filepath.toStdString().c_str());
        filepath += ".jpg";
        printf("File path: %s\n", filepath.toStdString().c_str());
        bool save_result = m_image->save(filepath, "JPG", -1);
        printf("Save result:%d\n", save_result);
	//TODO: When this button is clicked, we take a photo and save it.

}
void Qt1::showCapPhoto()
{

    if(m_image != NULL)
        delete m_image;
    m_image = NULL;
    m_image = new QImage(frameBufRGB, width, height, QImage::Format_RGB888);
    lb_pic->setPixmap(QPixmap::fromImage(*m_image));
}


void Qt1::fun_time()
{
    QDateTime d=QDateTime::currentDateTime();	//��ȡ��ǰʱ��,currentDateTime()��һ����̬����
    lb_time->setText(d.toString("yyyy-MM-dd-ddd hh:mm:ss"));	//����ʱ���ʽ
}

void Qt1::fun_cntl()
{
	if(rb_manual->isChecked ())
	{
		t2.stop();
		pb_prev->setEnabled(true);	
		pb_next->setEnabled(true);	
	}
	else if(rb_auto->isChecked ())
	{
		t2.start(1000);
		pb_prev->setEnabled(false);	
		pb_next->setEnabled(false);	
	}
}

void Qt1::fun_pic()		
{
	i++;
	if(i==(len+1))i=1;

	p=p->next;
	display_pic();
}


void Qt1::fun_prev()	//fun_next()��fun_pic()һ��
{
	i--;
	if(i==0)i=len;

	p=p->prev;
	display_pic();
}

void Qt1::display_pic()
{
	QString buf(p->path);
	QPixmap p(buf);
	lb_pic->setPixmap(p);
	lb_pic->setScaledContents(1);
	lb_num->setText(QString::number(i));
	lb_sum->setText(QString::number(len));
}


void Qt1::fun_delete()
{
	int ret;
        ret=QMessageBox::warning(this, tr("m_image browser"),
					   tr("Are you sure to delete it?"),
					   QMessageBox::Yes | QMessageBox::No,
					   QMessageBox::No);
	if(ret==QMessageBox::Yes)
	{
		remove(p->path);
		DLIST *r=(&head)->next;
		int n=1;
		while(r!=p)		//�ҵ���ǰ p��λ��
		{
			n++;
			r=r->next;
		}
		p=p->next;
		del_dlinklist(&head,n);
		if(i==len)i--;
		len=legnth_dlinklist(&head);
		display_pic();
	}
//	dlinkilist_tofile(&head);
}

void Qt1::fun_open()
{
    QString fileName;
    QFileDialog dlg(this,"open image file");
    dlg.resize(480,272);
    dlg.setWindowFlags(Qt::FramelessWindowHint);
    dlg.setAcceptMode(QFileDialog::AcceptOpen);
    dlg.setNameFilter("Images (*.png *.xpm *.jpg)");
    dlg.setDirectory("./images");
    dlg.exec();
    QStringList files = dlg.selectedFiles();
    if(!files.isEmpty())
    {
        fileName = files.at(0);
    }
        //QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
        //                                         "./images",
        //                                         tr("Images (*.png *.xpm *.jpg)"));
	printf("open folder!\n");
	if(fileName!=NULL)
	{
		init_dlinklist(&head);	//Ϊ����һ��Ŀ¼��׼��
		char *str;
		char filename[100]={0};
		DIR *dp;					//ָ��Ŀ¼
		struct dirent *dirp;		//ָ��Ŀ¼��(��������Ŀ¼����ͨ�ļ�)
		
		int m=strlen(fileName.toAscii().data());
		str=(char*)malloc(m);
		strcpy(str,fileName.toAscii().data());
		strcpy(filename,str);

		QString buf(fileName.toAscii().data());
		QPixmap first(buf);
		lb_pic->setPixmap(first);
		lb_pic->setScaledContents(1);
		
		strip(filename);
		dp=opendir(filename);
		while(dirp=readdir(dp))
		{
			if(judge(dirp->d_name)==1)
			{
				strcat(filename,"/");
				strcat(filename,dirp->d_name);	//�ļ����ڵ�Ŀ¼+�ļ���=����·��
				printf("%s\n",filename);
				insert_dlinklist(&head,filename);
				filename[strlen(filename)-strlen(dirp->d_name)-1]='\0';
                                strList<<dirp->d_name;
			}	
		}
		q->next=(&head)->next;
		(&head)->next->prev=q;
		
		len=legnth_dlinklist(&head);
		int pos=1;
		DLIST *r=(&head)->next;
		while(strcmp(r->path,str)!=0)
		{
			r=r->next;
			pos++;
		}
		p=r;
		i=pos;
		lb_num->setText(QString::number(i));
		lb_sum->setText(QString::number(len));
                comboBox->addItems(strList);
                comboBox->setDisabled(false);
		rb_manual->setDisabled(false);
		rb_auto->setDisabled(false);
		pb_prev->setDisabled(false);
		pb_next->setDisabled(false);
		pb_del->setDisabled(false);
		rb_manual->setChecked(true);
//		display_pic();
	}
//	dlinkilist_tofile(&head);
}

void Qt1::strip(char *s)
{
	char *p=s;
	int n=0;
	while(*p)
	{
		if(*p=='/')
		{
			n++;
		}
		p++;
	}
	if(n==1)
	{
		s[1]='\0';
	}
	else
	{
		p--;
		while(*p)
		{
			if(*p=='/')
			{
				*p='\0';
				break;
			}
			p--;
		}
	}
}

int Qt1::judge(char * s)	
{
        int i,len=strlen(s);
	char *p=s;
	if(len<5)
	{
		return 0;
	}
	else 
	{
		while(*p)
		{
			p++;
		}
		for(i=0;i<4;i++)p--;
		if((strncmp(p,".jpg",4)==0)||(strncmp(p,".png",4)==0)||(strncmp(p,".xpm",4)==0))
		{
			return 1;
		}
		else 
			return 0;
	}
}

void insert_dlinklist(DLIST *d,char *s)	//������ĩβ����,֮���԰���������ᵽ�������Ϊ����ʹ��ȫ�ֱ���DLIST *q
{
	DLIST *r=d;
//	DLIST *q;
	q=(DLIST*)malloc(sizeof(DLIST));	//�����q�����Ǿֲ��ģ������޷��γ�˫��ѭ������
	memset(q,0,sizeof(q));
	strcpy(q->path,s);

	if(r->next==NULL)
	{
		r->next=q;
		q->next=NULL;
	}
	else
	{
		while(r->next!=NULL)
		{
			r=r->next;
		}
		r->next=q;
		q->prev=r;
		q->next=NULL;
	}
	printf("insert success\n");
}


Qt1::~Qt1()
{
    delete[] frameBufRGB;
    delete[] frameBufYUV;
    delete m_image;
}
