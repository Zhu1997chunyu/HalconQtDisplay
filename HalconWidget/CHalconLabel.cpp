/**
@file                 CHalconLabel.cpp
@brief                绑定Halcon的QLabel界面

用于Halocn图像的显示与ROI交互
@author               ZHU
@version              V1.0  2023.10.15
@updateTime           2023.10.15
@modified by          ZHU
@modified content     增加鼠标拖动事件
*/
#include "CHalconLabel.h"

CHalconLabel::CHalconLabel(QWidget *parent)
	: QLabel(parent)
	, m_bIsMove(false)
	, m_bIsDrawROI(false)
{
	//初始化图像
	gen_empty_obj(&hCurrentImage);
	//初始化ROI区域
	gen_empty_obj(&m_drawnRegion);
	//设置文本位置---居中且上方，设置文本颜色
	this->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
	this->setStyleSheet("color: red;");
}

CHalconLabel::~CHalconLabel()
{

}

//窗体重绘事件
void CHalconLabel::resizeEvent(QResizeEvent *ev)
{
	if (m_hHalconID != NULL)
	{
		//防止窗口闪烁
		set_system("flush_graphic", "false");
		//重新显示
#if 0 //显示一,充满整个画幅
		clear_window(m_hHalconID);
		detach_background_from_window(m_hHalconID);
		HTuple imgWidth, imgHeight;
		get_image_size(ho_Image, &imgWidth, &imgHeight);
		set_part(m_hHalconID, 0, 0, imgHeight - 1, imgWidth - 1);
		attach_background_to_window(ho_Image, m_hHalconID);
#endif
#if 1 //显示二，维持原有图像比例且居中显示	
		clear_window(m_hHalconID);
		detach_background_from_window(m_hHalconID);
		int labelWidth = this->width();  //窗口尺寸
		int labelHeight = this->height();
		HTuple imgWidth, imgHeight;                 //原图尺寸
		HTuple m_scaledWidth, m_scaledHeight;       //缩放后的尺寸
		HTuple m_hvScaledRate;                      //缩放比例
										
		get_image_size(hCurrentImage, &imgWidth, &imgHeight);
		//获取缩放系数
		tuple_min2(1.0 * labelWidth / imgWidth, 1.0 * labelHeight / imgHeight, &m_hvScaledRate);
		//进行图像缩放
		zoom_image_factor(hCurrentImage, &ho_ImageZoom, m_hvScaledRate, m_hvScaledRate, "constant");
		get_image_size(ho_ImageZoom, &m_scaledWidth, &m_scaledHeight);

		if (1.0*labelWidth / imgWidth < 1.0*labelHeight / imgHeight)
		{
			set_window_extents(m_hHalconID, labelHeight / 2.0 - m_scaledHeight / 2.0, 0, labelWidth, m_scaledHeight);
		}
		else
		{
			set_window_extents(m_hHalconID, 0, labelWidth / 2.0 - m_scaledWidth / 2.0, m_scaledWidth, labelHeight);
		}
		set_part(m_hHalconID, 0, 0, imgHeight - 1, imgWidth - 1);
		attach_background_to_window(hCurrentImage, m_hHalconID);
		set_system("flush_graphic", "true");
		detach_background_from_window(m_hHalconID);
		attach_background_to_window(hCurrentImage, m_hHalconID);
		disp_obj(hCurrentImage, m_hHalconID);
#endif
	}
}

//显示界面---鼠标滚轮缩放事件
void CHalconLabel::wheelEvent(QWheelEvent* ev)
{
	double Zoom;   //放大或缩小倍率
	HTuple  mouseRow, mouseCol, Button;
	HTuple startRowBf, startColBf, endRowBf, endColBf, Ht, Wt, startRowAft, startColAft, endRowAft, endColAft;
	//滚轮前滑，放大
	if (ev->delta()>0)
	{
		Zoom = 2.0;//单步放大倍率
	}
	else//否则缩小
	{
		Zoom = 1 / 2.0;
	}
	//获取光标在原图上的位置，注意是原图坐标，不是Label下的坐标
	HTuple  hv_Exception, hv_ErrMsg;
	try
	{
		get_mposition(m_hHalconID, &mouseRow, &mouseCol, &Button);
	}
	catch (HException& HDevExpDefaultException)
	{
		return;
	}
	//获取原图显示的部分，注意也是原图坐标
	get_part(m_hHalconID, &startRowBf, &startColBf, &endRowBf, &endColBf);
	//缩放前显示的图像宽高
	Ht = endRowBf - startRowBf;
	Wt = endColBf - startColBf;
	//普通版halcon能处理的图像最大尺寸是32K*32K。如果无限缩小原图像，导致显示的图像超出限制，则会造成程序崩溃
	if (Ht*Wt<20000 * 20000 || Zoom == 2.0)
	{
		//计算缩放后的图像区域
		startRowAft = mouseRow - ((mouseRow - startRowBf) / Zoom);
		startColAft = mouseCol - ((mouseCol - startColBf) / Zoom);
		endRowAft = startRowAft + (Ht / Zoom);
		endColAft = startColAft + (Wt / Zoom);
		//如果放大过大，则返回
		if (endRowAft - startRowAft < 2)
		{
			return;
		}

		if (m_hHalconID != NULL)
		{
			//如果有图像，则先清空图像
			detach_background_from_window(m_hHalconID);
		}
		set_part(m_hHalconID, startRowAft, startColAft, endRowAft, endColAft);
		attach_background_to_window(hCurrentImage, m_hHalconID);
	}
}

//显示界面---鼠标按下事件
void CHalconLabel::mousePressEvent(QMouseEvent* ev)
{
	HTuple mouseRow, mouseCol, Button;
	try
	{
		get_mposition(m_hHalconID, &mouseRow, &mouseCol, &Button);
	}
	catch (HException)
	{
		return;
	}
	//鼠标按下时的行列坐标
	m_tMouseDownRow = mouseRow;
	m_tMouseDownCol = mouseCol;
	m_bIsMove = true;
}

//显示界面---鼠标释放事件
void CHalconLabel::mouseReleaseEvent(QMouseEvent* ev)
{
	m_bIsMove = false;
}

//显示界面---鼠标移动事件(有三种情况)
void CHalconLabel::mouseMoveEvent(QMouseEvent* ev)
{
	//情况一:鼠标绘制ROI区域时，不响应鼠标移动事件
	if (m_bIsDrawROI) {
		this->setCursor(Qt::ArrowCursor);   //指针设置为普通类型
		return;
	}
	//情况二：鼠标按下并移动时，只移动图像
	HTuple startRowBf, startColBf, endRowBf, endColBf, mouseRow, mouseCol, Button;
	try
	{
		set_check("~give_error");    //不要报错
		Herror ret0 = get_mposition(m_hHalconID, &mouseRow, &mouseCol, &Button);
		if (ret0 != H_MSG_TRUE)
		{
			return;
		}
		if (mouseCol.Num() <= 0 || mouseRow.Num() < 0)
		{
			return;
		}
		set_check("give_error");
	}
	catch (HException)
	{
		return;
	}
	if (m_bIsMove)
	{
		this->setCursor(Qt::PointingHandCursor);   //设置鼠标样式为手型指针
												   //计算移动值
		double RowMove = mouseRow[0].D() - m_tMouseDownRow[0].D();
		double ColMove = mouseCol[0].D() - m_tMouseDownCol[0].D();
		//得到当前的窗口坐标
		get_part(m_hHalconID, &startRowBf, &startColBf, &endRowBf, &endColBf);
		//移动图像
		if (m_hHalconID != NULL)
		{
			//如果有图像，则先清空图像
			detach_background_from_window(m_hHalconID);
		}
		set_part(m_hHalconID, startRowBf - RowMove, startColBf - ColMove, endRowBf - RowMove, endColBf - ColMove);
		set_check("~give_error");
		Herror ret1 = attach_background_to_window(hCurrentImage, m_hHalconID);
		//当光标不在Halcon窗口内时返回，否则会报错
		if (ret1 != H_MSG_TRUE)
			return;
		set_check("give_error");
	}
	//情况三：鼠标未按下移动时，实时获取当前图像坐标
	else {
		this->setCursor(Qt::ArrowCursor);   //指针设置为普通类型
		HTuple pointGray;
		try
		{
			set_check("~give_error");    //不要报错
			Herror ret2 = get_grayval(hCurrentImage, mouseRow, mouseCol, &pointGray);
			//当光标不在Halcon窗口内时返回，否则会报错
			if (ret2 != H_MSG_TRUE)
			{
				return;
			}
			if (mouseCol.Num() <= 0 || pointGray[0].D() < 0)
			{
				return;
			}
			set_check("give_error");    //不要报错
		}
		catch (HException)
		{
			// 设置文本的颜色为红色
			// this->setText(QString("X坐标：-    Y坐标：-    灰度值：-"));
			return;
		}
		//在绘图显示界面上显示坐标
		// this->setText(QString("X坐标：%1    Y坐标：%2    灰度值：%3").arg(mouseCol[0].D()).arg(mouseRow[0].D()).arg(pointGray[0].D()));
	}
}

//异步设置ID与显示，最好不要在qt的构造函数执行
void CHalconLabel::SetID()
{
	if (m_hHalconID == NULL) {
		set_window_attr("background_color", "black");     //设置背景色
		m_hLabelID = (Hlong)this->winId();
		open_window(0, 0, this->width(), this->height(), m_hLabelID, "visible", "", &m_hHalconID);
	}
}

void CHalconLabel::SetPixelTracke(bool ret)
{
	//设置鼠标追踪，可以实时响应鼠标移动事件
	this->setMouseTracking(ret);
}


//设置显示图像
void CHalconLabel::DisplayImage(Hobject  hDisplayImage)
{
	//隔离数据
	copy_image(hDisplayImage, &hCurrentImage);
#if 0 //显示一,充满整个画幅
	clear_window(m_hHalconID);
	detach_background_from_window(m_hHalconID);
	HTuple imgWidth, imgHeight;

	get_image_size(ho_Image, &imgWidth, &imgHeight);
	set_part(m_hHalconID, 0, 0, imgHeight - 1, imgWidth - 1);
	attach_background_to_window(ho_Image, m_hHalconID);
#endif

#if 1 //显示二，维持原有图像比例且居中显示	
	clear_window(m_hHalconID);
	detach_background_from_window(m_hHalconID);
	int labelWidth = this->width();  //窗口尺寸
	int labelHeight = this->height();
	HTuple imgWidth, imgHeight;                 //原图尺寸
	HTuple m_scaledWidth, m_scaledHeight;       //缩放后的尺寸
	HTuple m_hvScaledRate;                      //缩放比例
	get_image_size(hCurrentImage, &imgWidth, &imgHeight);
	//获取缩放系数
	tuple_min2(1.0 * labelWidth / imgWidth, 1.0 * labelHeight / imgHeight, &m_hvScaledRate);
	//进行图像缩放
	zoom_image_factor(hCurrentImage, &ho_ImageZoom, m_hvScaledRate, m_hvScaledRate, "constant");
	get_image_size(ho_ImageZoom, &m_scaledWidth, &m_scaledHeight);

	if (1.0*labelWidth / imgWidth < 1.0*labelHeight / imgHeight)
	{
		set_window_extents(m_hHalconID, labelHeight / 2.0 - m_scaledHeight / 2.0, 0, labelWidth, m_scaledHeight);
	}
	else
	{
		set_window_extents(m_hHalconID, 0, labelWidth / 2.0 - m_scaledWidth / 2.0, m_scaledWidth, labelHeight);
	}
	set_part(m_hHalconID, 0, 0, imgHeight - 1, imgWidth - 1);
	attach_background_to_window(hCurrentImage, m_hHalconID);

#endif

}

//恢复显示
void CHalconLabel::ResetDisplayImage()
{
#if 0 //显示一,充满整个画幅
	clear_window(m_hHalconID);
	detach_background_from_window(m_hHalconID);
	HTuple imgWidth, imgHeight;

	get_image_size(ho_Image, &imgWidth, &imgHeight);
	set_part(m_hHalconID, 0, 0, imgHeight - 1, imgWidth - 1);
	attach_background_to_window(ho_Image, m_hHalconID);
#endif

#if 1 //显示二，维持原有图像比例且居中显示	
	clear_window(m_hHalconID);
	detach_background_from_window(m_hHalconID);
	int labelWidth = this->width();  //窗口尺寸
	int labelHeight = this->height();
	HTuple imgWidth, imgHeight;                 //原图尺寸
	HTuple m_scaledWidth, m_scaledHeight;       //缩放后的尺寸
	HTuple m_hvScaledRate;                      //缩放比例
												//read_image(&ho_Image, "E:/1.jpg");
	get_image_size(hCurrentImage, &imgWidth, &imgHeight);
	//获取缩放系数
	tuple_min2(1.0 * labelWidth / imgWidth, 1.0 * labelHeight / imgHeight, &m_hvScaledRate);
	//进行图像缩放
	zoom_image_factor(hCurrentImage, &ho_ImageZoom, m_hvScaledRate, m_hvScaledRate, "constant");
	get_image_size(ho_ImageZoom, &m_scaledWidth, &m_scaledHeight);

	if (1.0*labelWidth / imgWidth < 1.0*labelHeight / imgHeight)
	{
		set_window_extents(m_hHalconID, labelHeight / 2.0 - m_scaledHeight / 2.0, 0, labelWidth, m_scaledHeight);
	}
	else
	{
		set_window_extents(m_hHalconID, 0, labelWidth / 2.0 - m_scaledWidth / 2.0, m_scaledWidth, labelHeight);
	}
	set_part(m_hHalconID, 0, 0, imgHeight - 1, imgWidth - 1);
	attach_background_to_window(hCurrentImage, m_hHalconID);
#endif
}

//绘制圆
void CHalconLabel::DrawCircle()
{
	//绘制的过程中，不能鼠标移动图像,不响应鼠标移动事件
	m_bIsDrawROI = true;
	Hobject currentRegion;
	HTuple Row, Column, Radius;
	draw_circle(m_hHalconID, &Row, &Column, &Radius);
	gen_circle(&currentRegion, Row, Column, Radius);
	union2(m_drawnRegion, currentRegion, &m_drawnRegion);
	//设置ROI边缘线
	set_color(m_hHalconID, "red");
	set_draw(m_hHalconID, "margin");
	set_line_width(m_hHalconID, 2);
	disp_obj(m_drawnRegion, m_hHalconID);
	//绘制结束以后，实时获取当前图像的像素坐标
	m_bIsDrawROI = false;
}

//绘制矩形
void CHalconLabel::DrawRectangle()
{
	m_bIsDrawROI = true;
	Hobject currentRegion;
	HTuple R1, C1, R2, C2;
	draw_rectangle1(m_hHalconID, &R1, &C1, &R2, &C2);
	gen_rectangle1(&currentRegion, R1, C1, R2, C2);
	union2(m_drawnRegion, currentRegion, &m_drawnRegion);
	//设置ROI边缘线
	set_color(m_hHalconID, "red");
	set_draw(m_hHalconID, "margin");
	set_line_width(m_hHalconID, 2);
	disp_obj(m_drawnRegion, m_hHalconID);
	m_bIsDrawROI = false;
}

//绘制旋转矩形
void CHalconLabel::DrawRotateRectangle()
{
	m_bIsDrawROI = true;
	Hobject currentRegion;
	HTuple Row, Column, Phi, Length1, Length2;
	draw_rectangle2(m_hHalconID, &Row, &Column, &Phi, &Length1, &Length2);
	gen_rectangle2(&currentRegion, Row, Column, Phi, Length1, Length2);
	union2(m_drawnRegion, currentRegion, &m_drawnRegion);
	//设置ROI边缘线
	set_color(m_hHalconID, "red");
	set_draw(m_hHalconID, "margin");
	set_line_width(m_hHalconID, 2);
	disp_obj(m_drawnRegion, m_hHalconID);
	m_bIsDrawROI = false;
}

//绘制椭圆
void CHalconLabel::DrawEllipse()
{
	m_bIsDrawROI = true;
	Hobject currentRegion;
	HTuple Row, Column, Phi, Radius1, Radius2;
	draw_ellipse(m_hHalconID, &Row, &Column, &Phi, &Radius1, &Radius2);
	gen_ellipse(&currentRegion, Row, Column, Phi, Radius1, Radius2);
	union2(m_drawnRegion, currentRegion, &m_drawnRegion);
	//设置ROI边缘线
	set_color(m_hHalconID, "red");
	set_draw(m_hHalconID, "margin");
	set_line_width(m_hHalconID, 2);
	disp_obj(m_drawnRegion, m_hHalconID);
	m_bIsDrawROI = false;
}

//清空所有的ROI区域
void CHalconLabel::ClearROI()
{
	//直接清空
	gen_empty_region(&m_drawnRegion);
}

//适应窗口，显示图像(同时显示图像和ROI区域)
void CHalconLabel::DispalyImageROI()
{
	DisplayImage(hCurrentImage);
	Hobject emptyRegion;
	Hlong isEqual;
	gen_empty_region(&emptyRegion);
	test_equal_region(emptyRegion, m_drawnRegion, &isEqual);
	if (isEqual == 0) {
		disp_obj(m_drawnRegion, m_hHalconID);
	}
}

//只显示ROI区域内的图像
void CHalconLabel::DisplayRegion()
{
	Hobject image;
	Hobject emptyRegion;
	Hlong isEqual;
	gen_empty_region(&emptyRegion);
	test_equal_region(emptyRegion, m_drawnRegion, &isEqual);
	if (isEqual != 0) {
		return;
	}
	reduce_domain(hCurrentImage, m_drawnRegion, &image);
	clear_window(m_hHalconID);
	DisplayImage(image);
}

//获取ROI区域
Hobject CHalconLabel::GetRegion()
{
	return m_drawnRegion;
}
