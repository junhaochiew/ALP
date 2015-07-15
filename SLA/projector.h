#ifndef PROJECTOR_H

#include <QWidget>
#include <QLabel>

#include <cv.h>


class Projector:public QWidget{
	Q_OBJECT

public:
	Projector(QWidget *parent = 0);
	~Projector();

public:
	//��Ա����
	void showBG();
	void showImage(int imgID);					//��ʾ����
	void showImage(int imgID, int blockID);		//��ʾ�ֿ�
	void hideImage();							//����ʾ
	void pushback(QPixmap *);					//����
	void pushback(cv::Mat *);					//����
	void clearImgList();						//��ʼ��


public:
	//��Ա����
	std::vector<QPixmap *> imgList;		//ͼƬ�б�
	std::vector<cv::Mat *> matList;		//ͼƬ�б�
	QLabel *imgTable;
	QPixmap *bg;

	QRect rect0;
	QRect rect1;
};

#endif