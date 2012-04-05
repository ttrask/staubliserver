; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=MousePage
LastTemplate=generic CWnd
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "P5UtilBeta.h"

ClassCount=8
Class1=CP5UtilBetaApp
Class2=CP5UtilBetaDlg
Class3=CAboutDlg

ResourceCount=7
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_DLG_BENDS
Resource4=IDD_DLG_MOUSE_EX
Class4=BendsPage
Class5=MousePage
Class6=MouseExPage
Resource5=IDD_DLG_XYZ
Class7=XyzPage
Resource6=IDD_DLG_MOUSE
Class8=ButtonsPage
Resource7=IDD_DLG_BUTTONS

[CLS:CP5UtilBetaApp]
Type=0
HeaderFile=P5UtilBeta.h
ImplementationFile=P5UtilBeta.cpp
Filter=N
LastObject=CP5UtilBetaApp

[CLS:CP5UtilBetaDlg]
Type=0
HeaderFile=P5UtilBetaDlg.h
ImplementationFile=P5UtilBetaDlg.cpp
Filter=D
LastObject=CP5UtilBetaDlg
BaseClass=CPropertySheet
VirtualFilter=hWC

[CLS:CAboutDlg]
Type=0
HeaderFile=P5UtilBetaDlg.h
ImplementationFile=P5UtilBetaDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_DLG_BENDS]
Type=1
Class=BendsPage
ControlCount=12
Control1=IDC_THUMB,msctls_progress32,1342177285
Control2=IDC_INDEX,msctls_progress32,1342177285
Control3=IDC_MIDDLE,msctls_progress32,1342177285
Control4=IDC_RING,msctls_progress32,1342177285
Control5=IDC_PINKY,msctls_progress32,1342177285
Control6=IDC_STATIC,button,1342177287
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342308352
Control10=IDC_TXT_BENDS_MSG,static,1342308352
Control11=IDC_TXT_STATUS,static,1342308352
Control12=IDC_STATIC,static,1342308352

[DLG:IDD_DLG_MOUSE]
Type=1
Class=MousePage
ControlCount=20
Control1=IDC_SLD_STICKY,msctls_trackbar32,1342242836
Control2=IDC_COMBO_LCLICK,combobox,1344339970
Control3=IDC_SLD_L_SENS,msctls_trackbar32,1342242836
Control4=IDC_COMBO_MCLICK,combobox,1344339970
Control5=IDC_SLD_M_SENS,msctls_trackbar32,1342242836
Control6=IDC_COMBO_RCLICK,combobox,1344339970
Control7=IDC_SLD_R_SENS,msctls_trackbar32,1342242836
Control8=IDC_BUT_SAVE_MAP,button,1342242816
Control9=IDC_STATIC,button,1342178055
Control10=IDC_STATIC,static,1342308608
Control11=IDC_STATIC,static,1342308352
Control12=IDC_TXT_LAREA,static,1342308608
Control13=IDC_TXT_RAREA,static,1342308608
Control14=IDC_STATIC,static,1342308352
Control15=IDC_TXT_MAREA,static,1342308608
Control16=IDC_STATIC,button,1342177287
Control17=IDC_STATIC,static,1342308608
Control18=IDC_TXT_STICKY_TIME,static,1342308608
Control19=IDC_CLICK,static,1342177550
Control20=IDC_STATIC,button,1342194439

[DLG:IDD_DLG_MOUSE_EX]
Type=1
Class=MouseExPage
ControlCount=6
Control1=IDC_STATIC,button,1342177287
Control2=IDC_RADIO_MOUSE_ON,button,1342177289
Control3=IDC_RADIO_MOUSE_OFF,button,1342177289
Control4=IDC_STATIC,static,1342308352
Control5=IDC_TXT_MOUSE_MODE,static,1342308352
Control6=IDC_STATIC,static,1342181383

[CLS:BendsPage]
Type=0
HeaderFile=BendsPage.h
ImplementationFile=BendsPage.cpp
BaseClass=CPropertyPage
Filter=D
VirtualFilter=idWC
LastObject=BendsPage

[CLS:MousePage]
Type=0
HeaderFile=MousePage.h
ImplementationFile=MousePage.cpp
BaseClass=CPropertyPage
Filter=D
LastObject=MousePage
VirtualFilter=idWC

[CLS:MouseExPage]
Type=0
HeaderFile=MouseExPage.h
ImplementationFile=MouseExPage.cpp
BaseClass=CPropertyPage
Filter=D
VirtualFilter=idWC
LastObject=IDC_RADIO_MOUSE_OFF

[DLG:IDD_DLG_XYZ]
Type=1
Class=XyzPage
ControlCount=8
Control1=IDC_SLD_X,msctls_trackbar32,1342242832
Control2=IDC_SLD_Z,msctls_trackbar32,1342242836
Control3=IDC_SLD_Y,msctls_trackbar32,1342242834
Control4=IDC_TXT_X,static,1342308352
Control5=IDC_TXT_Z,static,1342308352
Control6=IDC_TXT_Y,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_RESET_POS_DATA,button,1342242816

[CLS:XyzPage]
Type=0
HeaderFile=XyzPage.h
ImplementationFile=XyzPage.cpp
BaseClass=CPropertyPage
Filter=D
LastObject=IDC_RESET_POS_DATA
VirtualFilter=idWC

[DLG:IDD_DLG_BUTTONS]
Type=1
Class=ButtonsPage
ControlCount=7
Control1=IDC_BUT_A,button,1342242819
Control2=IDC_BUT_B,button,1342242819
Control3=IDC_BUT_C,button,1342242819
Control4=IDC_BUT_D,button,1342242819
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342181383
Control7=IDC_STATIC,static,1342308352

[CLS:ButtonsPage]
Type=0
HeaderFile=ButtonsPage.h
ImplementationFile=ButtonsPage.cpp
BaseClass=CPropertyPage
Filter=D
VirtualFilter=idWC
LastObject=IDC_BUT_A

