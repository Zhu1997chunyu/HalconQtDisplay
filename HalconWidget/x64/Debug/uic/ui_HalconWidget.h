/********************************************************************************
** Form generated from reading UI file 'HalconWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.9.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HALCONWIDGET_H
#define UI_HALCONWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_HalconWidgetClass
{
public:
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    QHBoxLayout *layout;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QDockWidget *dockWidget;
    QWidget *dockWidgetContents;
    QVBoxLayout *verticalLayout;
    QPushButton *readImageBtn;
    QPushButton *drawCircleBtn;
    QPushButton *drawRectangleBtn;
    QPushButton *drawRotateBtn;
    QPushButton *drawEllipseBtn;
    QPushButton *clearROIBtn;
    QPushButton *resetImageBtn;
    QSpacerItem *verticalSpacer;

    void setupUi(QMainWindow *HalconWidgetClass)
    {
        if (HalconWidgetClass->objectName().isEmpty())
            HalconWidgetClass->setObjectName(QStringLiteral("HalconWidgetClass"));
        HalconWidgetClass->resize(1069, 660);
        centralWidget = new QWidget(HalconWidgetClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        layout = new QHBoxLayout();
        layout->setSpacing(6);
        layout->setObjectName(QStringLiteral("layout"));

        horizontalLayout->addLayout(layout);

        HalconWidgetClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(HalconWidgetClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1069, 26));
        HalconWidgetClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(HalconWidgetClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        HalconWidgetClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(HalconWidgetClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        HalconWidgetClass->setStatusBar(statusBar);
        dockWidget = new QDockWidget(HalconWidgetClass);
        dockWidget->setObjectName(QStringLiteral("dockWidget"));
        dockWidget->setMinimumSize(QSize(180, 292));
        dockWidget->setMaximumSize(QSize(180, 524287));
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QStringLiteral("dockWidgetContents"));
        verticalLayout = new QVBoxLayout(dockWidgetContents);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        readImageBtn = new QPushButton(dockWidgetContents);
        readImageBtn->setObjectName(QStringLiteral("readImageBtn"));

        verticalLayout->addWidget(readImageBtn);

        drawCircleBtn = new QPushButton(dockWidgetContents);
        drawCircleBtn->setObjectName(QStringLiteral("drawCircleBtn"));

        verticalLayout->addWidget(drawCircleBtn);

        drawRectangleBtn = new QPushButton(dockWidgetContents);
        drawRectangleBtn->setObjectName(QStringLiteral("drawRectangleBtn"));

        verticalLayout->addWidget(drawRectangleBtn);

        drawRotateBtn = new QPushButton(dockWidgetContents);
        drawRotateBtn->setObjectName(QStringLiteral("drawRotateBtn"));

        verticalLayout->addWidget(drawRotateBtn);

        drawEllipseBtn = new QPushButton(dockWidgetContents);
        drawEllipseBtn->setObjectName(QStringLiteral("drawEllipseBtn"));

        verticalLayout->addWidget(drawEllipseBtn);

        clearROIBtn = new QPushButton(dockWidgetContents);
        clearROIBtn->setObjectName(QStringLiteral("clearROIBtn"));

        verticalLayout->addWidget(clearROIBtn);

        resetImageBtn = new QPushButton(dockWidgetContents);
        resetImageBtn->setObjectName(QStringLiteral("resetImageBtn"));

        verticalLayout->addWidget(resetImageBtn);

        verticalSpacer = new QSpacerItem(20, 370, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        dockWidget->setWidget(dockWidgetContents);
        HalconWidgetClass->addDockWidget(static_cast<Qt::DockWidgetArea>(2), dockWidget);

        retranslateUi(HalconWidgetClass);

        QMetaObject::connectSlotsByName(HalconWidgetClass);
    } // setupUi

    void retranslateUi(QMainWindow *HalconWidgetClass)
    {
        HalconWidgetClass->setWindowTitle(QApplication::translate("HalconWidgetClass", "HalconWidget", Q_NULLPTR));
        dockWidget->setWindowTitle(QApplication::translate("HalconWidgetClass", "\345\233\276\345\203\217\346\223\215\344\275\234", Q_NULLPTR));
        readImageBtn->setText(QApplication::translate("HalconWidgetClass", "\350\257\273\345\217\226\345\233\276\345\203\217", Q_NULLPTR));
        drawCircleBtn->setText(QApplication::translate("HalconWidgetClass", "\347\273\230\345\210\266\345\234\206\345\275\242", Q_NULLPTR));
        drawRectangleBtn->setText(QApplication::translate("HalconWidgetClass", "\347\273\230\345\210\266\347\237\251\345\275\242", Q_NULLPTR));
        drawRotateBtn->setText(QApplication::translate("HalconWidgetClass", "\347\273\230\345\210\266\346\227\213\350\275\254\347\237\251\345\275\242", Q_NULLPTR));
        drawEllipseBtn->setText(QApplication::translate("HalconWidgetClass", "\347\273\230\345\210\266\346\244\255\345\234\206", Q_NULLPTR));
        clearROIBtn->setText(QApplication::translate("HalconWidgetClass", "\346\270\205\351\231\244ROI", Q_NULLPTR));
        resetImageBtn->setText(QApplication::translate("HalconWidgetClass", "\346\201\242\345\244\215\345\216\237\345\233\276", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class HalconWidgetClass: public Ui_HalconWidgetClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HALCONWIDGET_H
