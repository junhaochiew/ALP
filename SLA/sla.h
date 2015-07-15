#ifndef SLA_H
#define SLA_H

#include <QtGui/QMainWindow>
#include <QTimer>
#include <vector>
#include "ui_sla.h"
#include "Serial/win_qextserialport.h"
#include "projector.h"
#include "MyWidget.h"

enum PRINTERMODE {NORMAL, PRINTING, PAUSE, STOP};


class SLAFile{
public:

	SLAFile(){
		prefix = "";
		filename = "";
		fileroot = "";
	}
	SLAFile(QString filepath){
		fileroot = filepath.left(filepath.lastIndexOf('/') + 1);
		filename = filepath.mid(filepath.lastIndexOf('/') + 1, filepath.lastIndexOf('.') - filepath.lastIndexOf('/') - 1);
		prefix = filepath.right(filepath.length() - filepath.lastIndexOf('.') - 1);
	}

	QString prefix;
	QString filename;
	QString fileroot;
};

//G���룬��ӡ����
class Gcode{
public:
	Gcode(QString line){
		this->line = line;
		words = line.split(" ");
	}
	QString		line;			//ԭ���룬��
	QStringList words;			//���ʷָ�

};

class SLA : public QMainWindow
{
	Q_OBJECT

public:
	SLA(QWidget *parent = 0, Qt::WFlags flags = 0);
	~SLA();

//��Ա����
public:
	void parainit();

//��Ӧ����
public slots:
	//setting view
	void cbComChanged(const QString& comName);			//�˿ںŸ�����Ӧ
	void cbBaudChanged(const QString& baudName);		//�����ʸ�����Ӧ
	void btnConnectClicked();							//���Ӷ˿�
	void btnDisconnectClicked();						//�Ͽ��˿�
	void btnSaveParameterClicked();						//�����ӡ��������
	void btnCancelParameterClicked();					//ȡ������
	void btnLoadClicked();								//����ģ��
	void btnGenerateClicked();							//����G Code

	//model layer
	void btnLoadModelClicked();							//����ģ��
	void btnSliceClicked();								//�и�
	void eScaleRateChecked();							//�ı������
	void eRxChecked();									//��ת�Ƕȸ���
	void eRyChecked();
	void eRzChecked();
	void eMxChecked();									//��ת�Ƕȸ���
	void eMyChecked();
	void eMzChecked();

	//comtrol view
	void sliderUpChanged(int value);					//����
	void sliderDownChanged(int value);					//����
	void btnUpClicked();								//Up��ť
	void btnDownClicked();								
	void btnUpHomeClicked();
	void btnDownHomeClicked();
	void btnLeftClicked();
	void btnRightClicked();
	void btnStartClicked();
	void btnStopClicked();
	void btnPauseClicked();
	void btnContinueClicked();


	//projector view
	void btnStartProjectorClicked();
	void btnEndProjectorClicked();
	void btnSaveCalibrationClicked();					//�����ӡ������
	void btnCancelCalibrationClicked();					//ȡ����ӡ����������

	//read & write
	void readMyCom();
	void writeMyCom(QString &qdata);
	void parsingGcode();								//����Gcode�����г���

private:
	void closeEvent(QCloseEvent *event);
	void warning(QString &warning);						//��ʾ����
	void parsing(QString &receiveMsg);					//����оƬ���͹������ַ���

private:
	Ui::SLAClass ui;

//˽�����ݳ�Ա
private:
	//�˿�ͨ��
	Win_QextSerialPort *myCom;

	//�˿���Ϣ
	QString comPort;						//�˿ں�
	BaudRateType baudRate;					//������
	DataBitsType dataBits; //= DATA_8;		//����λ
	ParityType parity; //= PAR_NONE;		//У��λ
	StopBitsType stopBits; //= STOP_1;		//ֹͣλ
	FlowType flowControl; //= FLOW_OFF;		//����������
	long timeout_Millisec; //= 500;			//��ʱ����

	//ͶӰ������
	float ph_xPos; //= 500.0f;
	float ph_yPos; //= 500.0f;
	float ph_width; //= 1024.0f;
	float ph_height; //= 768.0f;

	//��ӡ��������
	float thickness; //=0.1 mm;
	int exposuretime; //=3000 ms;
	int supportinglayer; //=2
	int SLexposuretime; //=8000 ms

	//���Ʋ����
	int totalLayer;

	//���ݶ�ȡ
	QString receiveMsg;						//������Ϣ

	//��ӡ��
	Projector *projector;					//��ӡ������
	MyWidget *myWidget;						//��ά��ʾ

	//���Ʊ���
	bool isWaiting;							//�ȴ�оƬ����
	PRINTERMODE mode;						//����״̬
	QTimer *timer;							//ʱ��

	//G������
	SLAFile slafile;
	int currentGcodeLine;					//����������
	std::vector<Gcode> Gcodes;				//G����

	//����������
	static const int STEP_PER_MM = 200;		//ÿ����1mm����Ҫ���ת200�Ρ�
};

#endif // SLA_H
