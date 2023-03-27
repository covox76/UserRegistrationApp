#include "pch.h"
#include "UserRegistrationApp.h"

#include "AppDefs.h"
#include "UserRegistrationDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CUserRegistrationDlg::CUserRegistrationDlg(CWnd* parent /*=nullptr*/)
  : CDialogEx(IDD_USERREGISTRATIONAPP_DIALOG, parent)
{
  m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUserRegistrationDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialogEx::DoDataExchange(pDX);

  DDX_Text(pDX, IDC_FIRST_NAME_EDIT, m_firstName);
  DDX_Text(pDX, IDC_COUNTRY_EDIT, m_country);
  DDX_Text(pDX, IDC_LAST_NAME_EDIT, m_lastName);
  DDX_Text(pDX, IDC_EMAIL_EDIT, m_email);
  DDX_Text(pDX, IDC_USERNAME_EDIT, m_userName);
  DDX_Text(pDX, IDC_PASSWORD_EDIT, m_password);
}

BEGIN_MESSAGE_MAP(CUserRegistrationDlg, CDialogEx)
  ON_WM_PAINT()
  ON_WM_QUERYDRAGICON()
  ON_BN_CLICKED(IDC_SAVE_BUTTON, &CUserRegistrationDlg::OnBnClickedSaveButton)
  ON_BN_CLICKED(IDC_GENERATE_BUTTON, &CUserRegistrationDlg::OnBnClickedGenerateButton)
END_MESSAGE_MAP()

BOOL CUserRegistrationDlg::OnInitDialog()
{
  CDialogEx::OnInitDialog();

  SetIcon(m_hIcon, TRUE);
  SetIcon(m_hIcon, FALSE);

  return TRUE;
}

void CUserRegistrationDlg::OnPaint()
{
  if (IsIconic())
  {
    CPaintDC dc(this);

    SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

    CRect rect;
    GetClientRect(&rect);

    int cxIcon = GetSystemMetrics(SM_CXICON);
    int cyIcon = GetSystemMetrics(SM_CYICON);
    int x = (rect.Width() - cxIcon + 1) / 2;
    int y = (rect.Height() - cyIcon + 1) / 2;

    dc.DrawIcon(x, y, m_hIcon);
  }
  else
  {
    CDialogEx::OnPaint();
  }
}

HCURSOR CUserRegistrationDlg::OnQueryDragIcon()
{
  return static_cast<HCURSOR>(m_hIcon);
}

void CUserRegistrationDlg::OnBnClickedSaveButton()
{
  if (UpdateData() == FALSE)
  {
    AfxMessageBox(ErrorText::UnexpectedProblem, MB_OK | MB_ICONSTOP);

    return;
  }

  if (m_firstName.IsEmpty() || m_lastName.IsEmpty() || m_country.IsEmpty() ||
      m_email.IsEmpty() || m_userName.IsEmpty() || m_password.IsEmpty())
  {
    AfxMessageBox(WarningText::RequiredDataMissing, MB_OK | MB_ICONEXCLAMATION);
    
    return;
  }

  CWaitCursor waitCursor;
  GUID newId {};

  if (CoCreateGuid(&newId) == S_OK)
  {
    CString fileName;
    fileName.Format(L"%08lX%04hX%04hX%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX%s",
                    newId.Data1, newId.Data2, newId.Data3,
                    newId.Data4[0], newId.Data4[1], newId.Data4[2], newId.Data4[3],
                    newId.Data4[4], newId.Data4[5], newId.Data4[6], newId.Data4[7],
                    FileExtension::Json);

    boost::property_tree::ptree root;

    root.put("firstname", CT2CA(m_firstName.GetString()));
    root.put("lastname", CT2CA(m_lastName.GetString()));
    root.put("country", CT2CA(m_country.GetString()));
    root.put("email", CT2CA(m_email.GetString()));
    root.put("username", CT2CA(m_userName.GetString()));
    root.put("password", CT2CA(m_password.GetString()));

    std::string outputFile(CT2CA(fileName.GetString()));

    boost::property_tree::write_json(outputFile, root);

    AfxMessageBox(InfoText::OutputFileSaved, MB_OK | MB_ICONINFORMATION);
  }
  else
    AfxMessageBox(ErrorText::GuidGenerateProblem, MB_OK | MB_ICONSTOP);
}

void CUserRegistrationDlg::OnBnClickedGenerateButton()
{
  auto result = GetApiData();

  if (!result.empty())
  {
    boost::property_tree::ptree root;
    std::stringstream bufferData(result);

    boost::property_tree::json_parser::read_json(bufferData, root);

    for (const auto& [key, val] : root.get_child("results"))
    {
      m_firstName = val.get<std::string>("name.first").c_str();
      m_lastName = val.get<std::string>("name.last").c_str();
      m_country = val.get<std::string>("location.country").c_str();
      m_email = val.get<std::string>("email").c_str();
      m_userName = val.get<std::string>("login.username").c_str();
      m_password = val.get<std::string>("login.password").c_str();
    }

    if (UpdateData(FALSE))
      AfxMessageBox(InfoText::InputDataLoaded, MB_OK | MB_ICONINFORMATION);
  }
}

std::string CUserRegistrationDlg::GetApiData()
{
  namespace beast = boost::beast; // from <boost/beast.hpp>
  namespace http = beast::http;   // from <boost/beast/http.hpp>
  namespace net = boost::asio;    // from <boost/asio.hpp>
  namespace ssl = net::ssl;       // from <boost/asio/ssl.hpp>
  using tcp = net::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

  std::string resultData;

  try
  {
    CWaitCursor waitCursor;
    const auto host = "randomuser.me";
    const auto port = "443";
    const auto target = "/api";
    int version = 11;

    // The io_context is required for all I/O
    net::io_context ioc;

    // The SSL context is required, and holds certificates
    ssl::context ctx(ssl::context::tlsv12_client);

    // This holds the root certificate used for verification
    load_root_certificates(ctx);

    // Verify the remote server's certificate
    ctx.set_verify_mode(ssl::verify_peer);

    // These objects perform our I/O
    tcp::resolver resolver(ioc);
    beast::ssl_stream<beast::tcp_stream> stream(ioc, ctx);

    // Set SNI Hostname (many hosts need this to handshake successfully)
    if (!SSL_set_tlsext_host_name(stream.native_handle(), host))
    {
      beast::error_code ec{ static_cast<int>(::ERR_get_error()), net::error::get_ssl_category() };
      throw beast::system_error{ ec };
    }

    // Look up the domain name
    const auto results = resolver.resolve(host, port);

    // Make the connection on the IP address we get from a lookup
    beast::get_lowest_layer(stream).connect(results);

    // Perform the SSL handshake
    stream.handshake(ssl::stream_base::client);

    // Set up an HTTP GET request message
    http::request<http::string_body> req{ http::verb::get, target, version };
    req.set(http::field::host, host);
    req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

    // Send the HTTP request to the remote host
    http::write(stream, req);

    // This buffer is used for reading and must be persisted
    beast::flat_buffer buffer;

    // Declare a container to hold the response
    http::response<http::dynamic_body> res;

    // Receive the HTTP response
    http::read(stream, buffer, res);

    // Parse the message to JSON tree
    resultData = boost::beast::buffers_to_string(res.body().data());

    // Gracefully close the stream
    beast::error_code ec;
    stream.shutdown(ec);

    if (ec == boost::asio::ssl::error::stream_truncated)
    {
      // Rationale:
      // http://stackoverflow.com/questions/25587403/boost-asio-ssl-async-shutdown-always-finishes-with-an-error
      ec = {};
    }

    if (ec == net::error::eof)
      ec = {};

    if (ec)
      throw beast::system_error{ ec };

    // If we get here then the connection is closed gracefully
  }
  catch (const std::exception& e)
  {
    CString errorInfo(e.what());

    AfxMessageBox(errorInfo, MB_OK | MB_ICONSTOP);
  }

  return resultData;
}