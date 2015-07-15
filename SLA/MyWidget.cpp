#include "MyWidget.h"  
#include <QFile>
#include <math.h> 
#include <GL/glut.h>
#include <QPushButton>

#include "Calculator.h"
//opencv

#include <iostream>
using namespace std;
using namespace cv;

MyWidget::MyWidget(QWidget *parent /* = 0 */)  
	: QGLWidget(parent)  
{  
	initialize();
}  

MyWidget::~MyWidget()  
{  

}  

void MyWidget::initializeGL()  
{  
	glShadeModel(GL_SMOOTH);  

	glClearColor(1.0,1.0,1.0,1.0);  
	glClearDepth(1.0);  

	glEnable(GL_DEPTH_TEST);  

	glDepthFunc(GL_LEQUAL);  
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST); 

	GLfloat sun_light_position[] = {0.0f, 0.0f, 0.0f, 1.0f};
	GLfloat sun_light_ambient[] = {0.1f, 0.1f, 0.1f, 1.1f};
	GLfloat sun_light_diffuse[] = {0.7f, 0.0f, 0.0f, 1.0f};
	GLfloat sun_light_specular[] = {0.5 , 0.0 , 0.0 , 0.5};
	glLightfv(GL_LIGHT0, GL_POSITION, sun_light_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, sun_light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, sun_light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, sun_light_specular);

	GLfloat sun_mat_ambient[] = {0.5f, 0.4f, 0.1f, 1.0f};
	GLfloat sun_mat_diffuse[] = {0.6f, 0.2f, 0.2f, 1.0f};
	GLfloat sun_mat_specular[] = {0.2f, 0.3f, 0.4f, 1.0f};
	GLfloat sun_mat_emission[] = {1.0f, 0.5f, 0.5f, 1.0f};
	GLfloat sun_mat_shininess = 0.5f;
	glMaterialfv(GL_FRONT, GL_AMBIENT, sun_mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, sun_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, sun_mat_specular);
	glMaterialfv(GL_FRONT, GL_EMISSION, sun_mat_emission);
	glMaterialf (GL_FRONT, GL_SHININESS, sun_mat_shininess);
	glutSolidSphere(2.0, 40, 32);
}  

void MyWidget::resizeGL(int w, int h)  
{  
	glViewport(0,0,(GLint)width(),(GLint)height());  

	glMatrixMode(GL_PROJECTION);  
	glLoadIdentity();  

	gluPerspective(45.0,(GLfloat)width() / (GLfloat)height(),0.1,1000.0);  
	glMatrixMode(GL_MODELVIEW);  

	glLoadIdentity();  
}  

void MyWidget::paintGL()  
{  

	//第一版
	/*glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  
	glLoadIdentity();
	glTranslatef(m_xTranslation, m_yTranslation, m_zTranslation);
	glTranslatef(m_x, m_y, m_z);
	glRotatef(m_xRotation,1.0f,0.0f,0.0f);
	glRotatef(m_yRotation,0.0f,1.0f,0.0f);
	glRotatef(m_zRotation,0.0f,0.0f,1.0f);
	glScalef(m_Scaling, m_Scaling, m_Scaling);
	glTranslatef(-m_x, -m_y, -m_z);

	glColor3f(1.0, 0.5, 0.5);

	for(int i = 0; i < faces.size(); i ++){
	glBegin(GL_TRIANGLES);
	glVertex3f(faces[i]->p[0][0], faces[i]->p[0][1],faces[i]->p[0][2]);
	glVertex3f(faces[i]->p[1][0], faces[i]->p[1][1],faces[i]->p[1][2]);
	glVertex3f(faces[i]->p[2][0], faces[i]->p[2][1],faces[i]->p[2][2]);
	glEnd();  
	}

	glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
	glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
	glGetIntegerv(GL_VIEWPORT, viewport);*/


	//第二版
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  
	glLoadIdentity();
	glTranslatef(m_xTranslation, m_yTranslation, m_zTranslation);
	glRotatef(m_xRotation,1.0f,0.0f,0.0f);
	glRotatef(m_yRotation,0.0f,1.0f,0.0f);
	glRotatef(m_zRotation,0.0f,0.0f,1.0f);
	glScalef(m_Scaling, m_Scaling, m_Scaling);

	//坐标系
	drawCoordinates();

	//物体
	glEnable(GL_LIGHT0);   //支持光源GL_LIGHT0---GL_LIGHT7
	glEnable(GL_LIGHTING);
	for(int i = 0; i < faces.size(); i ++){
		glBegin(GL_TRIANGLES);
		glVertex3f(faces_clone[i]->p[0][0], faces_clone[i]->p[0][1],faces_clone[i]->p[0][2]);
		glVertex3f(faces_clone[i]->p[1][0], faces_clone[i]->p[1][1],faces_clone[i]->p[1][2]);
		glVertex3f(faces_clone[i]->p[2][0], faces_clone[i]->p[2][1],faces_clone[i]->p[2][2]);
		glEnd();
	}
	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHTING);

	glColor3f(0.0, 0.0, 0.0);

	glPushAttrib(GL_LINE_BIT);
	double gra, range[2];
	glGetDoublev(GL_LINE_WIDTH_GRANULARITY, &gra);
	glGetDoublev(GL_LINE_WIDTH_RANGE, range);
	glLineWidth(range[0] + 10 * gra);
	//x轴1
	for(int i = 0; i < lines.size(); i ++){
		glBegin(GL_LINES);
		glVertex3f(lines[i]->p[0], lines[i]->p[1], lines[i]->p[2]);
		glVertex3f(lines[i]->q[0], lines[i]->q[1], lines[i]->q[2]);
		glEnd();
	}
	glPopAttrib();


	glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
	glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
	glGetIntegerv(GL_VIEWPORT, viewport);
} 

void MyWidget::mousePressEvent(QMouseEvent *event){
	lastPoint = event->posF();
}

void MyWidget::mouseMoveEvent(QMouseEvent *event){
	curPoint = event->posF();

	double sx,sy;
	double cx,cy;
	wcl_to_screen(0.0,0.0,0.0,sx,sy);
	wcl_to_screen(0.0,1.0,0.0,cx,cy);

	m_xRotation -= (float)(lastPoint.y()- curPoint.y()) / 2;
	m_yRotation = cy >= sy ? m_yRotation + (float)(lastPoint.x() - curPoint.x()) / 2 : m_yRotation - (float)(lastPoint.x() - curPoint.x()) / 2;

	lastPoint = curPoint;
	update();
}

void MyWidget::mouseReleaseEvent(QMouseEvent *event){
	curPoint = event->posF();
	lastPoint = curPoint;
}

void MyWidget::wheelEvent(QWheelEvent *event){
	int numDegrees = event->delta() / 8;
	int numSteps = numDegrees / 15;
	if (numSteps > 0)
	{
		m_Scaling *= 1.25;
		if (m_Scaling < 0.00001) 
			m_Scaling = 0.00001f;
	}
	else 
	{
		m_Scaling *= 0.85;
		if (m_Scaling < 0.00001) 
			m_Scaling = 0.00001f;
	}
	update();
}

void MyWidget::readModel(QString &filePath){
	//清空上个模型
	clear(faces);
	clear(faces_clone);

	float min_x = 100000, min_y = 100000, min_z = 1000000;
	float max_x = -100000, max_y = -100000, max_z = -1000000;
	//read
	QFile file(filePath);
	file.open(QIODevice::ReadOnly);


	char head[80];					//文件头
	int unTriangles = 0;			//三角面片个数
	float x, y, z;					//法线矢量，坐标点
	char unusedByte;				//无用数据

	//读取文件头
	file.read((char*)&head, sizeof(head));
	//printf("%s\n", head);

	//面片个数
	file.read((char*)&unTriangles, sizeof(unTriangles));
	//printf("face number = %d\n", unTriangles);

	if(unTriangles == 0){
		file.close();
		return;
	}
	
	//读取面片信息
	for(int i = 0; i < unTriangles; i ++){
		Vec3f nx;
		Vec3f p[3];

		//printf("\n");
		for (int j = 0; j < 4; j ++){
			file.read((char*)&x, sizeof(x));
			file.read((char*)&y, sizeof(y));
			file.read((char*)&z, sizeof(z));
			//printf("x = %f, y = %f, z = %f\n", x, y, z);

			if(j == 0){
				nx = Vec3f(x, y, z);
			} else {
				p[j - 1] = Vec3f(x, y, z);

				min_x = min_x < x ? min_x : x;
				min_y = min_y < y ? min_y : y;
				min_z = min_z < z ? min_z : z;

				max_x = max_x > x ? max_x : x;
				max_y = max_y > y ? max_y : y;
				max_z = max_z > z ? max_z : z;
			}
		}
		//无用数据位
		file.read((char*)&unusedByte, sizeof(unusedByte));
		file.read((char*)&unusedByte, sizeof(unusedByte));

		//存储
		faces.push_back(new face(nx, p));
	}
	file.close();

	m_x = (min_x + max_x) / 2;
	m_y = (min_y + max_y) / 2;
	m_z = (min_z + max_z) / 2;
	
	//数据更新和处理
	for(int i = 0; i < faces.size(); i ++){
		Vec3f vec3f(m_x, m_y, m_z);
		faces[i]->p[0] = faces[i]->p[0] - vec3f;
		faces[i]->p[1] = faces[i]->p[1] - vec3f;
		faces[i]->p[2] = faces[i]->p[2] - vec3f;
	}
	min_x -= m_x; max_x -= m_x;
	min_y -= m_y; max_y -= m_y;
	min_z -= m_z; max_z -= m_z;

	for(int i = 0; i < faces.size(); i ++){
		Vec3f vec3f(0, SEMI_LENGTH_SIDE + min_y, 0);

		Vec3f p[3];
		p[0] = faces[i]->p[0] - vec3f;
		p[1] = faces[i]->p[1] - vec3f;
		p[2] = faces[i]->p[2] - vec3f;

		faces_clone.push_back(new face(faces[i]->n, p));
	}

	update();
}

void MyWidget::clear(vector<face *> &faces){
	for(int i = 0; i < faces.size(); i ++){
		delete faces[i];
	}
	faces.clear();
}

void MyWidget::body_scaleChanged(float value){
	body_scale = value;
	body_parameter_chanegd();
}

void MyWidget::body_rxChanged(float value){
	body_rx = value;
	body_parameter_chanegd();
}

void MyWidget::body_ryChanged(float value){
	body_ry = value;
	body_parameter_chanegd();
}

void MyWidget::body_rzChanged(float value){
	body_rz = value;
	body_parameter_chanegd();
}

void MyWidget::body_mxChanged(float value){
	body_mx = value;
	body_parameter_chanegd();
}

void MyWidget::body_myChanged(float value){
	body_my = value;
	body_parameter_chanegd();
}

void MyWidget::body_mzChanged(float value){
	body_mz = value;
	body_parameter_chanegd();
}

void MyWidget::body_parameter_chanegd(){
	clear(faces_clone);

	float min_y = 10000;
	for(int i = 0; i < faces.size(); i ++){
		Vec3f p[3];
		for(int j = 0; j < 3; j ++){
			conversion_of_coordinates(faces[i]->p[j], p[j], body_scale, body_rx, body_ry, body_rz);
			min_y = min_y < p[j][1] ? min_y : p[j][1];
		}
		faces_clone.push_back(new face(faces[i]->n, p));
	}

	printf("min_y = %f, body_my = %f\n", min_y, body_my);

	Vec3f move_vec3f = (body_mx, body_my, body_mz);
	for(int i = 0; i < faces_clone.size(); i ++){
		Vec3f vec3f(body_mx, body_my - min_y - SEMI_LENGTH_SIDE, body_mz);

		faces_clone[i]->p[0] = faces_clone[i]->p[0] + vec3f;
		faces_clone[i]->p[1] = faces_clone[i]->p[1] + vec3f;
		faces_clone[i]->p[2] = faces_clone[i]->p[2] + vec3f;
	}
	update();
}

void MyWidget::conversion_of_coordinates(Vec3f &from, Vec3f &to, float scale, float rx, float ry, float rz){
	float PI = 3.1415926535f;

	float sinx = sin(rx / 180 * PI);
	float cosx = cos(rx / 180 * PI);
	float siny = sin(ry / 180 * PI);
	float cosy = cos(ry / 180 * PI);
	float sinz = sin(rz / 180 * PI);
	float cosz = cos(rz / 180 * PI);

	float x = from[0] * scale, y = from[1] * scale, z = from[2] * scale;

	to[0] = x * cosy * cosz + y * (sinx * siny * cosz - cosx * sinz) + z * (cosx * siny * cosz + sinx * sinz);
	to[1] = x * cosy * sinz + y * (sinx * siny * sinz + cosx * cosz) + z * (cosx * siny * sinz - sinx * cosz);
	to[2] = -x * siny + y * sinx * cosy + z * cosx * cosy;
}

Mat MyWidget::getGFloor(){
	Rect rect = getRect();
	Mat GFloor(2 * SEMI_LENGTH_SIDE * MM_PER_PIXEL, 2 * SEMI_LENGTH_SIDE * MM_PER_PIXEL, CV_8UC1, Scalar(255));
	//Mat roi = GFloor(rect);
	//Mat mat(rect.height, rect.width, CV_8UC1, Scalar(255));
	//mat.copyTo(roi);

	return GFloor;
}

Mat MyWidget::slice(float y, bool fill){
	if(y < -50 || y > 50){
		printf("out of range!\n");
	}

	for(int i = 0; i < lines.size(); i ++){
		delete lines[i];
	}
	lines.clear();

	for(int i = 0; i < faces_clone.size(); i ++){
		face &f = *faces_clone[i];

		if(f.min_y() > y || f.max_y() < y){
			continue;
		}

		vector<Vec3f> result;
		if(CalPlaneLineIntersectPoint(faces_clone[i]->p, y, result)){
			lines.push_back(new Line(result[0], result[1]));
		}
	}

	update();

	Mat mat(2 * SEMI_LENGTH_SIDE * MM_PER_PIXEL, 2 * SEMI_LENGTH_SIDE * MM_PER_PIXEL, CV_8UC1, Scalar(0));
	
	for(int i = 0; i < lines.size(); i ++){
		line(mat, Point2f((lines[i]->p[0] + SEMI_LENGTH_SIDE) * MM_PER_PIXEL, (lines[i]->p[2] + SEMI_LENGTH_SIDE) * MM_PER_PIXEL), Point2f((lines[i]->q[0] + SEMI_LENGTH_SIDE) * MM_PER_PIXEL, (lines[i]->q[2] + SEMI_LENGTH_SIDE) * MM_PER_PIXEL), Scalar(255));
	}

	
	if(fill){
		//内部填充
		int m = mat.rows;
		int n = mat.cols;

		int **data = new int*[m];
		for(int i = 0; i < m; i ++){
			data[i] = new int[n];
			for(int j = 0; j < n; j ++){
				int v = mat.at<unsigned char>(i, j);
				data[i][j] = v;
			}
		}

		for(int i = 0; i < m; i ++){
			for (int j = 0; j < n; j ++){
				if(Calculator::pointInContours(data, m, n, Point(i, j))){
					mat.at<unsigned char>(i, j) = 255;
				}
			}
		}

		for(int i = 0; i < m; i ++){
			delete[] data[i];
		}
		delete[] data;

		//腐蚀
		int erosion_size = 1;
		Mat element1 = getStructuringElement( MORPH_CROSS, Size( 2 * erosion_size + 1 , 2 * erosion_size + 1 ), Point( erosion_size, erosion_size ));  
		//膨胀
		int dilation_size = 1;
		Mat element2 = getStructuringElement( MORPH_CROSS, Size( 2 * dilation_size + 1, 2 * dilation_size + 1 ), Point( dilation_size,  dilation_size));  

		erode ( mat, mat, element1 );		//腐蚀
		dilate( mat, mat, element2 );		//膨胀
		//颜色翻转
		for(int i = 0; i < m; i ++){
			for (int j = 0; j < n; j ++){
				mat.at<unsigned char>(i, j) = abs(mat.at<unsigned char>(i, j) - 255);
			}
		}
		erode ( mat, mat, element1 );		//腐蚀
		dilate( mat, mat, element2 );		//膨胀
		//颜色翻转
		for(int i = 0; i < m; i ++){
			for (int j = 0; j < n; j ++){
				mat.at<unsigned char>(i, j) = abs(mat.at<unsigned char>(i, j) - 255);
			}
		}
	} else {
		//膨胀
		int dilation_size = 1;
		Mat element = getStructuringElement( MORPH_CROSS, Size( 2 * dilation_size + 1, 2 * dilation_size + 1 ), Point( dilation_size,  dilation_size));  
		dilate( mat, mat, element );		//膨胀
	}

	

	return mat;
}

float MyWidget::min_y(){
	if(faces_clone.size() == 0)
		return 0;

	float min_y = SEMI_LENGTH_SIDE;
	for(int i = 0; i < faces_clone.size(); i++){
		min_y = min_y < faces_clone[i]->min_y() ? min_y : faces_clone[i]->min_y();
	}

	min_y = min_y >= -SEMI_LENGTH_SIDE ? min_y : -SEMI_LENGTH_SIDE;

	return min_y;
}

float MyWidget::max_y(){
	if(faces_clone.size() == 0)
		return 0;

	float max_y = -SEMI_LENGTH_SIDE;
	for(int i = 0; i < faces_clone.size(); i ++){
		max_y = max_y > faces_clone[i]->max_y() ? max_y : faces_clone[i]->max_y();
	}

	return max_y;
}

Rect MyWidget::getRect(){
	if(faces_clone.size() == 0)
		return Rect(0, 0, 0, 0);

	float max_x = -SEMI_LENGTH_SIDE;
	float max_z = -SEMI_LENGTH_SIDE;
	float min_x = SEMI_LENGTH_SIDE;
	float min_z = SEMI_LENGTH_SIDE;

	for(int i = 0; i < faces_clone.size(); i ++){
		max_x = max_x > faces_clone[i]->max_x() ? max_x : faces_clone[i]->max_x();
		max_z = max_z > faces_clone[i]->max_z() ? max_z : faces_clone[i]->max_z();

		min_x = min_x < faces_clone[i]->min_x() ? min_x : faces_clone[i]->min_x();
		min_z = min_z < faces_clone[i]->min_z() ? min_z : faces_clone[i]->min_z();
	}

	Rect rect((min_x + SEMI_LENGTH_SIDE) * MM_PER_PIXEL, (min_z + SEMI_LENGTH_SIDE) * MM_PER_PIXEL, (max_x - min_x) * MM_PER_PIXEL, (max_z - min_z) * MM_PER_PIXEL);
	return rect;
}

void MyWidget::initialize(){
	clear(faces);
	clear(faces_clone);
	m_xTranslation = 0.0f;
	m_yTranslation = 0.0f;
	m_zTranslation = -70.0f;

	m_xRotation = 0.0f;
	m_yRotation = 0.0f;
	m_zRotation = 0.0f;

	m_Scaling = 1.0f;

	//物体中心点
	m_x = 0.0f;
	m_y = 0.0f;
	m_z = 0.0f;

	//物体参数,大小和旋转角度
	body_scale = 1.0;
	body_rx = 0;
	body_ry = 0;
	body_rz = 0;
	body_mx = 0;
	body_my = 0;
	body_mz = 0;
}

void MyWidget::drawCoordinates(){
	glColor3f(0.3, 0.3, 0.3);

	//x轴1
	glBegin(GL_LINES);
	glVertex3f(-SEMI_LENGTH_SIDE, -SEMI_LENGTH_SIDE, -SEMI_LENGTH_SIDE);
	glVertex3f(SEMI_LENGTH_SIDE, -SEMI_LENGTH_SIDE, -SEMI_LENGTH_SIDE);
	glEnd();
	//x轴2
	glBegin(GL_LINES);
	glVertex3f(-SEMI_LENGTH_SIDE, SEMI_LENGTH_SIDE, -SEMI_LENGTH_SIDE);
	glVertex3f(SEMI_LENGTH_SIDE, SEMI_LENGTH_SIDE, -SEMI_LENGTH_SIDE);
	glEnd();
	//x轴3
	glBegin(GL_LINES);
	glVertex3f(-SEMI_LENGTH_SIDE, -SEMI_LENGTH_SIDE, SEMI_LENGTH_SIDE);
	glVertex3f(SEMI_LENGTH_SIDE, -SEMI_LENGTH_SIDE, SEMI_LENGTH_SIDE);
	glEnd();
	//x轴4
	glBegin(GL_LINES);
	glVertex3f(-SEMI_LENGTH_SIDE, SEMI_LENGTH_SIDE, SEMI_LENGTH_SIDE);
	glVertex3f(SEMI_LENGTH_SIDE, SEMI_LENGTH_SIDE, SEMI_LENGTH_SIDE);
	glEnd();

	//y轴1
	glBegin(GL_LINES);
	glVertex3f(-SEMI_LENGTH_SIDE, -SEMI_LENGTH_SIDE, -SEMI_LENGTH_SIDE);
	glVertex3f(-SEMI_LENGTH_SIDE, SEMI_LENGTH_SIDE, -SEMI_LENGTH_SIDE);
	glEnd();
	//y轴2
	glBegin(GL_LINES);
	glVertex3f(SEMI_LENGTH_SIDE, -SEMI_LENGTH_SIDE, -SEMI_LENGTH_SIDE);
	glVertex3f(SEMI_LENGTH_SIDE, SEMI_LENGTH_SIDE, -SEMI_LENGTH_SIDE);
	glEnd();
	//y轴3
	glBegin(GL_LINES);
	glVertex3f(-SEMI_LENGTH_SIDE, -SEMI_LENGTH_SIDE, SEMI_LENGTH_SIDE);
	glVertex3f(-SEMI_LENGTH_SIDE, SEMI_LENGTH_SIDE, SEMI_LENGTH_SIDE);
	glEnd();
	//y轴4
	glBegin(GL_LINES);
	glVertex3f(SEMI_LENGTH_SIDE, -SEMI_LENGTH_SIDE, SEMI_LENGTH_SIDE);
	glVertex3f(SEMI_LENGTH_SIDE, SEMI_LENGTH_SIDE, SEMI_LENGTH_SIDE);
	glEnd();

	//z轴1
	glBegin(GL_LINES);
	glVertex3f(-SEMI_LENGTH_SIDE, -SEMI_LENGTH_SIDE, -SEMI_LENGTH_SIDE);
	glVertex3f(-SEMI_LENGTH_SIDE, -SEMI_LENGTH_SIDE, SEMI_LENGTH_SIDE);
	glEnd();
	//z轴2
	glBegin(GL_LINES);
	glVertex3f(SEMI_LENGTH_SIDE, -SEMI_LENGTH_SIDE, -SEMI_LENGTH_SIDE);
	glVertex3f(SEMI_LENGTH_SIDE, -SEMI_LENGTH_SIDE, SEMI_LENGTH_SIDE);
	glEnd();
	//z轴3
	glBegin(GL_LINES);
	glVertex3f(-SEMI_LENGTH_SIDE, SEMI_LENGTH_SIDE, -SEMI_LENGTH_SIDE);
	glVertex3f(-SEMI_LENGTH_SIDE, SEMI_LENGTH_SIDE, SEMI_LENGTH_SIDE);
	glEnd();
	//z轴4
	glBegin(GL_LINES);
	glVertex3f(SEMI_LENGTH_SIDE, SEMI_LENGTH_SIDE, -SEMI_LENGTH_SIDE);
	glVertex3f(SEMI_LENGTH_SIDE, SEMI_LENGTH_SIDE, SEMI_LENGTH_SIDE);
	glEnd();


	//画底面
	glColor3f(0.8, 0.8, 0.8);
	glBegin(GL_QUADS);
	glVertex3f(-SEMI_LENGTH_SIDE, -SEMI_LENGTH_SIDE, -SEMI_LENGTH_SIDE);
	glVertex3f(SEMI_LENGTH_SIDE, -SEMI_LENGTH_SIDE, -SEMI_LENGTH_SIDE);
	glVertex3f(SEMI_LENGTH_SIDE, -SEMI_LENGTH_SIDE, SEMI_LENGTH_SIDE);
	glVertex3f(-SEMI_LENGTH_SIDE, -SEMI_LENGTH_SIDE, SEMI_LENGTH_SIDE);
	glEnd();


	////画坐标方向
	//glColor3f(1.0, 0, 0);
	////x正方向
	//glBegin(GL_LINES);
	//glVertex3f(0, -SEMI_LENGTH_SIDE, 0);
	//glVertex3f(1.2 * SEMI_LENGTH_SIDE, -SEMI_LENGTH_SIDE, 0);
	//glEnd();
	////y正方向
	//glBegin(GL_LINES);
	//glVertex3f(0, -SEMI_LENGTH_SIDE, 0);
	//glVertex3f(0, 1.2 * SEMI_LENGTH_SIDE, 0);
	//glEnd();
	////z正方向
	//glBegin(GL_LINES);
	//glVertex3f(0, -SEMI_LENGTH_SIDE, 0);
	//glVertex3f(0, -SEMI_LENGTH_SIDE, 1.2 * SEMI_LENGTH_SIDE);
	//glEnd();


}

bool MyWidget::CalPlaneLineIntersectPoint(Vec3f p[3], float y, std::vector<Vec3f> &result){
	//初始化
	result.clear();

	vector<Vec3f> sp;
	vector<Vec3f> mp;
	vector<Vec3f> bp;

	for(int i = 0; i < 3; i ++){
		if(p[i][1] < y){
			sp.push_back(p[i]);
		} else if(p[i][1] > y){
			bp.push_back(p[i]);
		} else {
			mp.push_back(p[i]);
		}
	}

	if(mp.size() == 0){

		if(sp.size() == 0 || bp.size() == 0){
			//printf("wrong face!\n");
			return false;
		}

		if(sp.size() == 1){
			float x = sp[0][0] + (y - sp[0][1]) * (bp[0][0] - sp[0][0]) / (bp[0][1] - sp[0][1]);
			float z = sp[0][2] + (y - sp[0][1]) * (bp[0][2] - sp[0][2]) / (bp[0][1] - sp[0][1]);
			result.push_back(Vec3f(x, y, z));

			x = sp[0][0] + (y - sp[0][1]) * (bp[1][0] - sp[0][0]) / (bp[1][1] - sp[0][1]);
			z = sp[0][2] + (y - sp[0][1]) * (bp[1][2] - sp[0][2]) / (bp[1][1] - sp[0][1]);
			result.push_back(Vec3f(x, y, z));
		} else {
			float x = sp[0][0] + (y - sp[0][1]) * (bp[0][0] - sp[0][0]) / (bp[0][1] - sp[0][1]);
			float z = sp[0][2] + (y - sp[0][1]) * (bp[0][2] - sp[0][2]) / (bp[0][1] - sp[0][1]);
			result.push_back(Vec3f(x, y, z));

			x = sp[1][0] + (y - sp[1][1]) * (bp[0][0] - sp[1][0]) / (bp[0][1] - sp[1][1]);
			z = sp[1][2] + (y - sp[1][1]) * (bp[0][2] - sp[1][2]) / (bp[0][1] - sp[1][1]);
			result.push_back(Vec3f(x, y, z));
		}
	} else if(mp.size() == 1){
		if(sp.size() != 1 && bp.size() != 1){
			//printf("wrong face!\n");
			return false;
		}

		result.push_back(mp[0]);

		float x = sp[0][0] + (y - sp[0][1]) * (bp[0][0] - sp[0][0]) / (bp[0][1] - sp[0][1]);
		float z = sp[0][2] + (y - sp[0][1]) * (bp[0][2] - sp[0][2]) / (bp[0][1] - sp[0][1]);
		result.push_back(Vec3f(x, y, z));

	} else if(mp.size() == 2){
		result.push_back(mp[0]);
		result.push_back(mp[1]);
	} else {
		//printf("wrong face!\n");
		return false;
	}

	return true;
}

void MyWidget::wcl_to_screen(double cx, double cy, double cz, double &sx, double &sy){
	GLdouble winx, winy, winz;
	gluProject(cx, cy, cz, modelMatrix, projMatrix, viewport, &winx, &winy, &winz); 

	sx = winx;
	sy = height() - winy;
}