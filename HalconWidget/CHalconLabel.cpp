/**
@file                 CHalconLabel.cpp
@brief                ��Halcon��QLabel����

����Halocnͼ�����ʾ��ROI����
@author               ZHU
@version              V1.0  2023.10.15
@updateTime           2023.10.15
@modified by          ZHU
@modified content     ��������϶��¼�
*/
#include "CHalconLabel.h"

CHalconLabel::CHalconLabel(QWidget *parent)
	: QLabel(parent)
	, m_bIsMove(false)
	, m_bIsDrawROI(false)
{
	//��ʼ��ͼ��
	gen_empty_obj(&hCurrentImage);
	//��ʼ��ROI����
	gen_empty_obj(&m_drawnRegion);
	//�����ı�λ��---�������Ϸ��������ı���ɫ
	this->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
	this->setStyleSheet("color: red;");
}

CHalconLabel::~CHalconLabel()
{

}

//�����ػ��¼�
void CHalconLabel::resizeEvent(QResizeEvent *ev)
{
	if (m_hHalconID != NULL)
	{
		//��ֹ������˸
		set_system("flush_graphic", "false");
		//������ʾ
#if 0 //��ʾһ,������������
		clear_window(m_hHalconID);
		detach_background_from_window(m_hHalconID);
		HTuple imgWidth, imgHeight;
		get_image_size(ho_Image, &imgWidth, &imgHeight);
		set_part(m_hHalconID, 0, 0, imgHeight - 1, imgWidth - 1);
		attach_background_to_window(ho_Image, m_hHalconID);
#endif
#if 1 //��ʾ����ά��ԭ��ͼ������Ҿ�����ʾ	
		clear_window(m_hHalconID);
		detach_background_from_window(m_hHalconID);
		int labelWidth = this->width();  //���ڳߴ�
		int labelHeight = this->height();
		HTuple imgWidth, imgHeight;                 //ԭͼ�ߴ�
		HTuple m_scaledWidth, m_scaledHeight;       //���ź�ĳߴ�
		HTuple m_hvScaledRate;                      //���ű���
										
		get_image_size(hCurrentImage, &imgWidth, &imgHeight);
		//��ȡ����ϵ��
		tuple_min2(1.0 * labelWidth / imgWidth, 1.0 * labelHeight / imgHeight, &m_hvScaledRate);
		//����ͼ������
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

//��ʾ����---�����������¼�
void CHalconLabel::wheelEvent(QWheelEvent* ev)
{
	double Zoom;   //�Ŵ����С����
	HTuple  mouseRow, mouseCol, Button;
	HTuple startRowBf, startColBf, endRowBf, endColBf, Ht, Wt, startRowAft, startColAft, endRowAft, endColAft;
	//����ǰ�����Ŵ�
	if (ev->delta()>0)
	{
		Zoom = 2.0;//�����Ŵ���
	}
	else//������С
	{
		Zoom = 1 / 2.0;
	}
	//��ȡ�����ԭͼ�ϵ�λ�ã�ע����ԭͼ���꣬����Label�µ�����
	HTuple  hv_Exception, hv_ErrMsg;
	try
	{
		get_mposition(m_hHalconID, &mouseRow, &mouseCol, &Button);
	}
	catch (HException& HDevExpDefaultException)
	{
		return;
	}
	//��ȡԭͼ��ʾ�Ĳ��֣�ע��Ҳ��ԭͼ����
	get_part(m_hHalconID, &startRowBf, &startColBf, &endRowBf, &endColBf);
	//����ǰ��ʾ��ͼ����
	Ht = endRowBf - startRowBf;
	Wt = endColBf - startColBf;
	//��ͨ��halcon�ܴ����ͼ�����ߴ���32K*32K�����������Сԭͼ�񣬵�����ʾ��ͼ�񳬳����ƣ������ɳ������
	if (Ht*Wt<20000 * 20000 || Zoom == 2.0)
	{
		//�������ź��ͼ������
		startRowAft = mouseRow - ((mouseRow - startRowBf) / Zoom);
		startColAft = mouseCol - ((mouseCol - startColBf) / Zoom);
		endRowAft = startRowAft + (Ht / Zoom);
		endColAft = startColAft + (Wt / Zoom);
		//����Ŵ�����򷵻�
		if (endRowAft - startRowAft < 2)
		{
			return;
		}

		if (m_hHalconID != NULL)
		{
			//�����ͼ���������ͼ��
			detach_background_from_window(m_hHalconID);
		}
		set_part(m_hHalconID, startRowAft, startColAft, endRowAft, endColAft);
		attach_background_to_window(hCurrentImage, m_hHalconID);
	}
}

//��ʾ����---��갴���¼�
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
	//��갴��ʱ����������
	m_tMouseDownRow = mouseRow;
	m_tMouseDownCol = mouseCol;
	m_bIsMove = true;
}

//��ʾ����---����ͷ��¼�
void CHalconLabel::mouseReleaseEvent(QMouseEvent* ev)
{
	m_bIsMove = false;
}

//��ʾ����---����ƶ��¼�(���������)
void CHalconLabel::mouseMoveEvent(QMouseEvent* ev)
{
	//���һ:������ROI����ʱ������Ӧ����ƶ��¼�
	if (m_bIsDrawROI) {
		this->setCursor(Qt::ArrowCursor);   //ָ������Ϊ��ͨ����
		return;
	}
	//���������갴�²��ƶ�ʱ��ֻ�ƶ�ͼ��
	HTuple startRowBf, startColBf, endRowBf, endColBf, mouseRow, mouseCol, Button;
	try
	{
		set_check("~give_error");    //��Ҫ����
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
		this->setCursor(Qt::PointingHandCursor);   //���������ʽΪ����ָ��
												   //�����ƶ�ֵ
		double RowMove = mouseRow[0].D() - m_tMouseDownRow[0].D();
		double ColMove = mouseCol[0].D() - m_tMouseDownCol[0].D();
		//�õ���ǰ�Ĵ�������
		get_part(m_hHalconID, &startRowBf, &startColBf, &endRowBf, &endColBf);
		//�ƶ�ͼ��
		if (m_hHalconID != NULL)
		{
			//�����ͼ���������ͼ��
			detach_background_from_window(m_hHalconID);
		}
		set_part(m_hHalconID, startRowBf - RowMove, startColBf - ColMove, endRowBf - RowMove, endColBf - ColMove);
		set_check("~give_error");
		Herror ret1 = attach_background_to_window(hCurrentImage, m_hHalconID);
		//����겻��Halcon������ʱ���أ�����ᱨ��
		if (ret1 != H_MSG_TRUE)
			return;
		set_check("give_error");
	}
	//����������δ�����ƶ�ʱ��ʵʱ��ȡ��ǰͼ������
	else {
		this->setCursor(Qt::ArrowCursor);   //ָ������Ϊ��ͨ����
		HTuple pointGray;
		try
		{
			set_check("~give_error");    //��Ҫ����
			Herror ret2 = get_grayval(hCurrentImage, mouseRow, mouseCol, &pointGray);
			//����겻��Halcon������ʱ���أ�����ᱨ��
			if (ret2 != H_MSG_TRUE)
			{
				return;
			}
			if (mouseCol.Num() <= 0 || pointGray[0].D() < 0)
			{
				return;
			}
			set_check("give_error");    //��Ҫ����
		}
		catch (HException)
		{
			// �����ı�����ɫΪ��ɫ
			// this->setText(QString("X���꣺-    Y���꣺-    �Ҷ�ֵ��-"));
			return;
		}
		//�ڻ�ͼ��ʾ��������ʾ����
		// this->setText(QString("X���꣺%1    Y���꣺%2    �Ҷ�ֵ��%3").arg(mouseCol[0].D()).arg(mouseRow[0].D()).arg(pointGray[0].D()));
	}
}

//�첽����ID����ʾ����ò�Ҫ��qt�Ĺ��캯��ִ��
void CHalconLabel::SetID()
{
	if (m_hHalconID == NULL) {
		set_window_attr("background_color", "black");     //���ñ���ɫ
		m_hLabelID = (Hlong)this->winId();
		open_window(0, 0, this->width(), this->height(), m_hLabelID, "visible", "", &m_hHalconID);
	}
}

void CHalconLabel::SetPixelTracke(bool ret)
{
	//�������׷�٣�����ʵʱ��Ӧ����ƶ��¼�
	this->setMouseTracking(ret);
}


//������ʾͼ��
void CHalconLabel::DisplayImage(Hobject  hDisplayImage)
{
	//��������
	copy_image(hDisplayImage, &hCurrentImage);
#if 0 //��ʾһ,������������
	clear_window(m_hHalconID);
	detach_background_from_window(m_hHalconID);
	HTuple imgWidth, imgHeight;

	get_image_size(ho_Image, &imgWidth, &imgHeight);
	set_part(m_hHalconID, 0, 0, imgHeight - 1, imgWidth - 1);
	attach_background_to_window(ho_Image, m_hHalconID);
#endif

#if 1 //��ʾ����ά��ԭ��ͼ������Ҿ�����ʾ	
	clear_window(m_hHalconID);
	detach_background_from_window(m_hHalconID);
	int labelWidth = this->width();  //���ڳߴ�
	int labelHeight = this->height();
	HTuple imgWidth, imgHeight;                 //ԭͼ�ߴ�
	HTuple m_scaledWidth, m_scaledHeight;       //���ź�ĳߴ�
	HTuple m_hvScaledRate;                      //���ű���
	get_image_size(hCurrentImage, &imgWidth, &imgHeight);
	//��ȡ����ϵ��
	tuple_min2(1.0 * labelWidth / imgWidth, 1.0 * labelHeight / imgHeight, &m_hvScaledRate);
	//����ͼ������
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

//�ָ���ʾ
void CHalconLabel::ResetDisplayImage()
{
#if 0 //��ʾһ,������������
	clear_window(m_hHalconID);
	detach_background_from_window(m_hHalconID);
	HTuple imgWidth, imgHeight;

	get_image_size(ho_Image, &imgWidth, &imgHeight);
	set_part(m_hHalconID, 0, 0, imgHeight - 1, imgWidth - 1);
	attach_background_to_window(ho_Image, m_hHalconID);
#endif

#if 1 //��ʾ����ά��ԭ��ͼ������Ҿ�����ʾ	
	clear_window(m_hHalconID);
	detach_background_from_window(m_hHalconID);
	int labelWidth = this->width();  //���ڳߴ�
	int labelHeight = this->height();
	HTuple imgWidth, imgHeight;                 //ԭͼ�ߴ�
	HTuple m_scaledWidth, m_scaledHeight;       //���ź�ĳߴ�
	HTuple m_hvScaledRate;                      //���ű���
												//read_image(&ho_Image, "E:/1.jpg");
	get_image_size(hCurrentImage, &imgWidth, &imgHeight);
	//��ȡ����ϵ��
	tuple_min2(1.0 * labelWidth / imgWidth, 1.0 * labelHeight / imgHeight, &m_hvScaledRate);
	//����ͼ������
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

//����Բ
void CHalconLabel::DrawCircle()
{
	//���ƵĹ����У���������ƶ�ͼ��,����Ӧ����ƶ��¼�
	m_bIsDrawROI = true;
	Hobject currentRegion;
	HTuple Row, Column, Radius;
	draw_circle(m_hHalconID, &Row, &Column, &Radius);
	gen_circle(&currentRegion, Row, Column, Radius);
	union2(m_drawnRegion, currentRegion, &m_drawnRegion);
	//����ROI��Ե��
	set_color(m_hHalconID, "red");
	set_draw(m_hHalconID, "margin");
	set_line_width(m_hHalconID, 2);
	disp_obj(m_drawnRegion, m_hHalconID);
	//���ƽ����Ժ�ʵʱ��ȡ��ǰͼ�����������
	m_bIsDrawROI = false;
}

//���ƾ���
void CHalconLabel::DrawRectangle()
{
	m_bIsDrawROI = true;
	Hobject currentRegion;
	HTuple R1, C1, R2, C2;
	draw_rectangle1(m_hHalconID, &R1, &C1, &R2, &C2);
	gen_rectangle1(&currentRegion, R1, C1, R2, C2);
	union2(m_drawnRegion, currentRegion, &m_drawnRegion);
	//����ROI��Ե��
	set_color(m_hHalconID, "red");
	set_draw(m_hHalconID, "margin");
	set_line_width(m_hHalconID, 2);
	disp_obj(m_drawnRegion, m_hHalconID);
	m_bIsDrawROI = false;
}

//������ת����
void CHalconLabel::DrawRotateRectangle()
{
	m_bIsDrawROI = true;
	Hobject currentRegion;
	HTuple Row, Column, Phi, Length1, Length2;
	draw_rectangle2(m_hHalconID, &Row, &Column, &Phi, &Length1, &Length2);
	gen_rectangle2(&currentRegion, Row, Column, Phi, Length1, Length2);
	union2(m_drawnRegion, currentRegion, &m_drawnRegion);
	//����ROI��Ե��
	set_color(m_hHalconID, "red");
	set_draw(m_hHalconID, "margin");
	set_line_width(m_hHalconID, 2);
	disp_obj(m_drawnRegion, m_hHalconID);
	m_bIsDrawROI = false;
}

//������Բ
void CHalconLabel::DrawEllipse()
{
	m_bIsDrawROI = true;
	Hobject currentRegion;
	HTuple Row, Column, Phi, Radius1, Radius2;
	draw_ellipse(m_hHalconID, &Row, &Column, &Phi, &Radius1, &Radius2);
	gen_ellipse(&currentRegion, Row, Column, Phi, Radius1, Radius2);
	union2(m_drawnRegion, currentRegion, &m_drawnRegion);
	//����ROI��Ե��
	set_color(m_hHalconID, "red");
	set_draw(m_hHalconID, "margin");
	set_line_width(m_hHalconID, 2);
	disp_obj(m_drawnRegion, m_hHalconID);
	m_bIsDrawROI = false;
}

//������е�ROI����
void CHalconLabel::ClearROI()
{
	//ֱ�����
	gen_empty_region(&m_drawnRegion);
}

//��Ӧ���ڣ���ʾͼ��(ͬʱ��ʾͼ���ROI����)
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

//ֻ��ʾROI�����ڵ�ͼ��
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

//��ȡROI����
Hobject CHalconLabel::GetRegion()
{
	return m_drawnRegion;
}
