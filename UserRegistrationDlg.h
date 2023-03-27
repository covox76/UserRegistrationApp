#pragma once

class CUserRegistrationDlg : public CDialogEx
{
public:
  CUserRegistrationDlg(CWnd* parent = nullptr);

protected:
  HICON m_hIcon;

  BOOL OnInitDialog() override;
  void DoDataExchange(CDataExchange* pDX) override;

  afx_msg void OnBnClickedSaveButton();
  afx_msg void OnBnClickedGenerateButton();
  afx_msg void OnPaint();
  afx_msg HCURSOR OnQueryDragIcon();

  DECLARE_MESSAGE_MAP()

private:
#ifdef AFX_DESIGN_TIME
  enum { IDD = IDD_USERREGISTRATIONAPP_DIALOG };
#endif

  CString m_firstName;
  CString m_lastName;
  CString m_country;
  CString m_email;
  CString m_userName;
  CString m_password;

  std::string GetApiData();
};
