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
	//成员函数
	void showBG();
	void showImage(int imgID);					//显示层数
	void showImage(int imgID, int blockID);		//显示分块
	void hideImage();							//不显示
	void pushback(QPixmap *);					//进入
	void pushback(cv::Mat *);					//进入
	void clearImgList();						//初始化


public:
	//成员变量
	std::vector<QPixmap *> imgList;		//图片列表
	std::vector<cv::Mat *> matList;		//图片列表
	QLabel *imgTable;
	QPixmap *bg;

	QRect rect0;
	QRect rect1;
};

#endif