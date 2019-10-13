/********************************************************************************
** Form generated from reading UI file 'main.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAIN_H
#define UI_MAIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDial>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGroupBox *groupAccelerometer;
    QLabel *lblAcc;
    QLCDNumber *lcdAcceleration;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *layoutOpenGL;
    QLabel *lblGyro;
    QProgressBar *progressGyroX;
    QProgressBar *progressGyroY;
    QProgressBar *progressGyroZ;
    QLabel *lblMagnetometer;
    QProgressBar *progressMagnetY;
    QProgressBar *progressMagnetX;
    QProgressBar *progressMagnetZ;
    QGroupBox *groupBox_2;
    QProgressBar *progressIR6;
    QProgressBar *progressIR4;
    QProgressBar *progressIR2;
    QLabel *lblIR7;
    QLabel *lblIR0;
    QProgressBar *progressIR0;
    QLabel *lblIR2;
    QLabel *lblIR1;
    QProgressBar *progressIR5;
    QProgressBar *progressIR7;
    QLabel *lblIR3;
    QLabel *lblIR6;
    QProgressBar *progressIR1;
    QLabel *lblIR5;
    QLabel *lblIR4;
    QProgressBar *progressIR3;
    QLabel *label_11;
    QLabel *lblIRaddress;
    QLabel *label_7;
    QLabel *lblIRdata;
    QLabel *lblIRcheck;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *lblLight;
    QGroupBox *groupBox_4;
    QPushButton *btn2;
    QPushButton *btn5;
    QPushButton *btn1;
    QPushButton *btn4;
    QPushButton *btn3;
    QPushButton *btnAudioOff;
    QGroupBox *groupBox_5;
    QPushButton *btnUp;
    QLabel *lblVel;
    QPushButton *btnStop;
    QPushButton *btnRight;
    QPushButton *btnLeft;
    QPushButton *btnDown;
    QSlider *sliderVel;
    QLabel *lblVelValue;
    QGroupBox *groupBox_6;
    QCheckBox *checkLed5;
    QCheckBox *checkLed6;
    QCheckBox *checkLed2;
    QCheckBox *checkLed0;
    QCheckBox *checkLed8;
    QCheckBox *checkLed3;
    QCheckBox *checkLed1;
    QCheckBox *checkLed9;
    QCheckBox *checkLed7;
    QCheckBox *checkLed4;
    QSlider *slideRed;
    QSlider *slideBlue;
    QSlider *slideGreen;
    QLabel *lblRed;
    QLabel *lblGreen;
    QLabel *lblBlue;
    QGroupBox *groupBox_7;
    QLineEdit *txtPort;
    QPushButton *btnDisconnect;
    QPushButton *btnConnect;
    QCheckBox *chkSensors;
    QPushButton *btnTest;
    QGroupBox *groupBox_8;
    QPushButton *btnImage;
    QScrollArea *scrollCamera;
    QWidget *scrollAreaWidgetContents;
    QLineEdit *txtZoom;
    QLineEdit *txtHeight;
    QLineEdit *txtWidth;
    QPushButton *btnParameters;
    QLabel *label_8;
    QLabel *label_9;
    QRadioButton *radioColor;
    QRadioButton *radioGrayscale;
    QLabel *label_10;
    QCheckBox *chkRotate;
    QGroupBox *groupBox_9;
    QLabel *label_4;
    QLabel *lblSelector;
    QDial *dialSelector;
    QGroupBox *groupBox_3;
    QLabel *lblMic1;
    QProgressBar *progressMic0;
    QProgressBar *progressMic1;
    QProgressBar *progressMic2;
    QLabel *lblMic2;
    QLabel *lblMic0;
    QProgressBar *progressMic3;
    QLabel *lblMic3;
    QLabel *lblMic0Val;
    QLabel *lblMic1Val;
    QLabel *lblMic2Val;
    QLabel *lblMic3Val;
    QGroupBox *groupBox_10;
    QLabel *label_15;
    QLabel *lblDist;
    QProgressBar *progressDistance;
    QLabel *label_16;
    QGroupBox *groupBox_11;
    QLabel *lblBtnState;
    QPushButton *pushButton;
    QGroupBox *groupBox_12;
    QLabel *label_12;
    QLabel *lblBattAdc;
    QProgressBar *progressBatt;
    QLabel *label;
    QLabel *lblBattCharge;
    QGroupBox *groupBox_13;
    QLabel *lblSdStateTxt;
    QLabel *lblSdStateColor;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(980, 650);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setMinimumSize(QSize(980, 650));
        MainWindow->setMaximumSize(QSize(980, 650));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        groupAccelerometer = new QGroupBox(centralwidget);
        groupAccelerometer->setObjectName(QStringLiteral("groupAccelerometer"));
        groupAccelerometer->setGeometry(QRect(650, 10, 301, 231));
        groupAccelerometer->setStyleSheet(QLatin1String("QGroupBox {\n"
"	border: 1px solid gray;\n"
"	border-radius: 9px;\n"
"	margin-top: 0.5em;\n"
"}\n"
"\n"
"QGroupBox::title {\n"
"    subcontrol-origin: margin;\n"
"    left: 10px;\n"
"    padding: 0 3px 0 3px;\n"
"}\n"
""));
        lblAcc = new QLabel(groupAccelerometer);
        lblAcc->setObjectName(QStringLiteral("lblAcc"));
        lblAcc->setGeometry(QRect(20, 30, 61, 23));
        lcdAcceleration = new QLCDNumber(groupAccelerometer);
        lcdAcceleration->setObjectName(QStringLiteral("lcdAcceleration"));
        lcdAcceleration->setGeometry(QRect(80, 30, 91, 23));
        lcdAcceleration->setSegmentStyle(QLCDNumber::Flat);
        verticalLayoutWidget = new QWidget(groupAccelerometer);
        verticalLayoutWidget->setObjectName(QStringLiteral("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(20, 70, 151, 141));
        layoutOpenGL = new QVBoxLayout(verticalLayoutWidget);
        layoutOpenGL->setObjectName(QStringLiteral("layoutOpenGL"));
        layoutOpenGL->setContentsMargins(0, 0, 0, 0);
        lblGyro = new QLabel(groupAccelerometer);
        lblGyro->setObjectName(QStringLiteral("lblGyro"));
        lblGyro->setGeometry(QRect(190, 30, 81, 23));
        progressGyroX = new QProgressBar(groupAccelerometer);
        progressGyroX->setObjectName(QStringLiteral("progressGyroX"));
        progressGyroX->setGeometry(QRect(190, 53, 95, 20));
        progressGyroX->setStyleSheet(QLatin1String("QProgressBar {\n"
"    text-align: center;\n"
"}"));
        progressGyroX->setMinimum(-32768);
        progressGyroX->setMaximum(32768);
        progressGyroX->setValue(0);
        progressGyroX->setInvertedAppearance(false);
        progressGyroY = new QProgressBar(groupAccelerometer);
        progressGyroY->setObjectName(QStringLiteral("progressGyroY"));
        progressGyroY->setGeometry(QRect(190, 73, 95, 20));
        progressGyroY->setStyleSheet(QLatin1String("QProgressBar {\n"
"    text-align: center;\n"
"}"));
        progressGyroY->setMinimum(-32768);
        progressGyroY->setMaximum(32768);
        progressGyroY->setValue(0);
        progressGyroY->setInvertedAppearance(false);
        progressGyroZ = new QProgressBar(groupAccelerometer);
        progressGyroZ->setObjectName(QStringLiteral("progressGyroZ"));
        progressGyroZ->setGeometry(QRect(190, 93, 95, 20));
        progressGyroZ->setStyleSheet(QLatin1String("QProgressBar {\n"
"    text-align: center;\n"
"}"));
        progressGyroZ->setMinimum(-32768);
        progressGyroZ->setMaximum(32768);
        progressGyroZ->setValue(0);
        progressGyroZ->setInvertedAppearance(false);
        lblMagnetometer = new QLabel(groupAccelerometer);
        lblMagnetometer->setObjectName(QStringLiteral("lblMagnetometer"));
        lblMagnetometer->setGeometry(QRect(190, 117, 81, 23));
        progressMagnetY = new QProgressBar(groupAccelerometer);
        progressMagnetY->setObjectName(QStringLiteral("progressMagnetY"));
        progressMagnetY->setGeometry(QRect(190, 160, 95, 20));
        progressMagnetY->setMaximum(2000);
        progressMagnetY->setValue(24);
        progressMagnetY->setInvertedAppearance(false);
        progressMagnetX = new QProgressBar(groupAccelerometer);
        progressMagnetX->setObjectName(QStringLiteral("progressMagnetX"));
        progressMagnetX->setGeometry(QRect(190, 140, 95, 20));
        progressMagnetX->setMaximum(2000);
        progressMagnetX->setValue(24);
        progressMagnetX->setInvertedAppearance(false);
        progressMagnetZ = new QProgressBar(groupAccelerometer);
        progressMagnetZ->setObjectName(QStringLiteral("progressMagnetZ"));
        progressMagnetZ->setGeometry(QRect(190, 180, 95, 20));
        progressMagnetZ->setMaximum(2000);
        progressMagnetZ->setValue(24);
        progressMagnetZ->setInvertedAppearance(false);
        groupBox_2 = new QGroupBox(centralwidget);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(650, 250, 301, 241));
        groupBox_2->setStyleSheet(QLatin1String("QGroupBox {\n"
"	border: 1px solid gray;\n"
"	border-radius: 9px;\n"
"	margin-top: 0.5em;\n"
"}\n"
"\n"
"QGroupBox::title {\n"
"    subcontrol-origin: margin;\n"
"    left: 10px;\n"
"    padding: 0 3px 0 3px;\n"
"}\n"
""));
        groupBox_2->setFlat(false);
        progressIR6 = new QProgressBar(groupBox_2);
        progressIR6->setObjectName(QStringLiteral("progressIR6"));
        progressIR6->setGeometry(QRect(48, 184, 95, 20));
        progressIR6->setMaximum(2000);
        progressIR6->setValue(24);
        progressIR4 = new QProgressBar(groupBox_2);
        progressIR4->setObjectName(QStringLiteral("progressIR4"));
        progressIR4->setGeometry(QRect(48, 130, 95, 20));
        progressIR4->setMaximum(2000);
        progressIR4->setValue(24);
        progressIR2 = new QProgressBar(groupBox_2);
        progressIR2->setObjectName(QStringLiteral("progressIR2"));
        progressIR2->setGeometry(QRect(48, 76, 95, 20));
        progressIR2->setMaximum(2000);
        progressIR2->setValue(24);
        lblIR7 = new QLabel(groupBox_2);
        lblIR7->setObjectName(QStringLiteral("lblIR7"));
        lblIR7->setGeometry(QRect(20, 212, 20, 17));
        lblIR0 = new QLabel(groupBox_2);
        lblIR0->setObjectName(QStringLiteral("lblIR0"));
        lblIR0->setGeometry(QRect(20, 23, 20, 17));
        progressIR0 = new QProgressBar(groupBox_2);
        progressIR0->setObjectName(QStringLiteral("progressIR0"));
        progressIR0->setGeometry(QRect(48, 22, 95, 20));
        progressIR0->setMaximum(2000);
        progressIR0->setValue(24);
        progressIR0->setInvertedAppearance(false);
        lblIR2 = new QLabel(groupBox_2);
        lblIR2->setObjectName(QStringLiteral("lblIR2"));
        lblIR2->setGeometry(QRect(20, 77, 20, 17));
        lblIR1 = new QLabel(groupBox_2);
        lblIR1->setObjectName(QStringLiteral("lblIR1"));
        lblIR1->setGeometry(QRect(20, 50, 20, 17));
        progressIR5 = new QProgressBar(groupBox_2);
        progressIR5->setObjectName(QStringLiteral("progressIR5"));
        progressIR5->setGeometry(QRect(48, 157, 95, 20));
        progressIR5->setMaximum(2000);
        progressIR5->setValue(24);
        progressIR7 = new QProgressBar(groupBox_2);
        progressIR7->setObjectName(QStringLiteral("progressIR7"));
        progressIR7->setGeometry(QRect(48, 211, 95, 20));
        progressIR7->setMaximum(2000);
        progressIR7->setValue(24);
        lblIR3 = new QLabel(groupBox_2);
        lblIR3->setObjectName(QStringLiteral("lblIR3"));
        lblIR3->setGeometry(QRect(20, 104, 20, 17));
        lblIR6 = new QLabel(groupBox_2);
        lblIR6->setObjectName(QStringLiteral("lblIR6"));
        lblIR6->setGeometry(QRect(20, 185, 20, 17));
        progressIR1 = new QProgressBar(groupBox_2);
        progressIR1->setObjectName(QStringLiteral("progressIR1"));
        progressIR1->setGeometry(QRect(48, 49, 95, 20));
        progressIR1->setMaximum(2000);
        progressIR1->setValue(24);
        lblIR5 = new QLabel(groupBox_2);
        lblIR5->setObjectName(QStringLiteral("lblIR5"));
        lblIR5->setGeometry(QRect(20, 158, 20, 17));
        lblIR4 = new QLabel(groupBox_2);
        lblIR4->setObjectName(QStringLiteral("lblIR4"));
        lblIR4->setGeometry(QRect(20, 131, 20, 17));
        progressIR3 = new QProgressBar(groupBox_2);
        progressIR3->setObjectName(QStringLiteral("progressIR3"));
        progressIR3->setGeometry(QRect(48, 103, 95, 20));
        progressIR3->setMaximum(2000);
        progressIR3->setValue(24);
        label_11 = new QLabel(groupBox_2);
        label_11->setObjectName(QStringLiteral("label_11"));
        label_11->setGeometry(QRect(160, 14, 135, 31));
        label_11->setAlignment(Qt::AlignCenter);
        lblIRaddress = new QLabel(groupBox_2);
        lblIRaddress->setObjectName(QStringLiteral("lblIRaddress"));
        lblIRaddress->setGeometry(QRect(270, 137, 58, 17));
        label_7 = new QLabel(groupBox_2);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(193, 162, 69, 17));
        lblIRdata = new QLabel(groupBox_2);
        lblIRdata->setObjectName(QStringLiteral("lblIRdata"));
        lblIRdata->setGeometry(QRect(270, 162, 58, 17));
        lblIRcheck = new QLabel(groupBox_2);
        lblIRcheck->setObjectName(QStringLiteral("lblIRcheck"));
        lblIRcheck->setGeometry(QRect(270, 112, 58, 17));
        label_5 = new QLabel(groupBox_2);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(193, 112, 69, 17));
        label_6 = new QLabel(groupBox_2);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(193, 137, 69, 17));
        lblLight = new QLabel(groupBox_2);
        lblLight->setObjectName(QStringLiteral("lblLight"));
        lblLight->setGeometry(QRect(205, 42, 45, 45));
        groupBox_4 = new QGroupBox(centralwidget);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
        groupBox_4->setGeometry(QRect(340, 160, 181, 91));
        groupBox_4->setStyleSheet(QLatin1String("QGroupBox {\n"
"	border: 1px solid gray;\n"
"	border-radius: 9px;\n"
"	margin-top: 0.5em;\n"
"}\n"
"\n"
"QGroupBox::title {\n"
"    subcontrol-origin: margin;\n"
"    left: 10px;\n"
"    padding: 0 3px 0 3px;\n"
"}\n"
""));
        btn2 = new QPushButton(groupBox_4);
        btn2->setObjectName(QStringLiteral("btn2"));
        btn2->setGeometry(QRect(65, 20, 50, 32));
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/speaker.png"), QSize(), QIcon::Normal, QIcon::Off);
        btn2->setIcon(icon);
        btn5 = new QPushButton(groupBox_4);
        btn5->setObjectName(QStringLiteral("btn5"));
        btn5->setGeometry(QRect(65, 50, 50, 32));
        btn5->setIcon(icon);
        btn1 = new QPushButton(groupBox_4);
        btn1->setObjectName(QStringLiteral("btn1"));
        btn1->setGeometry(QRect(10, 20, 50, 32));
        btn1->setIcon(icon);
        btn4 = new QPushButton(groupBox_4);
        btn4->setObjectName(QStringLiteral("btn4"));
        btn4->setGeometry(QRect(10, 50, 50, 32));
        btn4->setIcon(icon);
        btn3 = new QPushButton(groupBox_4);
        btn3->setObjectName(QStringLiteral("btn3"));
        btn3->setGeometry(QRect(120, 20, 50, 32));
        btn3->setIcon(icon);
        btnAudioOff = new QPushButton(groupBox_4);
        btnAudioOff->setObjectName(QStringLiteral("btnAudioOff"));
        btnAudioOff->setGeometry(QRect(120, 50, 50, 32));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/images/mute.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnAudioOff->setIcon(icon1);
        groupBox_5 = new QGroupBox(centralwidget);
        groupBox_5->setObjectName(QStringLiteral("groupBox_5"));
        groupBox_5->setGeometry(QRect(340, 260, 181, 181));
        groupBox_5->setStyleSheet(QLatin1String("QGroupBox {\n"
"	border: 1px solid gray;\n"
"	border-radius: 9px;\n"
"	margin-top: 0.5em;\n"
"}\n"
"\n"
"QGroupBox::title {\n"
"    subcontrol-origin: margin;\n"
"    left: 10px;\n"
"    padding: 0 3px 0 3px;\n"
"}\n"
""));
        btnUp = new QPushButton(groupBox_5);
        btnUp->setObjectName(QStringLiteral("btnUp"));
        btnUp->setGeometry(QRect(65, 74, 50, 32));
        lblVel = new QLabel(groupBox_5);
        lblVel->setObjectName(QStringLiteral("lblVel"));
        lblVel->setGeometry(QRect(10, 20, 61, 17));
        lblVel->setAlignment(Qt::AlignCenter);
        btnStop = new QPushButton(groupBox_5);
        btnStop->setObjectName(QStringLiteral("btnStop"));
        btnStop->setGeometry(QRect(65, 106, 50, 32));
        btnRight = new QPushButton(groupBox_5);
        btnRight->setObjectName(QStringLiteral("btnRight"));
        btnRight->setGeometry(QRect(115, 106, 50, 32));
        btnLeft = new QPushButton(groupBox_5);
        btnLeft->setObjectName(QStringLiteral("btnLeft"));
        btnLeft->setGeometry(QRect(15, 106, 50, 32));
        btnDown = new QPushButton(groupBox_5);
        btnDown->setObjectName(QStringLiteral("btnDown"));
        btnDown->setGeometry(QRect(65, 138, 50, 32));
        sliderVel = new QSlider(groupBox_5);
        sliderVel->setObjectName(QStringLiteral("sliderVel"));
        sliderVel->setGeometry(QRect(15, 40, 150, 22));
        sliderVel->setMinimum(100);
        sliderVel->setMaximum(1100);
        sliderVel->setValue(500);
        sliderVel->setOrientation(Qt::Horizontal);
        sliderVel->setInvertedAppearance(false);
        sliderVel->setInvertedControls(false);
        sliderVel->setTickPosition(QSlider::NoTicks);
        sliderVel->setTickInterval(0);
        lblVelValue = new QLabel(groupBox_5);
        lblVelValue->setObjectName(QStringLiteral("lblVelValue"));
        lblVelValue->setGeometry(QRect(70, 20, 91, 17));
        lblVelValue->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        groupBox_6 = new QGroupBox(centralwidget);
        groupBox_6->setObjectName(QStringLiteral("groupBox_6"));
        groupBox_6->setGeometry(QRect(340, 10, 291, 141));
        groupBox_6->setStyleSheet(QLatin1String("QGroupBox {\n"
"	border: 1px solid gray;\n"
"	border-radius: 9px;\n"
"	margin-top: 0.5em;\n"
"}\n"
"\n"
"QGroupBox::title {\n"
"    subcontrol-origin: margin;\n"
"    left: 10px;\n"
"    padding: 0 3px 0 3px;\n"
"}\n"
""));
        checkLed5 = new QCheckBox(groupBox_6);
        checkLed5->setObjectName(QStringLiteral("checkLed5"));
        checkLed5->setGeometry(QRect(90, 22, 73, 21));
        checkLed6 = new QCheckBox(groupBox_6);
        checkLed6->setObjectName(QStringLiteral("checkLed6"));
        checkLed6->setGeometry(QRect(90, 44, 73, 21));
        checkLed2 = new QCheckBox(groupBox_6);
        checkLed2->setObjectName(QStringLiteral("checkLed2"));
        checkLed2->setGeometry(QRect(15, 66, 74, 21));
        checkLed0 = new QCheckBox(groupBox_6);
        checkLed0->setObjectName(QStringLiteral("checkLed0"));
        checkLed0->setGeometry(QRect(15, 22, 74, 21));
        checkLed8 = new QCheckBox(groupBox_6);
        checkLed8->setObjectName(QStringLiteral("checkLed8"));
        checkLed8->setGeometry(QRect(90, 88, 81, 21));
        checkLed3 = new QCheckBox(groupBox_6);
        checkLed3->setObjectName(QStringLiteral("checkLed3"));
        checkLed3->setGeometry(QRect(15, 88, 74, 21));
        checkLed1 = new QCheckBox(groupBox_6);
        checkLed1->setObjectName(QStringLiteral("checkLed1"));
        checkLed1->setGeometry(QRect(15, 44, 74, 21));
        checkLed9 = new QCheckBox(groupBox_6);
        checkLed9->setObjectName(QStringLiteral("checkLed9"));
        checkLed9->setGeometry(QRect(90, 110, 91, 21));
        checkLed7 = new QCheckBox(groupBox_6);
        checkLed7->setObjectName(QStringLiteral("checkLed7"));
        checkLed7->setGeometry(QRect(90, 66, 73, 21));
        checkLed4 = new QCheckBox(groupBox_6);
        checkLed4->setObjectName(QStringLiteral("checkLed4"));
        checkLed4->setGeometry(QRect(15, 110, 74, 21));
        slideRed = new QSlider(groupBox_6);
        slideRed->setObjectName(QStringLiteral("slideRed"));
        slideRed->setGeometry(QRect(190, 50, 20, 78));
        slideRed->setMaximum(100);
        slideRed->setOrientation(Qt::Vertical);
        slideBlue = new QSlider(groupBox_6);
        slideBlue->setObjectName(QStringLiteral("slideBlue"));
        slideBlue->setGeometry(QRect(250, 50, 20, 78));
        slideBlue->setMaximum(100);
        slideBlue->setOrientation(Qt::Vertical);
        slideGreen = new QSlider(groupBox_6);
        slideGreen->setObjectName(QStringLiteral("slideGreen"));
        slideGreen->setGeometry(QRect(220, 50, 20, 78));
        slideGreen->setMaximum(100);
        slideGreen->setOrientation(Qt::Vertical);
        lblRed = new QLabel(groupBox_6);
        lblRed->setObjectName(QStringLiteral("lblRed"));
        lblRed->setGeometry(QRect(190, 24, 20, 17));
        lblRed->setAlignment(Qt::AlignCenter);
        lblGreen = new QLabel(groupBox_6);
        lblGreen->setObjectName(QStringLiteral("lblGreen"));
        lblGreen->setGeometry(QRect(220, 24, 20, 17));
        lblGreen->setAlignment(Qt::AlignCenter);
        lblBlue = new QLabel(groupBox_6);
        lblBlue->setObjectName(QStringLiteral("lblBlue"));
        lblBlue->setGeometry(QRect(250, 24, 20, 17));
        lblBlue->setAlignment(Qt::AlignCenter);
        groupBox_7 = new QGroupBox(centralwidget);
        groupBox_7->setObjectName(QStringLiteral("groupBox_7"));
        groupBox_7->setGeometry(QRect(30, 10, 291, 141));
        groupBox_7->setStyleSheet(QLatin1String("QGroupBox {\n"
"	border: 1px solid gray;\n"
"	border-radius: 9px;\n"
"	margin-top: 0.5em;\n"
"}\n"
"\n"
"QGroupBox::title {\n"
"    subcontrol-origin: margin;\n"
"    left: 10px;\n"
"    padding: 0 3px 0 3px;\n"
"}\n"
""));
        txtPort = new QLineEdit(groupBox_7);
        txtPort->setObjectName(QStringLiteral("txtPort"));
        txtPort->setGeometry(QRect(20, 34, 251, 22));
        btnDisconnect = new QPushButton(groupBox_7);
        btnDisconnect->setObjectName(QStringLiteral("btnDisconnect"));
        btnDisconnect->setEnabled(false);
        btnDisconnect->setGeometry(QRect(150, 60, 130, 32));
        btnConnect = new QPushButton(groupBox_7);
        btnConnect->setObjectName(QStringLiteral("btnConnect"));
        btnConnect->setEnabled(true);
        btnConnect->setGeometry(QRect(20, 60, 131, 32));
        chkSensors = new QCheckBox(groupBox_7);
        chkSensors->setObjectName(QStringLiteral("chkSensors"));
        chkSensors->setGeometry(QRect(20, 110, 141, 21));
        chkSensors->setChecked(true);
        btnTest = new QPushButton(groupBox_7);
        btnTest->setObjectName(QStringLiteral("btnTest"));
        btnTest->setEnabled(true);
        btnTest->setGeometry(QRect(150, 110, 91, 21));
        groupBox_8 = new QGroupBox(centralwidget);
        groupBox_8->setObjectName(QStringLiteral("groupBox_8"));
        groupBox_8->setGeometry(QRect(30, 160, 291, 431));
        groupBox_8->setStyleSheet(QLatin1String("QGroupBox {\n"
"	border: 1px solid gray;\n"
"	border-radius: 9px;\n"
"	margin-top: 0.5em;\n"
"}\n"
"\n"
"QGroupBox::title {\n"
"    subcontrol-origin: margin;\n"
"    left: 10px;\n"
"    padding: 0 3px 0 3px;\n"
"}\n"
""));
        btnImage = new QPushButton(groupBox_8);
        btnImage->setObjectName(QStringLiteral("btnImage"));
        btnImage->setGeometry(QRect(45, 30, 201, 32));
        scrollCamera = new QScrollArea(groupBox_8);
        scrollCamera->setObjectName(QStringLiteral("scrollCamera"));
        scrollCamera->setGeometry(QRect(45, 70, 200, 200));
        scrollCamera->setMinimumSize(QSize(200, 200));
        scrollCamera->setMaximumSize(QSize(200, 200));
        scrollCamera->setAutoFillBackground(false);
        scrollCamera->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        scrollCamera->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        scrollCamera->setWidgetResizable(false);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 196, 196));
        scrollCamera->setWidget(scrollAreaWidgetContents);
        txtZoom = new QLineEdit(groupBox_8);
        txtZoom->setObjectName(QStringLiteral("txtZoom"));
        txtZoom->setGeometry(QRect(68, 359, 112, 22));
        txtHeight = new QLineEdit(groupBox_8);
        txtHeight->setObjectName(QStringLiteral("txtHeight"));
        txtHeight->setGeometry(QRect(68, 295, 112, 22));
        txtWidth = new QLineEdit(groupBox_8);
        txtWidth->setObjectName(QStringLiteral("txtWidth"));
        txtWidth->setGeometry(QRect(68, 327, 112, 22));
        btnParameters = new QPushButton(groupBox_8);
        btnParameters->setObjectName(QStringLiteral("btnParameters"));
        btnParameters->setGeometry(QRect(50, 391, 191, 32));
        label_8 = new QLabel(groupBox_8);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(18, 295, 42, 22));
        label_9 = new QLabel(groupBox_8);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setGeometry(QRect(18, 327, 42, 22));
        radioColor = new QRadioButton(groupBox_8);
        radioColor->setObjectName(QStringLiteral("radioColor"));
        radioColor->setGeometry(QRect(190, 296, 85, 21));
        radioColor->setChecked(true);
        radioGrayscale = new QRadioButton(groupBox_8);
        radioGrayscale->setObjectName(QStringLiteral("radioGrayscale"));
        radioGrayscale->setGeometry(QRect(190, 328, 85, 21));
        label_10 = new QLabel(groupBox_8);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setGeometry(QRect(18, 359, 42, 22));
        chkRotate = new QCheckBox(groupBox_8);
        chkRotate->setObjectName(QStringLiteral("chkRotate"));
        chkRotate->setGeometry(QRect(190, 360, 94, 21));
        groupBox_9 = new QGroupBox(centralwidget);
        groupBox_9->setObjectName(QStringLiteral("groupBox_9"));
        groupBox_9->setGeometry(QRect(540, 500, 90, 91));
        groupBox_9->setStyleSheet(QLatin1String("QGroupBox {\n"
"	border: 1px solid gray;\n"
"	border-radius: 9px;\n"
"	margin-top: 0.5em;\n"
"}\n"
"\n"
"QGroupBox::title {\n"
"    subcontrol-origin: margin;\n"
"    left: 10px;\n"
"    padding: 0 3px 0 3px;\n"
"}\n"
""));
        label_4 = new QLabel(groupBox_9);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(10, 20, 50, 17));
        lblSelector = new QLabel(groupBox_9);
        lblSelector->setObjectName(QStringLiteral("lblSelector"));
        lblSelector->setGeometry(QRect(59, 20, 31, 20));
        dialSelector = new QDial(groupBox_9);
        dialSelector->setObjectName(QStringLiteral("dialSelector"));
        dialSelector->setEnabled(false);
        dialSelector->setGeometry(QRect(25, 40, 40, 40));
        dialSelector->setMaximum(16);
        dialSelector->setPageStep(1);
        dialSelector->setWrapping(true);
        dialSelector->setNotchTarget(1);
        dialSelector->setNotchesVisible(true);
        groupBox_3 = new QGroupBox(centralwidget);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setGeometry(QRect(340, 450, 181, 141));
        groupBox_3->setStyleSheet(QLatin1String("QGroupBox {\n"
"	border: 1px solid gray;\n"
"	border-radius: 9px;\n"
"	margin-top: 0.5em;\n"
"}\n"
"\n"
"QGroupBox::title {\n"
"    subcontrol-origin: margin;\n"
"    left: 10px;\n"
"    padding: 0 3px 0 3px;\n"
"}\n"
""));
        lblMic1 = new QLabel(groupBox_3);
        lblMic1->setObjectName(QStringLiteral("lblMic1"));
        lblMic1->setGeometry(QRect(57, 120, 30, 17));
        lblMic1->setAlignment(Qt::AlignCenter);
        progressMic0 = new QProgressBar(groupBox_3);
        progressMic0->setObjectName(QStringLiteral("progressMic0"));
        progressMic0->setGeometry(QRect(20, 30, 20, 90));
        progressMic0->setMaximum(1500);
        progressMic0->setValue(24);
        progressMic0->setTextVisible(false);
        progressMic0->setOrientation(Qt::Vertical);
        progressMic0->setTextDirection(QProgressBar::BottomToTop);
        progressMic1 = new QProgressBar(groupBox_3);
        progressMic1->setObjectName(QStringLiteral("progressMic1"));
        progressMic1->setGeometry(QRect(60, 30, 20, 90));
        progressMic1->setMaximum(1500);
        progressMic1->setValue(24);
        progressMic1->setTextVisible(false);
        progressMic1->setOrientation(Qt::Vertical);
        progressMic1->setTextDirection(QProgressBar::BottomToTop);
        progressMic2 = new QProgressBar(groupBox_3);
        progressMic2->setObjectName(QStringLiteral("progressMic2"));
        progressMic2->setGeometry(QRect(100, 30, 20, 90));
        progressMic2->setMaximum(1500);
        progressMic2->setValue(24);
        progressMic2->setTextVisible(false);
        progressMic2->setOrientation(Qt::Vertical);
        progressMic2->setTextDirection(QProgressBar::BottomToTop);
        lblMic2 = new QLabel(groupBox_3);
        lblMic2->setObjectName(QStringLiteral("lblMic2"));
        lblMic2->setGeometry(QRect(97, 120, 30, 17));
        lblMic2->setAlignment(Qt::AlignCenter);
        lblMic0 = new QLabel(groupBox_3);
        lblMic0->setObjectName(QStringLiteral("lblMic0"));
        lblMic0->setGeometry(QRect(17, 120, 30, 17));
        lblMic0->setAlignment(Qt::AlignCenter);
        progressMic3 = new QProgressBar(groupBox_3);
        progressMic3->setObjectName(QStringLiteral("progressMic3"));
        progressMic3->setGeometry(QRect(140, 30, 20, 90));
        progressMic3->setMaximum(1500);
        progressMic3->setValue(24);
        progressMic3->setTextVisible(false);
        progressMic3->setOrientation(Qt::Vertical);
        progressMic3->setTextDirection(QProgressBar::BottomToTop);
        lblMic3 = new QLabel(groupBox_3);
        lblMic3->setObjectName(QStringLiteral("lblMic3"));
        lblMic3->setGeometry(QRect(137, 120, 30, 17));
        lblMic3->setAlignment(Qt::AlignCenter);
        lblMic0Val = new QLabel(groupBox_3);
        lblMic0Val->setObjectName(QStringLiteral("lblMic0Val"));
        lblMic0Val->setGeometry(QRect(15, 70, 30, 17));
        lblMic0Val->setAlignment(Qt::AlignCenter);
        lblMic1Val = new QLabel(groupBox_3);
        lblMic1Val->setObjectName(QStringLiteral("lblMic1Val"));
        lblMic1Val->setGeometry(QRect(55, 70, 30, 17));
        lblMic1Val->setAlignment(Qt::AlignCenter);
        lblMic2Val = new QLabel(groupBox_3);
        lblMic2Val->setObjectName(QStringLiteral("lblMic2Val"));
        lblMic2Val->setGeometry(QRect(95, 70, 30, 17));
        lblMic2Val->setAlignment(Qt::AlignCenter);
        lblMic3Val = new QLabel(groupBox_3);
        lblMic3Val->setObjectName(QStringLiteral("lblMic3Val"));
        lblMic3Val->setGeometry(QRect(135, 70, 30, 17));
        lblMic3Val->setAlignment(Qt::AlignCenter);
        groupBox_10 = new QGroupBox(centralwidget);
        groupBox_10->setObjectName(QStringLiteral("groupBox_10"));
        groupBox_10->setGeometry(QRect(650, 500, 301, 91));
        groupBox_10->setStyleSheet(QLatin1String("QGroupBox {\n"
"	border: 1px solid gray;\n"
"	border-radius: 9px;\n"
"	margin-top: 0.5em;\n"
"}\n"
"\n"
"QGroupBox::title {\n"
"    subcontrol-origin: margin;\n"
"    left: 10px;\n"
"    padding: 0 3px 0 3px;\n"
"}\n"
""));
        label_15 = new QLabel(groupBox_10);
        label_15->setObjectName(QStringLiteral("label_15"));
        label_15->setGeometry(QRect(20, 20, 81, 17));
        lblDist = new QLabel(groupBox_10);
        lblDist->setObjectName(QStringLiteral("lblDist"));
        lblDist->setGeometry(QRect(99, 20, 61, 20));
        progressDistance = new QProgressBar(groupBox_10);
        progressDistance->setObjectName(QStringLiteral("progressDistance"));
        progressDistance->setGeometry(QRect(20, 50, 241, 23));
        progressDistance->setStyleSheet(QLatin1String("QProgressBar {\n"
"    border: 2px solid grey;\n"
"    border-radius: 5px;\n"
"    text-align: center;\n"
"}"));
        progressDistance->setMaximum(200);
        progressDistance->setValue(0);
        label_16 = new QLabel(groupBox_10);
        label_16->setObjectName(QStringLiteral("label_16"));
        label_16->setGeometry(QRect(265, 50, 21, 17));
        groupBox_11 = new QGroupBox(centralwidget);
        groupBox_11->setObjectName(QStringLiteral("groupBox_11"));
        groupBox_11->setGeometry(QRect(540, 160, 90, 71));
        groupBox_11->setStyleSheet(QLatin1String("QGroupBox {\n"
"	border: 1px solid gray;\n"
"	border-radius: 9px;\n"
"	margin-top: 0.5em;\n"
"}\n"
"\n"
"QGroupBox::title {\n"
"    subcontrol-origin: margin;\n"
"    left: 10px;\n"
"    padding: 0 3px 0 3px;\n"
"}\n"
""));
        lblBtnState = new QLabel(groupBox_11);
        lblBtnState->setObjectName(QStringLiteral("lblBtnState"));
        lblBtnState->setGeometry(QRect(10, 45, 71, 20));
        lblBtnState->setAlignment(Qt::AlignCenter);
        pushButton = new QPushButton(groupBox_11);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setEnabled(true);
        pushButton->setGeometry(QRect(33, 20, 25, 25));
        pushButton->setStyleSheet(QStringLiteral(""));
        pushButton->setCheckable(true);
        pushButton->setChecked(false);
        groupBox_12 = new QGroupBox(centralwidget);
        groupBox_12->setObjectName(QStringLiteral("groupBox_12"));
        groupBox_12->setGeometry(QRect(540, 240, 90, 151));
        groupBox_12->setStyleSheet(QLatin1String("QGroupBox {\n"
"	border: 1px solid gray;\n"
"	border-radius: 9px;\n"
"	margin-top: 0.5em;\n"
"}\n"
"\n"
"QGroupBox::title {\n"
"    subcontrol-origin: margin;\n"
"    left: 10px;\n"
"    padding: 0 3px 0 3px;\n"
"}\n"
""));
        label_12 = new QLabel(groupBox_12);
        label_12->setObjectName(QStringLiteral("label_12"));
        label_12->setGeometry(QRect(10, 20, 71, 17));
        label_12->setAlignment(Qt::AlignCenter);
        lblBattAdc = new QLabel(groupBox_12);
        lblBattAdc->setObjectName(QStringLiteral("lblBattAdc"));
        lblBattAdc->setGeometry(QRect(10, 40, 71, 20));
        lblBattAdc->setAlignment(Qt::AlignCenter);
        progressBatt = new QProgressBar(groupBox_12);
        progressBatt->setObjectName(QStringLiteral("progressBatt"));
        progressBatt->setGeometry(QRect(25, 80, 41, 61));
        progressBatt->setStyleSheet(QLatin1String("QProgressBar {\n"
"    border: 2px solid grey;\n"
"    border-radius: 5px;\n"
"    text-align: center;\n"
"}\n"
"QProgressBar::chunk {\n"
"    background-color: rgb(170,170,170);\n"
"    height: 10px;\n"
"    margin: 0.5px;\n"
"}"));
        progressBatt->setValue(50);
        progressBatt->setTextVisible(false);
        progressBatt->setOrientation(Qt::Vertical);
        label = new QLabel(groupBox_12);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(35, 68, 21, 12));
        label->setStyleSheet(QStringLiteral("background-color: rgb(180, 180, 180);"));
        lblBattCharge = new QLabel(groupBox_12);
        lblBattCharge->setObjectName(QStringLiteral("lblBattCharge"));
        lblBattCharge->setEnabled(true);
        lblBattCharge->setGeometry(QRect(30, 95, 30, 30));
        lblBattCharge->setStyleSheet(QStringLiteral(""));
        lblBattCharge->setPixmap(QPixmap(QString::fromUtf8(":/images/charging.png")));
        lblBattCharge->setScaledContents(true);
        groupBox_13 = new QGroupBox(centralwidget);
        groupBox_13->setObjectName(QStringLiteral("groupBox_13"));
        groupBox_13->setGeometry(QRect(540, 400, 90, 91));
        groupBox_13->setStyleSheet(QLatin1String("QGroupBox {\n"
"	border: 1px solid gray;\n"
"	border-radius: 9px;\n"
"	margin-top: 0.5em;\n"
"}\n"
"\n"
"QGroupBox::title {\n"
"    subcontrol-origin: margin;\n"
"    left: 10px;\n"
"    padding: 0 3px 0 3px;\n"
"}\n"
""));
        lblSdStateTxt = new QLabel(groupBox_13);
        lblSdStateTxt->setObjectName(QStringLiteral("lblSdStateTxt"));
        lblSdStateTxt->setGeometry(QRect(10, 20, 71, 17));
        lblSdStateTxt->setAlignment(Qt::AlignCenter);
        lblSdStateColor = new QLabel(groupBox_13);
        lblSdStateColor->setObjectName(QStringLiteral("lblSdStateColor"));
        lblSdStateColor->setGeometry(QRect(30, 40, 31, 31));
        lblSdStateColor->setStyleSheet(QStringLiteral("background-color: rgb(0, 180, 0);"));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 980, 21));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "E-Puck Monitor", nullptr));
        groupAccelerometer->setTitle(QApplication::translate("MainWindow", "IMU", nullptr));
        lblAcc->setText(QApplication::translate("MainWindow", "Acceleration", nullptr));
        lblGyro->setText(QApplication::translate("MainWindow", "Gyro", nullptr));
        progressGyroX->setFormat(QApplication::translate("MainWindow", "%v", nullptr));
        progressGyroY->setFormat(QApplication::translate("MainWindow", "%v", nullptr));
        progressGyroZ->setFormat(QApplication::translate("MainWindow", "%v", nullptr));
        lblMagnetometer->setText(QApplication::translate("MainWindow", "Magnetometer", nullptr));
        groupBox_2->setTitle(QApplication::translate("MainWindow", "Infrareds", nullptr));
        lblIR7->setText(QApplication::translate("MainWindow", "IR7", nullptr));
        lblIR0->setText(QApplication::translate("MainWindow", "IR0", nullptr));
        lblIR2->setText(QApplication::translate("MainWindow", "IR2", nullptr));
        lblIR1->setText(QApplication::translate("MainWindow", "IR1", nullptr));
        lblIR3->setText(QApplication::translate("MainWindow", "IR3", nullptr));
        lblIR6->setText(QApplication::translate("MainWindow", "IR6", nullptr));
        lblIR5->setText(QApplication::translate("MainWindow", "IR5", nullptr));
        lblIR4->setText(QApplication::translate("MainWindow", "IR4", nullptr));
        label_11->setText(QApplication::translate("MainWindow", "Ambient light", nullptr));
        lblIRaddress->setText(QString());
        label_7->setText(QApplication::translate("MainWindow", "IR data:", nullptr));
        lblIRdata->setText(QString());
        lblIRcheck->setText(QString());
        label_5->setText(QApplication::translate("MainWindow", "IR check:", nullptr));
        label_6->setText(QApplication::translate("MainWindow", "IR address:", nullptr));
        lblLight->setText(QString());
        groupBox_4->setTitle(QApplication::translate("MainWindow", "Speaker", nullptr));
        btn2->setText(QApplication::translate("MainWindow", "2", nullptr));
        btn5->setText(QApplication::translate("MainWindow", "5", nullptr));
        btn1->setStyleSheet(QString());
        btn1->setText(QApplication::translate("MainWindow", "1", nullptr));
        btn4->setText(QApplication::translate("MainWindow", "4", nullptr));
        btn3->setText(QApplication::translate("MainWindow", "3", nullptr));
        btnAudioOff->setText(QString());
        groupBox_5->setTitle(QApplication::translate("MainWindow", "Motors", nullptr));
        btnUp->setText(QApplication::translate("MainWindow", "F", nullptr));
        lblVel->setText(QApplication::translate("MainWindow", "Velocity:", nullptr));
        btnStop->setText(QApplication::translate("MainWindow", "S", nullptr));
        btnRight->setText(QApplication::translate("MainWindow", "R", nullptr));
        btnLeft->setText(QApplication::translate("MainWindow", "L", nullptr));
        btnDown->setText(QApplication::translate("MainWindow", "B", nullptr));
        lblVelValue->setText(QApplication::translate("MainWindow", "500", nullptr));
        groupBox_6->setTitle(QApplication::translate("MainWindow", "LEDs", nullptr));
        checkLed5->setText(QApplication::translate("MainWindow", "LED_RGB5", nullptr));
        checkLed6->setText(QApplication::translate("MainWindow", "LED6", nullptr));
        checkLed2->setText(QApplication::translate("MainWindow", "LED2", nullptr));
        checkLed0->setText(QApplication::translate("MainWindow", "LED0", nullptr));
        checkLed8->setText(QApplication::translate("MainWindow", "BODY_LED8", nullptr));
        checkLed3->setText(QApplication::translate("MainWindow", "LED_RGB3", nullptr));
        checkLed1->setText(QApplication::translate("MainWindow", "LED_RGB1", nullptr));
        checkLed9->setText(QApplication::translate("MainWindow", "FRONT_LED9", nullptr));
        checkLed7->setText(QApplication::translate("MainWindow", "LED_RGB7", nullptr));
        checkLed4->setText(QApplication::translate("MainWindow", "LED4", nullptr));
        lblRed->setText(QApplication::translate("MainWindow", "R", nullptr));
        lblGreen->setText(QApplication::translate("MainWindow", "G", nullptr));
        lblBlue->setText(QApplication::translate("MainWindow", "B", nullptr));
        groupBox_7->setTitle(QApplication::translate("MainWindow", "User", nullptr));
        btnDisconnect->setText(QApplication::translate("MainWindow", "Disconnect", nullptr));
        btnConnect->setText(QApplication::translate("MainWindow", "Connect", nullptr));
        chkSensors->setText(QApplication::translate("MainWindow", "Activate sensors", nullptr));
        btnTest->setText(QApplication::translate("MainWindow", "Start test", nullptr));
        groupBox_8->setTitle(QApplication::translate("MainWindow", "Camera", nullptr));
        btnImage->setText(QApplication::translate("MainWindow", "Start receiving images", nullptr));
        txtZoom->setText(QApplication::translate("MainWindow", "8", nullptr));
        txtHeight->setText(QApplication::translate("MainWindow", "40", nullptr));
        txtWidth->setText(QApplication::translate("MainWindow", "40", nullptr));
        btnParameters->setText(QApplication::translate("MainWindow", "Send Paramters", nullptr));
        label_8->setText(QApplication::translate("MainWindow", "Height", nullptr));
        label_9->setText(QApplication::translate("MainWindow", "Width", nullptr));
        radioColor->setText(QApplication::translate("MainWindow", "Color", nullptr));
        radioGrayscale->setText(QApplication::translate("MainWindow", "Grayscale", nullptr));
        label_10->setText(QApplication::translate("MainWindow", "Zoom", nullptr));
        chkRotate->setText(QApplication::translate("MainWindow", "Rotate", nullptr));
        groupBox_9->setTitle(QApplication::translate("MainWindow", "Selector", nullptr));
        label_4->setText(QApplication::translate("MainWindow", "Position:", nullptr));
        lblSelector->setText(QApplication::translate("MainWindow", "0", nullptr));
        groupBox_3->setTitle(QApplication::translate("MainWindow", "Microphones", nullptr));
        lblMic1->setText(QApplication::translate("MainWindow", "Left", nullptr));
        progressMic0->setFormat(QApplication::translate("MainWindow", "%v", nullptr));
        progressMic1->setFormat(QApplication::translate("MainWindow", "%v", nullptr));
        progressMic2->setFormat(QApplication::translate("MainWindow", "%v", nullptr));
        lblMic2->setText(QApplication::translate("MainWindow", "Back", nullptr));
        lblMic0->setText(QApplication::translate("MainWindow", "Right", nullptr));
        progressMic3->setFormat(QApplication::translate("MainWindow", "%v", nullptr));
        lblMic3->setText(QApplication::translate("MainWindow", "Front", nullptr));
        lblMic0Val->setText(QApplication::translate("MainWindow", "0", nullptr));
        lblMic1Val->setText(QApplication::translate("MainWindow", "0", nullptr));
        lblMic2Val->setText(QApplication::translate("MainWindow", "0", nullptr));
        lblMic3Val->setText(QApplication::translate("MainWindow", "0", nullptr));
        groupBox_10->setTitle(QApplication::translate("MainWindow", "Time of Flight", nullptr));
        label_15->setText(QApplication::translate("MainWindow", "Distance (cm):", nullptr));
        lblDist->setText(QApplication::translate("MainWindow", "0", nullptr));
        progressDistance->setFormat(QApplication::translate("MainWindow", "%v", nullptr));
        label_16->setText(QApplication::translate("MainWindow", "2 m", nullptr));
        groupBox_11->setTitle(QApplication::translate("MainWindow", "Button", nullptr));
        lblBtnState->setText(QApplication::translate("MainWindow", "released", nullptr));
        pushButton->setText(QString());
        groupBox_12->setTitle(QApplication::translate("MainWindow", "Battery", nullptr));
        label_12->setText(QApplication::translate("MainWindow", "ADC value:", nullptr));
        lblBattAdc->setText(QApplication::translate("MainWindow", "0", nullptr));
        progressBatt->setFormat(QApplication::translate("MainWindow", "%p", nullptr));
        label->setText(QString());
        lblBattCharge->setText(QString());
        groupBox_13->setTitle(QApplication::translate("MainWindow", "Micro SD", nullptr));
        lblSdStateTxt->setText(QApplication::translate("MainWindow", "State", nullptr));
        lblSdStateColor->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAIN_H
