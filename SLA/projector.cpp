#include "projector.h"
#include <QDesktopWidget>
#include <QApplication>


#include "Calculator.h"

using namespace std;
using namespace cv;

Projector::Projector(QWidget *parent /* = 0 */)
	:QWidget(parent)
{
	//多显示屏设置
	QDesktopWidget *desktop = QApplication::desktop();

	int deskID = desktop->primaryScreen();
	int deskCount = desktop->screenCount();

	for(int i = deskID; i < deskCount; i ++){
		QRect rect = desktop->screenGeometry(i);
		printf("num %d\tw = %d, h = %d\n", i, rect.width(), rect.height());
	}

	if (deskCount < 2){
		printf("no projector!\n");
		

		this->setWindowOpacity(1);
		//this->setWindowFlags(Qt::FramelessWindowHint);      
		this->setAutoFillBackground(true);
		QPalette palette;
		palette.setColor(QPalette::Background, QColor(0,0,0));
		this->setPalette(palette);

		rect0 = desktop->screenGeometry(deskID);
		rect1 = QRect(rect0.width() / 4, rect0.height() / 4, rect0.width() / 2, rect0.height() / 2);
		this->setFixedSize(rect1.size());
		this->move(rect0.width() / 4, rect0.height() / 4);

	} else {
		this->setWindowOpacity(1);
		this->setWindowFlags(Qt::FramelessWindowHint);
		this->setAutoFillBackground(true);
		QPalette palette;
		palette.setColor(QPalette::Background, QColor(0,0,0));
		this->setPalette(palette);

		rect0 = desktop->screenGeometry(deskID);
		rect1 = desktop->screenGeometry(deskID + 1);
		this->setFixedSize(rect1.size());
		this->move(rect0.width(), 0);
	}

	imgTable = new QLabel(this);
	imgTable->setVisible(false);

	//设置背景图片
	cv::Mat mat(rect1.height(), rect1.height(), CV_8UC1, cv::Scalar(255));
	bg = new QPixmap(Calculator::Mat2QPixmap(mat));

	//垃圾回收
	//delete desktop;	//会出现问题
}

Projector::~Projector(){
	clearImgList();
}

void Projector::showBG(){

	int x = rect1.width() - bg->width();
	int y = rect1.height() - bg->height();

	imgTable->setPixmap(*bg);
	imgTable->move(x, y);
	imgTable->setVisible(true);
}

void Projector::showImage(int imageID){
	//获取图片位置
	int index = imageID;

	
	float scale = float(rect1.height()) / float(imgList[index]->height());
	

	QPixmap *image = new QPixmap(imgList[index]->scaled(imgList[index]->width() * scale, imgList[index]->height() * scale));
	int x = rect1.width() - image->width();
	int y = rect1.height() - image->height();

	imgTable->setPixmap(*image);
	imgTable->move(x, y);
	imgTable->setVisible(true);

	//垃圾回收
	delete image;
}

void Projector::showImage(int imageID, int blockID){
	//获取图片位置
	int index = imageID;

	Mat mat = *matList[index];
	Mat img(mat.rows, mat.cols, mat.type(), Scalar(0));
	Mat roi_mat;
	Mat roi_img;

	int bh = 0.6 * mat.rows;
	int bw = 0.6 * mat.cols;
	
	if(blockID == 0){
		roi_mat = mat(Rect(0, 0, bw, bh));
		roi_img = img(Rect(0, 0, bw, bh));
	} else if(blockID == 1){
		roi_mat = mat(Rect(mat.cols - bw, 0, bw, bh));
		roi_img = img(Rect(img.cols - bw, 0, bw, bh));
	} else if(blockID == 2){
		roi_mat = mat(Rect(0, mat.rows - bh, bw, bh));
		roi_img = img(Rect(0, img.rows - bh, bw, bh));
	} else {
		roi_mat = mat(Rect(mat.cols - bw, mat.rows - bh, bw, bh));
		roi_img = img(Rect(img.cols - bw, img.rows - bh, bw, bh));
	}
	roi_mat.copyTo(roi_img);

	QPixmap pixmap = Calculator::Mat2QPixmap(img);

	float scale = float(rect1.height()) / float(pixmap.height());


	QPixmap *image = new QPixmap(pixmap.scaled(pixmap.width() * scale, pixmap.height() * scale));
	
	int x = rect1.width() - image->width();
	int y = rect1.height() - image->height();

	imgTable->setPixmap(*image);
	imgTable->move(x, y);
	imgTable->setVisible(true);

	//垃圾回收
	delete image;
}

void Projector::hideImage(){
	imgTable->setVisible(false);
}

void Projector::pushback(QPixmap *image){
	imgList.push_back(image);
}

void Projector::pushback(Mat *mat){
	matList.push_back(mat);
}

void Projector::clearImgList(){
	for(int i = 0; i < imgList.size(); i ++){
		delete imgList[i];
	}
	for(int i = 0; i < matList.size(); i ++){
		delete matList[i];
	}
	imgList.clear();
	matList.clear();
}
