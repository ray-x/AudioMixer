// VolTab.cpp : implementation file
//

#include "stdafx.h"
#include "AudioMixer.h"
#include "VolTab.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVolTab

CVolTab::CVolTab()
{
	m_nNumOfPages = 0;
	m_nCurrentPage = 0;
}

CVolTab::~CVolTab()
{
}


BEGIN_MESSAGE_MAP(CVolTab, CTabCtrl)
	//{{AFX_MSG_MAP(CVolTab)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVolTab message handlers
BOOL CVolTab::AddPage(LPCTSTR title, CDialog *pDialog,UINT ID)
{
	if( MAXPAGE == m_nNumOfPages )
		return FALSE;
	
	m_nNumOfPages++;
	
	m_pPages[m_nNumOfPages-1] = pDialog;
	m_IDD[m_nNumOfPages-1] = ID;
	m_Title[m_nNumOfPages-1] = title;
	
	return TRUE;
}

void CVolTab::SetRect()
{
	CRect tabRect, itemRect;
	int nX, nY, nXc, nYc;
	
	GetClientRect(&tabRect);
	GetItemRect(0, &itemRect);
	
	nX=itemRect.left;
	nY=itemRect.bottom+1;
	nXc=tabRect.right-itemRect.left-2;
	nYc=tabRect.bottom-nY-2;
	
	m_pPages[0]->SetWindowPos(&wndTop, nX, nY, nXc, nYc, SWP_SHOWWINDOW);
	for( int nCount=1; nCount < m_nNumOfPages; nCount++ )
		m_pPages[nCount]->SetWindowPos(&wndTop, nX, nY, nXc, nYc, SWP_HIDEWINDOW);
	
}

void CVolTab::Show()
{
	int i;
	for( i=0; i < m_nNumOfPages; i++ )
	{
		m_pPages[i]->Create( m_IDD[i], this );
		InsertItem( i, m_Title[i] );
	}
	
	m_pPages[0]->ShowWindow(SW_SHOW);
	for(  i=1; i < m_nNumOfPages; i++)
		m_pPages[i]->ShowWindow(SW_HIDE);
	
	SetRect();
	
}


int CVolTab::SetCurSel(int nItem)
{
	if( nItem < 0 || nItem >= m_nNumOfPages)
		return -1;
	
	int ret = m_nCurrentPage;
	
	if(m_nCurrentPage != nItem )
	{
		m_pPages[m_nCurrentPage]->ShowWindow(SW_HIDE);
		m_nCurrentPage = nItem;
		m_pPages[m_nCurrentPage]->ShowWindow(SW_SHOW);
		//		m_pPages[m_nCurrentPage]->SetFocus();
		CTabCtrl::SetCurSel(nItem);
	}else
		m_pPages[m_nCurrentPage]->ShowWindow(SW_SHOW);
	
	return ret;
}

int CVolTab::GetCurSel()
{
	return CTabCtrl::GetCurSel();
}

void CVolTab::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CTabCtrl::OnLButtonDown(nFlags, point);
	
	if(m_nCurrentPage != GetCurFocus())
	{
		m_pPages[m_nCurrentPage]->ShowWindow(SW_HIDE);
		m_nCurrentPage=GetCurFocus();
		m_pPages[m_nCurrentPage]->ShowWindow(SW_SHOW);
		//		m_pPages[m_nCurrentPage]->SetFocus();
	}else
	{
		m_pPages[m_nCurrentPage]->ShowWindow(SW_HIDE);
		m_pPages[m_nCurrentPage]->ShowWindow(SW_SHOW);
	}

}
