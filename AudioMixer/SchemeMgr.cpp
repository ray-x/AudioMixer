// SchemeMgr.cpp : implementation file
//

#include "stdafx.h"
#include "AudioMixer.h"
#include "SchemeMgr.h"
#include "ConfData.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#include "Wait.h"
#include <sstream>

// #include "inifile.h"
// using namespace std;
/////////////////////////////////////////////////////////////////////////////
// CSchemeMgr property page

IMPLEMENT_DYNCREATE(CSchemeMgr, CPropertyPage)

CSchemeMgr::CSchemeMgr() : CPropertyPage(CSchemeMgr::IDD)
{
	//{{AFX_DATA_INIT(CSchemeMgr)
	m_strScheName = _T("");
	m_idxScheme = -1;
	m_SchemeName = _T("");
	m_SchemeNum = _T("");
	//}}AFX_DATA_INIT
}

CSchemeMgr::~CSchemeMgr()
{
	//m_IniFile.
}

void CSchemeMgr::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSchemeMgr)
	DDX_Control(pDX, IDC_EDIT_SCHEME_NUM, m_editSchemeNum);
	DDX_Control(pDX, IDC_EDIT_SCHEME_NAME, m_editSchemeName);
	DDX_Control(pDX, IDC_LIST_SCHEME, m_lstScheme);
	DDX_Text(pDX, IDC_EDIT_SCHEME_NAME, m_SchemeName);
	DDX_Text(pDX, IDC_EDIT_SCHEME_NUM, m_SchemeNum);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSchemeMgr, CPropertyPage)
	//{{AFX_MSG_MAP(CSchemeMgr)
	ON_BN_CLICKED(IDC_BUTTON_EXPORT, OnButtonExport)
	ON_BN_CLICKED(IDC_BUTTON_EMPTY, OnButtonDeleteScheme)
	ON_NOTIFY(NM_CLICK, IDC_LIST_SCHEME, OnClickListScheme)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, OnButtonApply)
	ON_BN_CLICKED(IDC_BUTTON_IMPORT, OnButtonImport)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSchemeMgr message handlers



void CSchemeMgr::OnEditchangeComboScheme() 
{
	// TODO: Add your control notification handler code here
	
}

void CSchemeMgr::OnDblclkComboScheme() 
{
	// TODO: Add your control notification handler code here
	
}

int CALLBACK ListCompare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	// lParamSort contains a pointer to the list view control.
	// The lParam of an item is just its index.
	CListCtrl* pListCtrl = (CListCtrl*) lParamSort;
	int item1,item2;
	CString     strItem1 = pListCtrl->GetItemText(lParam1, 0);

	CString     strItem2 = pListCtrl->GetItemText(lParam2, 0);
	item1=atoi(strItem1);

	item2=atoi(strItem2);

	return (item2>item1?0:1  );
}

BOOL CSchemeMgr::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	m_IniFile.Load("conf.ini",true);
	tstd::tstringstream ss1;

	CIniSection *pIniSection;
	tstd::tstringstream  sectName;
	tstd::tstring  SchemeName;
	char buf[3];

	m_lstScheme.InsertColumn(0, "编号", LVCFMT_LEFT, -1, 0);
	m_lstScheme.InsertColumn(1, "方案名称", LVCFMT_LEFT, -1, 1);
	// Insert first row

	// Set column widths (an optional nice touch)
	m_lstScheme.SetColumnWidth(0, 50);
	m_lstScheme.SetColumnWidth(1, 160);

	int Index;

	// Insert second row
	tstd::tstring	tstrName;

	for (int i=0;i<10;i++)
	{

		//sectName<<"scheme"<<i;
		pIniSection=m_IniFile.GetSection(tstd::tstring("scheme")+tstd::tstring(itoa(i+1,buf,10)));
		tstrName=tstd::tstring(itoa(i+1,buf,10));//+tstd::tstring(" ");

		if (pIniSection)
		{
			CIniKey *pIniKey=pIniSection->GetKey("name");
			SchemeName=pIniKey->GetValue();
	
			Index = m_lstScheme.InsertItem(LVIF_TEXT, 1, tstrName.c_str(), 0, 0, 0, NULL);
			m_lstScheme.SetItem(Index, 1, LVIF_TEXT, SchemeName.c_str(), 0, 0, 0, NULL);

		}else
		{
			Index = m_lstScheme.InsertItem(LVIF_TEXT, 1, tstrName.c_str(), 0, 0, 0, NULL);
		}
	}


     int j = m_lstScheme.GetItemCount();  
     for (int k=0;k<i;k++)  
     {  
         m_lstScheme.SetItemData(k,k);  
     }  
	m_lstScheme.SortItems( ListCompare, (DWORD)&m_lstScheme );


	UpdateData(FALSE);
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSchemeMgr::OnButtonDeleteScheme() 
{
	// TODO: Add your control notification handler code here

	int nItem;

	POSITION p=m_lstScheme.GetFirstSelectedItemPosition();
	while (p)
	{
		nItem = m_lstScheme.GetNextSelectedItem(p);
		m_SchemeNum= m_lstScheme.GetItemText(nItem, 0);
		m_SchemeName=m_lstScheme.GetItemText(nItem, 1);
		// Do something with item nSelected
	}
	
	std::string strSectName(m_SchemeNum.GetBuffer(2));
	strSectName="Scheme"+strSectName;
	
	std::string val;
	//CIniSection *pSection= m_IniFile.GetSection(strSectName);


	m_IniFile.RemoveSection(strSectName);
	m_IniFile.Save("conf.ini");
	
	m_lstScheme.DeleteAllItems();

	tstd::tstring	tstrName;
	CIniSection *pIniSection;
	std::string SchemeName;
	int Index;
	char buf[2];

	for (int i=0;i<10;i++)
	{
		
		//sectName<<"scheme"<<i;
		pIniSection=m_IniFile.GetSection(tstd::tstring("scheme")+tstd::tstring(itoa(i+1,buf,10)));
		tstrName=tstd::tstring(itoa(i+1,buf,10));//+tstd::tstring(" ");
		
		if (pIniSection)
		{
			CIniKey *pIniKey=pIniSection->GetKey("name");
			SchemeName=pIniKey->GetValue();
			
			Index = m_lstScheme.InsertItem(LVIF_TEXT, 1, tstrName.c_str(), 0, 0, 0, NULL);
			m_lstScheme.SetItem(Index, 1, LVIF_TEXT, SchemeName.c_str(), 0, 0, 0, NULL);
			
		}else
		{
			Index = m_lstScheme.InsertItem(LVIF_TEXT, 1, tstrName.c_str(), 0, 0, 0, NULL);
		}
	}
	
	
	int j = m_lstScheme.GetItemCount();  
	for (int k=0;k<i;k++)  
	{  
		m_lstScheme.SetItemData(k,k);  
	}  
	m_lstScheme.SortItems( ListCompare, (DWORD)&m_lstScheme );
	
	
	UpdateData(FALSE);

}

void CSchemeMgr::OnClickListScheme(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	::SendMessage(m_lstScheme.m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE,
    LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	
	
	UINT uSelectedCount = m_lstScheme.GetSelectedCount();
	int  nItem = -1;

	POSITION p=m_lstScheme.GetFirstSelectedItemPosition();
	while (p)
	{
		nItem = m_lstScheme.GetNextSelectedItem(p);
		m_SchemeNum= m_lstScheme.GetItemText(nItem, 0);
		m_SchemeName=m_lstScheme.GetItemText(nItem, 1);
		UpdateData(FALSE);
		// Do something with item nSelected
	}

}

void CSchemeMgr::OnButtonSave() 
{
	// TODO: Add your control notification handler code here
	// Save current scheme



	UpdateData(TRUE);
	CString SectionName("Scheme");
	SectionName=SectionName+m_SchemeNum;
	CIniSection *pSection=m_IniFile.AddSection(std::string(SectionName.GetBuffer(32)));

	std::string val;
	int i;
	char buf[2];

	pSection->SetKeyValue("channel",std::string(itoa( extConfigData.m_numCh,buf,10)));

	//
	val.erase();
	int vol;
	for (i=0;i<8;i++)
	{
		vol= (i<4||extConfigData.m_numCh==8)? extConfigData.m_InputConf[i]:0;
		val=val+itoa( vol, buf,10);
		if(i<7) val+="\40";
	}
	pSection->SetKeyValue("ivol",val);

	//mix information
	int j;
	for (j=0;j<8;j++)
	{
		val.erase();

		for (i=0;i<8;i++)
		{
			vol= (i<4||extConfigData.m_numCh==8)? extConfigData.m_ChannelConf[j][i]:0;
			val=val+itoa( vol, buf,10);
			if(i<7) val+="\40";
		}
		std::string key=std::string("mix")+std::string(itoa(i,buf,10));

		pSection->SetKeyValue(key,val);
	}
	

	//name of section
	pSection->SetKeyValue("name", std::string(m_SchemeName.GetBuffer(32)) );


	val.erase();
	for (i=0;i<8;i++)
	{
		vol= (i<4||extConfigData.m_numCh==8)? extConfigData.m_OutputConf[i]:0;
		val=val+itoa( vol, buf,10);
		if(i<7) val+="\40";
	}
	pSection->SetKeyValue("ivol",val);

	//m_IniFile.SetKeyValue(std::string(SectionName.GetBuffer(32)), );

	SectionName.ReleaseBuffer(32);
	m_SchemeName.ReleaseBuffer(32);
	m_IniFile.Save("conf.ini");
	
	
}

void CSchemeMgr::OnButtonApply() 
{
	// TODO: Add your control notification handler code here

	UINT i,j, uSelectedCount = m_lstScheme.GetSelectedCount();
	int  nItem = -1;
	char buf[2];

	POSITION p=m_lstScheme.GetFirstSelectedItemPosition();
	while (p)
	{
		nItem = m_lstScheme.GetNextSelectedItem(p);
		m_SchemeNum= m_lstScheme.GetItemText(nItem, 0);
		m_SchemeName=m_lstScheme.GetItemText(nItem, 1);
		// Do something with item nSelected
	}
	
	std::string strSectName(m_SchemeNum.GetBuffer(2));
	strSectName="Scheme"+strSectName;

	std::string val;
	CIniSection *pSection= m_IniFile.GetSection(strSectName);
	
	val=pSection->GetKeyValue("channel");
	extConfigData.m_numCh=atoi(val.c_str());
	

	val.erase();
	val=pSection->GetKeyValue("ivol");
	std::stringstream ss(val);

	
	for (i=0;i<8;i++ )
	{
		ss>>extConfigData.m_InputConf[i];
		
		
		extConfigData.m_InputConf[i]=(i<4||extConfigData.m_numCh==8)?extConfigData.m_InputConf[i]:0;
	}

	val.erase();
	for (j=0;j<8;j++)
	{
		ss.clear();
		std::string mixName=std::string("mix")+std::string(itoa(j+1,buf,10));
		ss<<pSection->GetKeyValue(mixName);
	
		for (i=0;i<8;i++ )
		{
			ss>>extConfigData.m_ChannelConf[j][i];
		
			extConfigData.m_ChannelConf[j][i]=(i<4||extConfigData.m_numCh==8)?extConfigData.m_ChannelConf[j][i]:0;
		}
	}

	val.erase();
	ss<<pSection->GetKeyValue("ovol");
	
	
	for (i=0;i<8;i++ )
	{
		ss>>extConfigData.m_OutputConf[i];
	
		extConfigData.m_OutputConf[i]=(i<4||extConfigData.m_numCh==8)?extConfigData.m_OutputConf[i]:0;
	}

	CWait dlg(0,NULL);
	dlg.Init();
	dlg.InitConfBuf();
	dlg.SendData();
	AfxMessageBox(_T("配置应用完成"),MB_OK,0);	
}

void CSchemeMgr::OnButtonExport() 
{
	// TODO: Add your control notification handler code here
	//ShellExecute(NULL,"open",".\\ReadMe.txt",NULL,NULL,SW_SHOWNORMAL);
	CFileDialog DlgFile(FALSE,"ini",NULL,OFN_OVERWRITEPROMPT,"ini 文件 (*.ini)|*.ini||");
	DlgFile.m_ofn.lpstrTitle=_T("导出配置文件");
	DlgFile.m_ofn.lpstrInitialDir=".\\";
	CString filename;
	DlgFile.DoModal();
	filename=_T(DlgFile.GetPathName());
	if (!filename.IsEmpty())
		m_IniFile.Save(_T(filename.GetBuffer(24)));
	
}

void CSchemeMgr::OnButtonImport() 
{
	// TODO: Add your control notification handler code here
	CFileDialog DlgFile(TRUE,"*.ini",NULL,OFN_OVERWRITEPROMPT,"ini 文件 (*.ini)|*.ini||");
	DlgFile.m_ofn.lpstrTitle=_T("导入配置文件");
	DlgFile.m_ofn.lpstrInitialDir=".\\";
	CString filename;
	DlgFile.DoModal();

	filename=_T(DlgFile.GetPathName());
	if (filename.IsEmpty())
		return;
	//m_IniFile.Save(_T(filename.GetBuffer(24)));
	
	//CIniFile newIniFile;
	m_IniFile.Load(tstd::tstring(filename.GetBuffer(32)),false);
	//m_IniFile=newIniFile;
	
	m_lstScheme.DeleteAllItems();
	
	tstd::tstring	tstrName;
	CIniSection *pIniSection;
	std::string SchemeName;
	int Index;
	char buf[2];
	
	for (int i=0;i<10;i++)
	{
		
		//sectName<<"scheme"<<i;
		pIniSection=m_IniFile.GetSection(tstd::tstring("scheme")+tstd::tstring(itoa(i+1,buf,10)));
		tstrName=tstd::tstring(itoa(i+1,buf,10));//+tstd::tstring(" ");
		
		if (pIniSection)
		{
			CIniKey *pIniKey=pIniSection->GetKey("name");
			SchemeName=pIniKey->GetValue();
			
			Index = m_lstScheme.InsertItem(LVIF_TEXT, 1, tstrName.c_str(), 0, 0, 0, NULL);
			m_lstScheme.SetItem(Index, 1, LVIF_TEXT, SchemeName.c_str(), 0, 0, 0, NULL);
			
		}else
		{
			Index = m_lstScheme.InsertItem(LVIF_TEXT, 1, tstrName.c_str(), 0, 0, 0, NULL);
		}
	}
	
	
	int j = m_lstScheme.GetItemCount();  
	for (int k=0;k<i;k++)  
	{  
		m_lstScheme.SetItemData(k,k);  
	}  
	m_lstScheme.SortItems( ListCompare, (DWORD)&m_lstScheme );
	
	
	UpdateData(FALSE);
	
}
