; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CInputVolAdjDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "AudioMixer.h"

ClassCount=23
Class1=CAudioMixerApp
Class2=CAudioMixerDlg
Class3=CAboutDlg

ResourceCount=13
Resource1=IDD_MIXCONF
Resource2=IDR_MAINFRAME
Resource3=IDD_DIALOG_SCHEME
Resource4=IDD_PROPPAGEL_MIXCTL
Resource5=IDD_DIALOG_WAIT
Resource6=IDD_PROPPAGEL_INPUT_VOLCTL
Resource7=IDD_AUDIOMIXER_DIALOG
Class4=COutputVolAdjDlg
Class5=CVolTabCtl
Class6=CInputVolAdjDlg
Class7=CVolAdjDlgBox
Class8=CVolTab
Class9=CMixConf
Class10=CMixCtlCh1
Class11=CMixCtlCh2
Class12=CMixCtlCh3
Class13=CMixCtlCh4
Class14=CMixCtlCh5
Class15=CMixCtlCh6
Class16=CMixCtlCh7
Class17=CMixCtlCh8
Class18=CMixChConf
Resource8=IDD_DIALOG_SYSMGR
Resource9=IDD_VOL
Resource10=IDD_ABOUTBOX
Class19=CConectMgr
Class20=CSchemeMgr
Class21=CConfigMgr
Resource11=IDD_PROPPAGEL_OUTPUT_VOLCTL
Class22=CWait
Resource12=IDD_PROPPAGEL_CONFIG_MGR
Class23=CVolCtl
Resource13=IDD_PROPPAGEL_VOLCTL

[CLS:CAudioMixerApp]
Type=0
HeaderFile=AudioMixer.h
ImplementationFile=AudioMixer.cpp
Filter=N

[CLS:CAudioMixerDlg]
Type=0
HeaderFile=AudioMixerDlg.h
ImplementationFile=AudioMixerDlg.cpp
Filter=D
LastObject=CAudioMixerDlg
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=AudioMixerDlg.h
ImplementationFile=AudioMixerDlg.cpp
Filter=D
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_AUDIOMIXER_DIALOG]
Type=1
Class=?
ControlCount=6
Control1=IDCANCEL,button,1342242816
Control2=IDC_INPUT_GAIN,button,1342242816
Control3=IDC_OUTPUT,button,1342242816
Control4=IDC_BUTTON3,button,1342242816
Control5=IDC_BUTTON4,button,1342242816
Control6=IDC_BUTTON_APPLY,button,1342242816

[DLG:IDD_PROPPAGEL_INPUT_VOLCTL]
Type=1
Class=CInputVolAdjDlg
ControlCount=22
Control1=IDC_SLIDER1,msctls_trackbar32,1342242842
Control2=IDC_STATIC,static,1342308352
Control3=IDC_SLIDER3,msctls_trackbar32,1342242842
Control4=IDC_STATIC,static,1342308352
Control5=IDC_SLIDER2,msctls_trackbar32,1342242842
Control6=IDC_STATIC,static,1342308352
Control7=IDC_SLIDER4,msctls_trackbar32,1342242842
Control8=IDC_STATIC,static,1342308352
Control9=IDC_SLIDER5,msctls_trackbar32,1342242842
Control10=IDC_STATIC,static,1342308352
Control11=IDC_SLIDER6,msctls_trackbar32,1342242842
Control12=IDC_STATIC,static,1342308352
Control13=IDC_SLIDER7,msctls_trackbar32,1342242842
Control14=IDC_STATIC,static,1342308352
Control15=IDC_SLIDER8,msctls_trackbar32,1342242842
Control16=IDC_STATIC,static,1342308352
Control17=IDC_STATIC,button,1342177287
Control18=IDC_STATIC,button,1342177287
Control19=IDC_STATIC,button,1073741831
Control20=IDC_RADIO_FOURCH,button,1342308361
Control21=IDC_RADIO_8CH,button,1342177289
Control22=IDC_BUTTON_APPLY_IVOLCHANGE,button,1342242816

[DLG:IDD_PROPPAGEL_OUTPUT_VOLCTL]
Type=1
Class=COutputVolAdjDlg
ControlCount=22
Control1=IDC_SLIDER1,msctls_trackbar32,1342242842
Control2=IDC_STATIC,static,1342308352
Control3=IDC_SLIDER3,msctls_trackbar32,1342242842
Control4=IDC_STATIC,static,1342308352
Control5=IDC_SLIDER2,msctls_trackbar32,1342242842
Control6=IDC_STATIC,static,1342308352
Control7=IDC_SLIDER4,msctls_trackbar32,1342242842
Control8=IDC_STATIC,static,1342308352
Control9=IDC_SLIDER5,msctls_trackbar32,1342242842
Control10=IDC_STATIC,static,1342308352
Control11=IDC_SLIDER6,msctls_trackbar32,1342242842
Control12=IDC_STATIC,static,1342308352
Control13=IDC_SLIDER7,msctls_trackbar32,1342242842
Control14=IDC_STATIC,static,1342308352
Control15=IDC_SLIDER8,msctls_trackbar32,1342242842
Control16=IDC_STATIC,static,1342308352
Control17=IDC_STATIC,button,1342177287
Control18=IDC_STATIC,button,1342177287
Control19=IDC_STATIC,button,1073741831
Control20=IDC_RADIO_FOURCH,button,1342308361
Control21=IDC_RADIO_8CH,button,1342177289
Control22=IDC_BUTTON_APPLY_OUTVOL,button,1342242816

[DLG:IDD_VOL]
Type=1
Class=CVolAdjDlgBox
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_VOL_TAB,SysTabControl32,1342177280

[CLS:CVolTabCtl]
Type=0
HeaderFile=VolTabCtl.h
ImplementationFile=VolTabCtl.cpp
BaseClass=CTabCtrl
Filter=W
LastObject=CVolTabCtl

[CLS:CInputVolAdjDlg]
Type=0
HeaderFile=InputVolAdjDlg.h
ImplementationFile=InputVolAdjDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_SLIDER3
VirtualFilter=dWC

[CLS:COutputVolAdjDlg]
Type=0
HeaderFile=OutputVolAdjDlg.h
ImplementationFile=OutputVolAdjDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=COutputVolAdjDlg
VirtualFilter=dWC

[CLS:CVolAdjDlgBox]
Type=0
HeaderFile=VolAdjDlgBox.h
ImplementationFile=VolAdjDlgBox.cpp
BaseClass=CDialog
Filter=D
LastObject=CVolAdjDlgBox
VirtualFilter=dWC

[CLS:CVolTab]
Type=0
HeaderFile=VolTab.h
ImplementationFile=VolTab.cpp
BaseClass=CTabCtrl
Filter=W
VirtualFilter=UWC
LastObject=CVolTab

[DLG:IDD_MIXCONF]
Type=1
Class=CMixConf
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_TAB_MIX,SysTabControl32,1342177280

[CLS:CMixConf]
Type=0
HeaderFile=MixConf.h
ImplementationFile=MixConf.cpp
BaseClass=CDialog
Filter=D
LastObject=CMixConf
VirtualFilter=dWC

[CLS:CMixCtlCh1]
Type=0
HeaderFile=MixCtlCh1.h
ImplementationFile=MixCtlCh1.cpp
BaseClass=CDialog
Filter=D
LastObject=CMixCtlCh1
VirtualFilter=dWC

[CLS:CMixCtlCh2]
Type=0
HeaderFile=MixCtlCh2.h
ImplementationFile=MixCtlCh2.cpp
BaseClass=CDialog
Filter=D
LastObject=CMixCtlCh2

[CLS:CMixCtlCh3]
Type=0
HeaderFile=MixCtlCh3.h
ImplementationFile=MixCtlCh3.cpp
BaseClass=CDialog
Filter=D
LastObject=CMixCtlCh3

[CLS:CMixCtlCh4]
Type=0
HeaderFile=MixCtlCh4.h
ImplementationFile=MixCtlCh4.cpp
BaseClass=CDialog
Filter=D
LastObject=CMixCtlCh4

[CLS:CMixCtlCh5]
Type=0
HeaderFile=MixCtlCh5.h
ImplementationFile=MixCtlCh5.cpp
BaseClass=CDialog
Filter=D
LastObject=CMixCtlCh5

[CLS:CMixCtlCh6]
Type=0
HeaderFile=MixCtlCh6.h
ImplementationFile=MixCtlCh6.cpp
BaseClass=CDialog
Filter=D
LastObject=CMixCtlCh6

[CLS:CMixCtlCh7]
Type=0
HeaderFile=MixCtlCh7.h
ImplementationFile=MixCtlCh7.cpp
BaseClass=CDialog
Filter=D
LastObject=CMixCtlCh7

[CLS:CMixCtlCh8]
Type=0
HeaderFile=MixCtlCh8.h
ImplementationFile=MixCtlCh8.cpp
BaseClass=CDialog
Filter=D
LastObject=CMixCtlCh8

[DLG:IDD_PROPPAGEL_MIXCTL]
Type=1
Class=CMixChConf
ControlCount=31
Control1=IDC_SLIDER_INPUT1,msctls_trackbar32,1342242842
Control2=IDC_STATIC,static,1342308352
Control3=IDC_SLIDER_INPUT3,msctls_trackbar32,1342242842
Control4=IDC_STATIC,static,1342308352
Control5=IDC_SLIDER_INPUT2,msctls_trackbar32,1342242842
Control6=IDC_STATIC,static,1342308352
Control7=IDC_SLIDER_INPUT4,msctls_trackbar32,1342242842
Control8=IDC_STATIC,static,1342308352
Control9=IDC_SLIDER_INPUT5,msctls_trackbar32,1342242842
Control10=IDC_STATIC,static,1342308352
Control11=IDC_SLIDER_INPUT6,msctls_trackbar32,1342242842
Control12=IDC_STATIC,static,1342308352
Control13=IDC_SLIDER_INPUT7,msctls_trackbar32,1342242842
Control14=IDC_STATIC,static,1342308352
Control15=IDC_SLIDER_INPUT8,msctls_trackbar32,1342242842
Control16=IDC_STATIC,static,1342308352
Control17=IDC_STATIC,button,1342177287
Control18=IDC_STATIC,button,1342177287
Control19=IDC_CHECK_MIX1,button,1342242819
Control20=IDC_CHECK_MIX2,button,1342242819
Control21=IDC_CHECK_MIX3,button,1342242819
Control22=IDC_CHECK_MIX4,button,1342242819
Control23=IDC_CHECK_MIX5,button,1342242819
Control24=IDC_CHECK_MIX6,button,1342242819
Control25=IDC_CHECK_MIX7,button,1342242819
Control26=IDC_CHECK_MIX8,button,1342242819
Control27=IDC_CHNUM,static,1342177280
Control28=IDC_STATIC,button,1073741831
Control29=IDC_RADIO_4,button,1342308361
Control30=IDC_RADIO_8,button,1342177289
Control31=IDC_BUTTON_APPLY,button,1342242816

[CLS:CMixChConf]
Type=0
HeaderFile=MixChConf.h
ImplementationFile=MixChConf.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_SLIDER_INPUT1

[DLG:IDD_DIALOG_SYSMGR]
Type=1
Class=CConectMgr
ControlCount=14
Control1=IDC_STATIC,button,1342177287
Control2=IDC_RADIO_SERIAL,button,1342308361
Control3=IDC_RADIO_USB,button,1342177289
Control4=IDC_STATIC,button,1342177287
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_EDIT_FIRMWARE_NAME,edit,1350631552
Control9=IDC_BUTTON_BROWSER,button,1342242816
Control10=IDC_BUTTON_UPGRADE,button,1342242816
Control11=IDC_COMBO_PORT,combobox,1344339970
Control12=IDC_COMBO_BAUDRATE,combobox,1344339971
Control13=IDC_STATIC,static,1342308352
Control14=IDC_COMBO_DATAWIDTH,combobox,1344339971

[DLG:IDD_DIALOG_SCHEME]
Type=1
Class=CSchemeMgr
ControlCount=13
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,button,1342177287
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_BUTTON_SAVE,button,1342242816
Control7=IDC_BUTTON_APPLY,button,1342242816
Control8=IDC_BUTTON_EMPTY,button,1342242816
Control9=IDC_BUTTON_EXPORT,button,1342242816
Control10=IDC_BUTTON_IMPORT,button,1342242816
Control11=IDC_LIST_SCHEME,SysListView32,1350664477
Control12=IDC_EDIT_SCHEME_NAME,edit,1350631552
Control13=IDC_EDIT_SCHEME_NUM,edit,1350631552

[CLS:CConectMgr]
Type=0
HeaderFile=ConectMgr.h
ImplementationFile=ConectMgr.cpp
BaseClass=CPropertyPage
Filter=D
VirtualFilter=idWC
LastObject=IDC_RADIO_USB

[CLS:CSchemeMgr]
Type=0
HeaderFile=SchemeMgr.h
ImplementationFile=SchemeMgr.cpp
BaseClass=CPropertyPage
Filter=D
VirtualFilter=idWC
LastObject=IDC_EDIT_SCHEME_NUM

[DLG:IDD_PROPPAGEL_CONFIG_MGR]
Type=1
Class=CConfigMgr
ControlCount=0

[CLS:CConfigMgr]
Type=0
HeaderFile=ConfigMgr.h
ImplementationFile=ConfigMgr.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CConfigMgr

[DLG:IDD_DIALOG_WAIT]
Type=1
Class=CWait
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STR_STATUS,static,1342308352

[CLS:CWait]
Type=0
HeaderFile=Wait.h
ImplementationFile=Wait.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CWait

[DLG:IDD_PROPPAGEL_VOLCTL]
Type=1
Class=CVolCtl
ControlCount=0

[CLS:CVolCtl]
Type=0
HeaderFile=VolCtl.h
ImplementationFile=VolCtl.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CVolCtl

