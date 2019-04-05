/********************************************************************************
** Form generated from reading UI file 'calibration_dialog.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CALIBRATION_DIALOG_H
#define UI_CALIBRATION_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QPushButton *receive_image_btn;
    QPushButton *loadImage_Btn;
    QGroupBox *Color_Box;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout_3;
    QRadioButton *YUV_RadioBtn;
    QRadioButton *HSI_RadioBtn;
    QGroupBox *Object_Box;
    QWidget *layoutWidget1;
    QVBoxLayout *verticalLayout;
    QRadioButton *ball_seg_Btn;
    QRadioButton *obs_seg_Btn;
    QRadioButton *field_segment_Btn;
    QPushButton *segment_result_btn;
    QGroupBox *Image_ROI_Box;
    QWidget *layoutWidget2;
    QVBoxLayout *verticalLayout_2;
    QRadioButton *BigCircle_Btn;
    QRadioButton *SmallCircle_Btn;
    QPushButton *Save_ROI_Btn;
    QPushButton *Save_Result_Btn;
    QPushButton *Load_Result_Btn;
    QPushButton *white_wave_Btn;
    QPushButton *show_segmentBtn;
    QPushButton *Save_image_Btn;
    QPushButton *load_whites_Btn;
    QPushButton *save_whits_Btn;
    QPushButton *Error_table_Btn;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QStringLiteral("Dialog"));
        Dialog->resize(1157, 733);
        receive_image_btn = new QPushButton(Dialog);
        receive_image_btn->setObjectName(QStringLiteral("receive_image_btn"));
        receive_image_btn->setGeometry(QRect(30, 630, 121, 31));
        loadImage_Btn = new QPushButton(Dialog);
        loadImage_Btn->setObjectName(QStringLiteral("loadImage_Btn"));
        loadImage_Btn->setGeometry(QRect(30, 670, 121, 31));
        Color_Box = new QGroupBox(Dialog);
        Color_Box->setObjectName(QStringLiteral("Color_Box"));
        Color_Box->setEnabled(true);
        Color_Box->setGeometry(QRect(730, 610, 81, 101));
        Color_Box->setLayoutDirection(Qt::LeftToRight);
        Color_Box->setAutoFillBackground(false);
        Color_Box->setAlignment(Qt::AlignCenter);
        Color_Box->setFlat(true);
        Color_Box->setCheckable(false);
        layoutWidget = new QWidget(Color_Box);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(0, 30, 53, 50));
        verticalLayout_3 = new QVBoxLayout(layoutWidget);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        YUV_RadioBtn = new QRadioButton(layoutWidget);
        YUV_RadioBtn->setObjectName(QStringLiteral("YUV_RadioBtn"));

        verticalLayout_3->addWidget(YUV_RadioBtn);

        HSI_RadioBtn = new QRadioButton(layoutWidget);
        HSI_RadioBtn->setObjectName(QStringLiteral("HSI_RadioBtn"));

        verticalLayout_3->addWidget(HSI_RadioBtn);

        Object_Box = new QGroupBox(Dialog);
        Object_Box->setObjectName(QStringLiteral("Object_Box"));
        Object_Box->setGeometry(QRect(820, 610, 121, 101));
        layoutWidget1 = new QWidget(Object_Box);
        layoutWidget1->setObjectName(QStringLiteral("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(0, 20, 121, 81));
        verticalLayout = new QVBoxLayout(layoutWidget1);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        ball_seg_Btn = new QRadioButton(layoutWidget1);
        ball_seg_Btn->setObjectName(QStringLiteral("ball_seg_Btn"));

        verticalLayout->addWidget(ball_seg_Btn);

        obs_seg_Btn = new QRadioButton(layoutWidget1);
        obs_seg_Btn->setObjectName(QStringLiteral("obs_seg_Btn"));

        verticalLayout->addWidget(obs_seg_Btn);

        field_segment_Btn = new QRadioButton(layoutWidget1);
        field_segment_Btn->setObjectName(QStringLiteral("field_segment_Btn"));
        field_segment_Btn->setContextMenuPolicy(Qt::ActionsContextMenu);

        verticalLayout->addWidget(field_segment_Btn);

        segment_result_btn = new QPushButton(Dialog);
        segment_result_btn->setObjectName(QStringLiteral("segment_result_btn"));
        segment_result_btn->setGeometry(QRect(160, 630, 121, 31));
        Image_ROI_Box = new QGroupBox(Dialog);
        Image_ROI_Box->setObjectName(QStringLiteral("Image_ROI_Box"));
        Image_ROI_Box->setGeometry(QRect(950, 610, 111, 101));
        layoutWidget2 = new QWidget(Image_ROI_Box);
        layoutWidget2->setObjectName(QStringLiteral("layoutWidget2"));
        layoutWidget2->setGeometry(QRect(0, 20, 101, 79));
        verticalLayout_2 = new QVBoxLayout(layoutWidget2);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        BigCircle_Btn = new QRadioButton(layoutWidget2);
        BigCircle_Btn->setObjectName(QStringLiteral("BigCircle_Btn"));

        verticalLayout_2->addWidget(BigCircle_Btn);

        SmallCircle_Btn = new QRadioButton(layoutWidget2);
        SmallCircle_Btn->setObjectName(QStringLiteral("SmallCircle_Btn"));
        SmallCircle_Btn->setContextMenuPolicy(Qt::DefaultContextMenu);

        verticalLayout_2->addWidget(SmallCircle_Btn);

        Save_ROI_Btn = new QPushButton(layoutWidget2);
        Save_ROI_Btn->setObjectName(QStringLiteral("Save_ROI_Btn"));

        verticalLayout_2->addWidget(Save_ROI_Btn);

        Save_ROI_Btn->raise();
        BigCircle_Btn->raise();
        SmallCircle_Btn->raise();
        Save_Result_Btn = new QPushButton(Dialog);
        Save_Result_Btn->setObjectName(QStringLiteral("Save_Result_Btn"));
        Save_Result_Btn->setGeometry(QRect(400, 670, 101, 31));
        Load_Result_Btn = new QPushButton(Dialog);
        Load_Result_Btn->setObjectName(QStringLiteral("Load_Result_Btn"));
        Load_Result_Btn->setGeometry(QRect(290, 630, 101, 31));
        white_wave_Btn = new QPushButton(Dialog);
        white_wave_Btn->setObjectName(QStringLiteral("white_wave_Btn"));
        white_wave_Btn->setGeometry(QRect(400, 630, 101, 31));
        white_wave_Btn->setFocusPolicy(Qt::WheelFocus);
        show_segmentBtn = new QPushButton(Dialog);
        show_segmentBtn->setObjectName(QStringLiteral("show_segmentBtn"));
        show_segmentBtn->setGeometry(QRect(160, 670, 121, 31));
        Save_image_Btn = new QPushButton(Dialog);
        Save_image_Btn->setObjectName(QStringLiteral("Save_image_Btn"));
        Save_image_Btn->setGeometry(QRect(290, 672, 101, 31));
        load_whites_Btn = new QPushButton(Dialog);
        load_whites_Btn->setObjectName(QStringLiteral("load_whites_Btn"));
        load_whites_Btn->setGeometry(QRect(510, 630, 101, 31));
        load_whites_Btn->setFocusPolicy(Qt::WheelFocus);
        save_whits_Btn = new QPushButton(Dialog);
        save_whits_Btn->setObjectName(QStringLiteral("save_whits_Btn"));
        save_whits_Btn->setGeometry(QRect(510, 670, 101, 31));
        save_whits_Btn->setContextMenuPolicy(Qt::ActionsContextMenu);
        Error_table_Btn = new QPushButton(Dialog);
        Error_table_Btn->setObjectName(QStringLiteral("Error_table_Btn"));
        Error_table_Btn->setGeometry(QRect(620, 630, 101, 31));
        Error_table_Btn->setFocusPolicy(Qt::WheelFocus);
        Error_table_Btn->raise();
        receive_image_btn->raise();
        loadImage_Btn->raise();
        Color_Box->raise();
        Object_Box->raise();
        segment_result_btn->raise();
        Image_ROI_Box->raise();
        Save_Result_Btn->raise();
        Load_Result_Btn->raise();
        white_wave_Btn->raise();
        show_segmentBtn->raise();
        Save_image_Btn->raise();
        load_whites_Btn->raise();
        save_whits_Btn->raise();

        retranslateUi(Dialog);

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QApplication::translate("Dialog", "Dialog", 0));
        receive_image_btn->setText(QApplication::translate("Dialog", "update_image", 0));
        loadImage_Btn->setText(QApplication::translate("Dialog", "LoadImage", 0));
#ifndef QT_NO_TOOLTIP
        Color_Box->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        Color_Box->setTitle(QApplication::translate("Dialog", "color_selection", 0));
        YUV_RadioBtn->setText(QApplication::translate("Dialog", "YUV", 0));
        HSI_RadioBtn->setText(QApplication::translate("Dialog", "HSI", 0));
        Object_Box->setTitle(QApplication::translate("Dialog", "object_selection", 0));
        ball_seg_Btn->setText(QApplication::translate("Dialog", "ball_segment", 0));
        obs_seg_Btn->setText(QApplication::translate("Dialog", "obs_segment", 0));
        field_segment_Btn->setText(QApplication::translate("Dialog", "field_segment", 0));
        segment_result_btn->setText(QApplication::translate("Dialog", "segment_result", 0));
        Image_ROI_Box->setTitle(QApplication::translate("Dialog", "Image_ROI", 0));
        BigCircle_Btn->setText(QApplication::translate("Dialog", "BigCircle", 0));
        SmallCircle_Btn->setText(QApplication::translate("Dialog", "SmallCircle", 0));
        Save_ROI_Btn->setText(QApplication::translate("Dialog", "Save_ROI", 0));
        Save_Result_Btn->setText(QApplication::translate("Dialog", "Save_Result", 0));
        Load_Result_Btn->setText(QApplication::translate("Dialog", "Load_Result", 0));
        white_wave_Btn->setText(QApplication::translate("Dialog", "white_wave", 0));
        show_segmentBtn->setText(QApplication::translate("Dialog", "show_segment", 0));
        Save_image_Btn->setText(QApplication::translate("Dialog", "Save_Image", 0));
        load_whites_Btn->setText(QApplication::translate("Dialog", "load_whites", 0));
        save_whits_Btn->setText(QApplication::translate("Dialog", "save_whits", 0));
        Error_table_Btn->setText(QApplication::translate("Dialog", "ErrorTable", 0));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CALIBRATION_DIALOG_H
