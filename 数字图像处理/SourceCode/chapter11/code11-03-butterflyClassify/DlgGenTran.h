#if !defined(AFX_DLGGEOTRAN_H__CCF6435B_255A_426D_8BF9_E6022B7074D8__INCLUDED_)
#define AFX_DLGGEOTRAN_H__CCF6435B_255A_426D_8BF9_E6022B7074D8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgGeoTran.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgGeoTran dialog

class CDlgGenTran : public CDialog
{
// Construction
public:
	CDlgGenTran(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgGeoTran)
	enum { IDD = IDD_Dlg_GenTrans };
	CSpinButtonCtrl	m_ctrlYSpin;
	CSpinButtonCtrl	m_ctrlXSpin;
	long	m_XOffset;
	long	m_YOffset;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgGeoTran)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgGeoTran)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGGEOTRAN_H__CCF6435B_255A_426D_8BF9_E6022B7074D8__INCLUDED_)
