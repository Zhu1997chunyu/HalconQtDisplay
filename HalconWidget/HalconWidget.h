#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_HalconWidget.h"
#include <QFileDialog>
#include "CHalconLabel.h"   //自定义的halcon显示控件

class HalconWidget : public QMainWindow
{
    Q_OBJECT

public:
    HalconWidget(QWidget *parent = Q_NULLPTR);
	void InitWidget();

private slots:
    void on_readImageBtn();
	void on_drawCircleBtn();
	void on_drawRectangleBtn();
	void on_drawRotateBtn();
	void on_drawEllipseBtn();
	void on_clearROIBtn();
	void on_resetImageBtn();

private:
    Ui::HalconWidgetClass ui;
	CHalconLabel *displayLabel;
};
