#include "Calculator.h"
#include <QDir>
using namespace std;
using namespace cv;

bool Calculator::pointInContours(int **data, int m, int n, cv::Point point){
	if(!data){
		return false;
	}

	int x = point.x;
	int y = point.y;

	//�����������
	if(data[x][y] == 255){
		return true;
	}

	//�жϵ��Ƿ��������ڲ�
	int rayInnConturs = 0;					//���������������������������������������ڲ�
	int rayNotInContours = 0;				//����ż���������������������������������ⲿ
	int contours = 0;						//���߾���������������
	int px = x, py = y;						//����ĸ���

	//x�������������
	contours = 0;						
	px = x, py = y;
	while(px < m - 1){
		if(data[px][py] == 0 && data[px + 1][py] == 255)
			contours ++;
		px ++;
	}
	if(contours % 2 == 0)
		rayNotInContours ++;
	else
		rayInnConturs ++;

	//x�Ḻ���������
	contours = 0;						
	px = x, py = y;
	while(px > 0){
		if(data[px][py] == 0 && data[px - 1][py] == 255)
			contours ++;
		px --;
	}
	if(contours % 2 == 0)
		rayNotInContours ++;
	else
		rayInnConturs ++;

	//y�������������
	contours = 0;						
	px = x, py = y;
	while(py < n - 1){
		if(data[px][py] == 0 && data[px][py + 1] == 255)
			contours ++;
		py++;
	}
	if(contours % 2 == 0)
		rayNotInContours ++;
	else
		rayInnConturs ++;

	//y�Ḻ���������
	contours = 0;						
	px = x, py = y;
	while(py > 0){
		if(data[px][py] == 0 && data[px][py - 1] == 255)
			contours ++;
		py--;
	}
	if(contours % 2 == 0)
		rayNotInContours ++;
	else
		rayInnConturs ++;


	//+x, +y���������
	contours = 0;						
	px = x, py = y;
	while(px < m - 1 && py < n - 1){
		if(data[px][py] == 0 && data[px + 1][py + 1] == 255)
			contours ++;
		px ++;
		py ++;
	}
	if(contours % 2 == 0)
		rayNotInContours ++;
	else
		rayInnConturs ++;

	//+x, -y���������
	contours = 0;						
	px = x, py = y;
	while(px < m - 1 && py > 0){
		if(data[px][py] == 0 && data[px + 1][py - 1] == 255)
			contours ++;
		px ++;
		py --;
	}
	if(contours % 2 == 0)
		rayNotInContours ++;
	else
		rayInnConturs ++;

	//-x, +y���������
	contours = 0;						
	px = x, py = y;
	while(px > 0 && py < n - 1){
		if(data[px][py] == 0 && data[px - 1][py + 1] == 255)
			contours ++;
		px --;
		py ++;
	}
	if(contours % 2 == 0)
		rayNotInContours ++;
	else
		rayInnConturs ++;

	//-x, -y���������
	contours = 0;						
	px = x, py = y;
	while(px > 0 && py > 0){
		if(data[px][py] == 0 && data[px - 1][py - 1] == 255)
			contours ++;
		px --;
		py --;
	}
	if(contours % 2 == 0)
		rayNotInContours ++;
	else
		rayInnConturs ++;

	if(rayInnConturs >= rayNotInContours){
		return true;
	} else {
		return false;
	}
}

QPixmap Calculator::Mat2QPixmap(cv::Mat &image){
	Mat rgb;  
	QImage img;  
	if(image.channels() == 3)    // RGB image  
	{  
		cvtColor(image,rgb,CV_BGR2RGB);  
		img = QImage((const uchar*)(rgb.data),  //(const unsigned char*)  
			rgb.cols,rgb.rows,  
			rgb.cols*rgb.channels(),   //new add  
			QImage::Format_RGB888);  
	}else                     // gray image  
	{  
		img = QImage((const uchar*)(image.data),  
			image.cols,image.rows,  
			image.cols*image.channels(),    //new add  
			QImage::Format_Indexed8);  
	}

	QPixmap imgdata = QPixmap::fromImage(img);
	return imgdata;
}

bool Calculator::removeDirWithContent(const QString &dirName){  
	static QVector<QString> dirNames;  
	static QString funcErrMsg="ɾ��[%1]ʧ��.";  
	static QString funcInfFndMsg="����[%1].";  
	static QString funcInfDelMsg="ɾ��[%1]�ɹ�.";  
	QDir dir;  
	QFileInfoList filst;  
	QFileInfoList::iterator curFi;  
	//��ʼ��
	dirNames.clear();  
	if(dir.exists()){  
		dirNames<<dirName;  
	}  
	else{  
		return true;  
	}  
	//���������ļ��У�������Щ�ļ����е��ļ�ɾ��  
	for(int i=0;i<dirNames.size();++i){  
		dir.setPath(dirNames[i]);  
		filst=dir.entryInfoList(QDir::Dirs|QDir::Files  
			|QDir::Readable|QDir::Writable  
			|QDir::Hidden|QDir::NoDotAndDotDot  
			,QDir::Name);  
		if(filst.size()>0){  
			curFi=filst.begin();  
			while(curFi!=filst.end()){  
				//�����ļ���,��������ļ����б�dirsβ��  
				if(curFi->isDir()){  
					dirNames.push_back(curFi->filePath());  
				}else if(curFi->isFile()){  
					//�����ļ�,��ɾ��֮  
					if(!dir.remove(curFi->fileName())){  
						return false;  
					}  
				}  
				curFi++;  
			}//end of while  
		}  
	}  
	//ɾ���ļ���  
	for(int i=dirNames.size()-1;i>=0;--i){  
		dir.setPath(dirNames[i]);  
		if(!dir.rmdir(".")){  
			return false;  
		}  
	}   
	return true;  
}  
