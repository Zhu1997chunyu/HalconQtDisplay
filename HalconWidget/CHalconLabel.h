/**
@file                 CHalconLabel.h
@brief                绑定Halcon的QLabel界面

用于Halocn图像的显示与ROI交互
@author               ZHU
@version              V1.0  2023.10.15
@updateTime           2023.10.15
@modified by          ZHU
@modified content     增加鼠标拖动事件
*/
#pragma once

#include <qlabel.h>
#include <QWheelEvent>
#include <qtimer.h>
#include "cpp\HalconCpp.h"   //halcon10导出的相关操作
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
	void resizeEvent(QResizeEvent *ev);            //显示界面---尺寸更改事件
	void wheelEvent(QWheelEvent* ev);              //显示界面---鼠标滚轮缩放事件
	void mousePressEvent(QMouseEvent* ev);         //显示界面---鼠标按下事件
	void mouseReleaseEvent(QMouseEvent* ev);       //显示界面---鼠标释放事件
	void mouseMoveEvent(QMouseEvent* ev);          //显示界面---鼠标移动事件(有三种情况)

public:
	void SetID();                                  //设置ID与显示窗口
	void SetPixelTracke(bool);                     //开启/关闭实时获取图像像素坐标下的灰度值
	void DisplayImage(Hobject  hDisplayImage);     //显示图像(只显示图像)
	void ResetDisplayImage();                      //恢复显示(只显示图像)
	void DrawCircle();                             //绘制圆
	void DrawRectangle();                          //绘制矩形
	void DrawRotateRectangle();                    //绘制旋转矩形
	void DrawEllipse();                            //绘制椭圆

	void ClearROI();                               //清空所有的ROI区域
	void DispalyImageROI();                        //显示图像(同时显示图像和ROI区域)
	void DisplayRegion();                          //只显示ROI区域内的图像
	Hobject GetRegion();                           //获取ROI区域

private:
	HTuple m_hLabelID;						//当前QLabel控件id
	HTuple m_hHalconID = NULL;				//Halcon显示窗口id
	Hobject m_drawnRegion;                  //绘图区域
	Hobject  ho_ImageZoom;                  //缩放的图像
	Hobject  hCurrentImage;                 //当前显示的图像
	HTuple m_tMouseDownRow, m_tMouseDownCol; //鼠标按下时的行列坐标
	bool m_bIsMove;                         //是否按下鼠标，未按下移动，实时获取坐标值，按下移动，移动显示图像
	bool m_bIsDrawROI;                      //绘制ROI区域时不再响应移动绘图事件
};
