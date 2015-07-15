#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QGLWidget>
#include <QMouseEvent>

#include <cv.h>
#include <highgui.h>

using namespace cv;

/*
class Vec3f{
public:

	Vec3f(){
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	}

	Vec3f(float x, float y, float z){
		this->x = x;
		this->y = y;
		this->z = z;
	}

	Vec3f(const Vec3f &vec3f){
		x = vec3f.x;
		y = vec3f.y;
		z = vec3f.z;
	}

	Vec3f &operator=(const Vec3f &vec3f){
		x = vec3f.x;
		y = vec3f.y;
		z = vec3f.z;
		return *this;
	}

	Vec3f operator-(Vec3f &vec3f){
		float _x = x - vec3f.x;
		float _y = y - vec3f.y;
		float _z = z - vec3f.z;

		return Vec3f(_x, _y, _z);
	}

	float x;
	float y;
	float z;
};

*/
class Line{
public:
	Line(){
		this->p = Vec3f(0, 0, 0);
		this->q = Vec3f(0, 0, 0);
	}

	Line(Vec3f p, Vec3f q){
		this->p = p;
		this->q = q;
	}

	Line(const Line &line){
		p = line.p;
		q = line.q;
	}

	Line &operator=(Line &line){
		this->p = line.p;
		this->q = line.q;

		return *this;
	}

public:
	Vec3f p;
	Vec3f q;
};

class face{
public:
	face(Vec3f n, Vec3f p[3]){
		this->n = n;
		this->p[0] = p[0];
		this->p[1] = p[1];
		this->p[2] = p[2];
	}

	float min_x(){
		float x = p[0][0];
		x = x < p[1][0] ? x : p[1][0];
		x = x < p[2][0] ? x : p[2][0];

		return x;
	}

	float max_x(){
		float x = p[0][0];
		x = x > p[1][0] ? x : p[1][0];
		x = x > p[2][0] ? x : p[2][0];

		return x;
	}


	float min_y(){
		float y = p[0][1];
		y = y < p[1][1] ? y : p[1][1];
		y = y < p[2][1] ? y : p[2][1];

		return y;
	}

	float max_y(){
		float y = p[0][1];
		y = y > p[1][1] ? y : p[1][1];
		y = y > p[2][1] ? y : p[2][1];

		return y;
	}

	float min_z(){
		float z = p[0][2];
		z = z < p[1][2] ? z : p[1][2];
		z = z < p[2][2] ? z : p[2][2];

		return z;
	}

	float max_z(){
		float z = p[0][2];
		z = z > p[1][2] ? z : p[1][2];
		z = z > p[2][2] ? z : p[2][2];

		return z;
	}

	

	Vec3f n;
	Vec3f p[3];
};

class MyWidget: public QGLWidget{
	Q_OBJECT

public:
	MyWidget(QWidget *parent = 0);
	~MyWidget();

public:  
	void initializeGL();  
	void resizeGL(int w, int h);  
	void paintGL();

public:
	//鼠标事件
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void wheelEvent(QWheelEvent *event);

public:
	void readModel(QString &filePath);
	void body_scaleChanged(float value);
	void body_rxChanged(float value);
	void body_ryChanged(float value);
	void body_rzChanged(float value);
	void body_mxChanged(float value);
	void body_myChanged(float value);
	void body_mzChanged(float value);
	void body_parameter_chanegd();
	void conversion_of_coordinates(Vec3f &from, Vec3f &to, float scale, float rx, float ry, float rz);
	float min_y();
	float max_y();
	cv::Rect getRect();
	cv::Mat getGFloor();
	cv::Mat slice(float y = -50.0, bool fill = false);
	

	void clear(std::vector<face *>& faces);
	void initialize();
	void drawCoordinates();

	//计算平面y 与三角面片的交线
	bool CalPlaneLineIntersectPoint(Vec3f p[3], float y, std::vector<Vec3f> &result);
	void wcl_to_screen(double cx, double cy, double cz, double &sx, double &sy);

private:  
	std::vector<face *> faces;			//三角形面片，原始数据
	std::vector<face *> faces_clone;	//三角形面片，显示
	std::vector<Line *> lines;			//显示切割线

	//鼠标事件
	//场景变换参数
	float m_xRotation, m_yRotation, m_zRotation;
	float m_xTranslation, m_yTranslation, m_zTranslation;
	float m_Scaling;
	float m_x, m_y, m_z;
	GLdouble modelMatrix[16];
	GLdouble projMatrix[16];
	GLint viewport[4];

	//鼠标点击的上一个点和当前点
	QPointF lastPoint;
	QPointF curPoint;

	//物体变换参数
	float body_scale;			//物体大小变换
	float body_rx, body_ry, body_rz;			//物体旋转变换
	float body_mx, body_my, body_mz;			//物体位移
	

	//坐标系常量
	static const int SEMI_LENGTH_SIDE = 25;
	static const int MM_PER_PIXEL = 4;
};

#endif

