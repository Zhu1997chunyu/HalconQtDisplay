/**
@file                 CHalconLabel.h
@brief                ��Halcon��QLabel����

����Halocnͼ�����ʾ��ROI����
@author               ZHU
@version              V1.0  2023.10.15
@updateTime           2023.10.15
@modified by          ZHU
@modified content     ��������϶��¼�
*/
#pragma once

#include <qlabel.h>
#include <QWheelEvent>
#include <qtimer.h>
#include "cpp\HalconCpp.h"   //halcon10��������ز���
#include "cpp\HOperator.h"
#include "Halcon.h"
#include "include\HSync.h"

using namespace Halcon;

#pragma execution_character_set("utf-8")

class CHalconLabel : public QLabel
{
	Q_OBJECT

public:
	CHalconLabel(QWidget *parent = Q_NULLPTR);
	~CHalconLabel();

protected:
	void resizeEvent(QResizeEvent *ev);            //��ʾ����---�ߴ�����¼�
	void wheelEvent(QWheelEvent* ev);              //��ʾ����---�����������¼�
	void mousePressEvent(QMouseEvent* ev);         //��ʾ����---��갴���¼�
	void mouseReleaseEvent(QMouseEvent* ev);       //��ʾ����---����ͷ��¼�
	void mouseMoveEvent(QMouseEvent* ev);          //��ʾ����---����ƶ��¼�(���������)

public:
	void SetID();                                  //����ID����ʾ����
	void SetPixelTracke(bool);                     //����/�ر�ʵʱ��ȡͼ�����������µĻҶ�ֵ
	void DisplayImage(Hobject  hDisplayImage);     //��ʾͼ��(ֻ��ʾͼ��)
	void ResetDisplayImage();                      //�ָ���ʾ(ֻ��ʾͼ��)
	void DrawCircle();                             //����Բ
	void DrawRectangle();                          //���ƾ���
	void DrawRotateRectangle();                    //������ת����
	void DrawEllipse();                            //������Բ

	void ClearROI();                               //������е�ROI����
	void DispalyImageROI();                        //��ʾͼ��(ͬʱ��ʾͼ���ROI����)
	void DisplayRegion();                          //ֻ��ʾROI�����ڵ�ͼ��
	Hobject GetRegion();                           //��ȡROI����

private:
	HTuple m_hLabelID;						//��ǰQLabel�ؼ�id
	HTuple m_hHalconID = NULL;				//Halcon��ʾ����id
	Hobject m_drawnRegion;                  //��ͼ����
	Hobject  ho_ImageZoom;                  //���ŵ�ͼ��
	Hobject  hCurrentImage;                 //��ǰ��ʾ��ͼ��
	HTuple m_tMouseDownRow, m_tMouseDownCol; //��갴��ʱ����������
	bool m_bIsMove;                         //�Ƿ�����꣬δ�����ƶ���ʵʱ��ȡ����ֵ�������ƶ����ƶ���ʾͼ��
	bool m_bIsDrawROI;                      //����ROI����ʱ������Ӧ�ƶ���ͼ�¼�
};
