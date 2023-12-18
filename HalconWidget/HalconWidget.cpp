#include "HalconWidget.h"

HalconWidget::HalconWidget(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	InitWidget();
	connect(ui.readImageBtn, &QPushButton::clicked, this, &HalconWidget::on_readImageBtn);
	connect(ui.drawCircleBtn, &QPushButton::clicked, this, &HalconWidget::on_drawCircleBtn);
	connect(ui.drawRectangleBtn, &QPushButton::clicked, this, &HalconWidget::on_drawRectangleBtn);
	connect(ui.drawRotateBtn, &QPushButton::clicked, this, &HalconWidget::on_drawRotateBtn);
	connect(ui.drawEllipseBtn, &QPushButton::clicked, this, &HalconWidget::on_drawEllipseBtn);
	connect(ui.clearROIBtn, &QPushButton::clicked, this, &HalconWidget::on_clearROIBtn);
	connect(ui.resetImageBtn, &QPushButton::clicked, this, &HalconWidget::on_resetImageBtn);
}

void HalconWidget::InitWidget()
{
	displayLabel = new CHalconLabel(this);
	ui.layout->addWidget(displayLabel);
}

void HalconWidget::on_readImageBtn()
{
	Hobject hImage;
	displayLabel->SetID();  //最好不要在主界面的构造函数内调用这个函数
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), ".", tr("Image Files (*.png *.jpg *.bmp)"));
	if (fileName.isEmpty()) {
		return;
	}
	//显示图像
	HTuple hFileName(fileName.toStdString().c_str());
	read_image(&hImage, hFileName);
	displayLabel->DisplayImage(hImage);
	displayLabel->SetPixelTracke(false);
}

void HalconWidget::on_drawCircleBtn()
{
	displayLabel->DrawCircle();
}

void HalconWidget::on_drawRectangleBtn()
{
	displayLabel->DrawRectangle();
}

void HalconWidget::on_drawRotateBtn()
{
	displayLabel->DrawRotateRectangle();
}

void HalconWidget::on_drawEllipseBtn()
{
	displayLabel->DrawEllipse();
}

void HalconWidget::on_clearROIBtn()
{
	displayLabel->ClearROI();
	displayLabel->DispalyImageROI();
}

void HalconWidget::on_resetImageBtn()
{
	displayLabel->DispalyImageROI();
}