; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CAppForm
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "yprmodetest.h"
LastPage=0

ClassCount=5
Class1=CRoundSliderCtrl
Class2=CApp
Class3=CAppForm
Class4=CAppDoc
Class5=CAppFrameWnd

ResourceCount=4
Resource1=IDD_FORMVIEW
Resource2=IDR_MAIN_ACCEL
Resource3=IDD_SELECTDEVICE
Resource4=IDR_MAINFRAME

[CLS:CRoundSliderCtrl]
Type=0
BaseClass=CSliderCtrl
HeaderFile=RoundSliderCtrl.h
ImplementationFile=RoundSliderCtrl.cpp

[CLS:CApp]
Type=0
BaseClass=CWinApp
HeaderFile=YPRModeTest.h
ImplementationFile=YPRModeTest.cpp
LastObject=CApp

[CLS:CAppForm]
Type=0
BaseClass=CFormView 
HeaderFile=YPRModeTest.h
ImplementationFile=YPRModeTest.cpp
Filter=D
VirtualFilter=VWC
LastObject=CAppForm

[CLS:CAppDoc]
Type=0
BaseClass=CDocument
HeaderFile=YPRModeTest.h
ImplementationFile=YPRModeTest.cpp
LastObject=CAppDoc

[CLS:CAppFrameWnd]
Type=0
BaseClass=CFrameWnd
HeaderFile=YPRModeTest.h
ImplementationFile=YPRModeTest.cpp
LastObject=CAppFrameWnd
Filter=T
VirtualFilter=fWC

[DLG:IDD_FORMVIEW]
Type=1
Class=CAppForm
ControlCount=21
Control1=IDC_RENDERVIEW,static,1342177287
Control2=IDC_STATIC,static,1342308352
Control3=IDC_TXT_MSG,static,1342308354
Control4=IDC_CHANGEDEVICE,button,1342177280
Control5=IDC_VIEWFULLSCREEN,button,1342177280
Control6=IDC_SLD_YAW,msctls_trackbar32,1342177304
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342308352
Control10=IDC_SLD_PITCH,msctls_trackbar32,1342177304
Control11=IDC_SLD_ROLL,msctls_trackbar32,1342177304
Control12=IDC_STATIC,static,1342181383
Control13=IDC_FRM_MODE,button,1342177287
Control14=IDC_MODE_ABS,button,1342177289
Control15=IDC_MODE_RC,button,1342177289
Control16=IDC_STATIC,static,1342181383
Control17=IDC_STATIC,button,1342177287
Control18=IDC_CHECK_ENABLE_YAW,button,1342390275
Control19=IDC_CHECK_ENABLE_PITCH,button,1342390275
Control20=IDC_CHECK_ENABLE_ROLL,button,1342390275
Control21=IDC_STATIC,static,1342308352

[ACL:IDR_MAIN_ACCEL]
Type=1
Class=?
Command1=IDM_CHANGEDEVICE
Command2=IDC_VIEWFULLSCREEN
CommandCount=2

[DLG:IDD_SELECTDEVICE]
Type=1
Class=?
ControlCount=14
Control1=IDC_STATIC,button,1342177287
Control2=IDC_STATIC,static,1342308864
Control3=IDC_ADAPTER_COMBO,combobox,1344339971
Control4=IDC_STATIC,static,1342308864
Control5=IDC_DEVICE_COMBO,combobox,1344339971
Control6=IDC_STATIC,button,1342177287
Control7=IDC_WINDOW,button,1342373897
Control8=IDC_FULLSCREEN,button,1342177289
Control9=IDC_FULLSCREENMODES_COMBO,combobox,1344471043
Control10=IDC_STATIC,button,1342177287
Control11=IDC_STATIC,static,1342308864
Control12=IDC_MULTISAMPLE_COMBO,combobox,1344339971
Control13=IDOK,button,1342242817
Control14=IDCANCEL,button,1342242816

[MNU:IDR_MAINFRAME]
Type=1
Class=?
Command1=IDM_CHANGEDEVICE
Command2=ID_APP_EXIT
CommandCount=2

