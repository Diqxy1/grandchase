// DlgEditValue.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool_Particle.h"
#include "DlgEditValue.h"
#include ".\dlgeditvalue.h"
//#include <sstream>
//#include <string>


char* EDIT_VALUE_TYPE_STR[CKTDGParticleSystem::EVENT_TYPE_NUM+1][4] = 
{
	{ "X","Y","","", },
	{ "R","G","B","A",},
	{ "X","Y","Z","", },	
	{ "X","Y","Z","", },
	{ "X","Y","Z","", },
	{ "X","","","" },
	{ "Y","","","" },
	{ "Z","","","" },
	{ "X","","","" },
	{ "Y","","","" },
	{ "Z","","","" },
	{ "Time", "","","" },
	{ "value","","","" },
};

char* EDIT_VALUE_TYPE_HEAD[CKTDGParticleSystem::EVENT_TYPE_NUM+1] =
{
	"XY(",	
	"RGBA(",
	"XYZ(",
	"XYZ(",
	"XYZ(",
	"",
};

char* EDIT_VALUE_TYPE_DEFAULT[CKTDGParticleSystem::EVENT_TYPE_NUM+1] =
{
	"XY(0,0)",	
	"rgba(1,1,1,1)",
	"XYZ(0,0,0)",
	"XYZ(0,0,0)",
	"XYZ(0,0,0)",
	"0",
	"0",
	"0",
	"0",
	"0",
	"0",
	"0",
	"0",
};

// CDlgEditValue 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgEditValue, CDialog)
CDlgEditValue::CDlgEditValue(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgEditValue::IDD, pParent)	
	, m_strValue(_T(""))
	, m_bFade(FALSE)
{
	memset(m_fMin,0,sizeof(m_fMin));
	memset(m_fMax,0,sizeof(m_fMax));
}

CDlgEditValue::~CDlgEditValue()
{
}

void CDlgEditValue::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);	
	DDX_Check(pDX, IDC_CHECK1, m_bFade);
	DDX_Text(pDX, IDC_STATIC1, m_strTitle[0]);
	DDX_Text(pDX, IDC_STATIC2, m_strTitle[1]);
	DDX_Text(pDX, IDC_STATIC3, m_strTitle[2]);
	DDX_Text(pDX, IDC_STATIC4, m_strTitle[3]);

	DDX_Control(pDX, IDC_Min1, m_editMin[0]);
	DDX_Control(pDX, IDC_Min2, m_editMin[1]);
	DDX_Control(pDX, IDC_MIN3, m_editMin[2]);
	DDX_Control(pDX, IDC_MIN4, m_editMin[3]);

	DDX_Control(pDX, IDC_MAX1, m_editMax[0]);
	DDX_Control(pDX, IDC_MAX2, m_editMax[1]);
	DDX_Control(pDX, IDC_MAX3, m_editMax[2]);
	DDX_Control(pDX, IDC_MAX4, m_editMax[3]);

	DDX_Text(pDX, IDC_Min1, m_fMin[0]);
	DDX_Text(pDX, IDC_Min2, m_fMin[1]);
	DDX_Text(pDX, IDC_MIN3, m_fMin[2]);
	DDX_Text(pDX, IDC_MIN4, m_fMin[3]);

	DDX_Text(pDX, IDC_MAX1, m_fMax[0]);
	DDX_Text(pDX, IDC_MAX2, m_fMax[1]);
	DDX_Text(pDX, IDC_MAX3, m_fMax[2]);
	DDX_Text(pDX, IDC_MAX4, m_fMax[3]);
}


BEGIN_MESSAGE_MAP(CDlgEditValue, CDialog)	
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_MIN, OnBnClickedMin)
	ON_BN_CLICKED(IDC_MAX, OnBnClickedMax)
END_MESSAGE_MAP()


// CDlgEditValue 메시지 처리기입니다.
void CDlgEditValue::SetValue( CKTDGParticleSystem::EventType eType, CString strValue, BOOL bFade )
{
	m_eType = eType;
	
	m_strValue = strValue;
	m_bFade = bFade;

	if( m_strValue == "" )
	{
		m_strValue = EDIT_VALUE_TYPE_DEFAULT[m_eType];		
	}
}

void CDlgEditValue::SetHelpText()
{
}

void CDlgEditValue::OnBnClickedOk()
{
	// 값을 검사해봅니다...
	// 통과하지 못하면...대략 좋지 않음..-_-ㅋ
	UpdateData();

	
	m_strValue = EDIT_VALUE_TYPE_HEAD[m_eType];

	for(int i = 0; i < 4; i++)
	{
		if( !m_editMin[i].IsWindowVisible() )
			break;

		std::stringstream stream;

		if( i != 0 )
			stream << ",";

		if( m_fMin[i] == m_fMax[i] )
		{			
			stream <<  m_fMin[i];			
		}
		else
		{
			stream << "random(" << m_fMin[i] << "," << m_fMax[i] << ")";			
		}
		m_strValue += stream.str().c_str();
	}
	if( m_eType <= CKTDGParticleSystem::EVENT_BLACKHOLE )
		m_strValue += ")";

	OnOK();
}

BOOL CDlgEditValue::OnInitDialog()
{
	CDialog::OnInitDialog();

	CKTDGParticleSystem::CParticleEmitterTokenizer tokenizer;
	try
	{
		tokenizer.Tokenize( (LPCTSTR)m_strValue );
		std::vector<CKTDGParticleSystem::CParticleEmitterToken>::iterator IterCur	= tokenizer.m_TokenVector.begin();
		std::vector<CKTDGParticleSystem::CParticleEmitterToken>::iterator IterEnd	= tokenizer.m_TokenVector.end();
		
		int i = 0;

		switch( m_eType )
		{
		case CKTDGParticleSystem::EVENT_COLOR:
			{
				CMinMax<D3DXCOLOR> Color;
				CKTDGParticleSystem::CParticleEmitterTokenizer::ProcessColor(Color, IterCur, IterEnd);
				
				for( i = 0; i < 4; i++)
				{		
					m_strTitle[i] = EDIT_VALUE_TYPE_STR[m_eType][i];			
					m_editMin[i].ShowWindow(SW_SHOW);
					m_editMax[i].ShowWindow(SW_SHOW);
				}
				m_fMin[0] = Color.m_Min.r;
				m_fMin[1] = Color.m_Min.g;
				m_fMin[2] = Color.m_Min.b;
				m_fMin[3] = Color.m_Min.a;

				m_fMax[0] = Color.m_Max.r;
				m_fMax[1] = Color.m_Max.g;
				m_fMax[2] = Color.m_Max.b;
				m_fMax[3] = Color.m_Max.a;
			}
			break;
		case CKTDGParticleSystem::EVENT_SIZE:
			{
				CMinMax<D3DXVECTOR2> v2;
				CKTDGParticleSystem::CParticleEmitterTokenizer::ProcessVector2(v2, IterCur, IterEnd);

				m_fMin[0] = v2.m_Min.x;
				m_fMin[1] = v2.m_Min.y;

				m_fMax[0] = v2.m_Max.x;
				m_fMax[1] = v2.m_Max.y;

				for( i = 0; i < 4; i++)
				{		
					m_strTitle[i] = EDIT_VALUE_TYPE_STR[m_eType][i];			
					m_editMin[i].ShowWindow(i < 2 ? SW_SHOW : SW_HIDE);
					m_editMax[i].ShowWindow(i < 2 ? SW_SHOW : SW_HIDE);
				}
			}
			break;		
		case CKTDGParticleSystem::EVENT_ROTATE:
		case CKTDGParticleSystem::EVENT_VELOCITY:
			{
				CMinMax<D3DXVECTOR3> v3;
				CKTDGParticleSystem::CParticleEmitterTokenizer::ProcessVector(v3, IterCur, IterEnd);

				m_fMin[0] = v3.m_Min.x;
				m_fMin[1] = v3.m_Min.y;
				m_fMin[2] = v3.m_Min.z;

				m_fMax[0] = v3.m_Max.x;
				m_fMax[1] = v3.m_Max.y;
				m_fMax[2] = v3.m_Max.z;
			
				for( i = 0; i < 4; i++)
				{		
					m_strTitle[i] = EDIT_VALUE_TYPE_STR[m_eType][i];			
					m_editMin[i].ShowWindow( i < 3 ? SW_SHOW : SW_HIDE );
					m_editMax[i].ShowWindow( i < 3 ? SW_SHOW : SW_HIDE );
				}
			}
			break;
		default:
			{
				CMinMax<float> fValue;
				CKTDGParticleSystem::CParticleEmitterTokenizer::ProcessNumber(fValue, IterCur, IterEnd);

				m_fMin[0] = fValue.m_Min;
				m_fMax[0] = fValue.m_Max;				

				for( i = 0; i < 4; i++)
				{		
					m_strTitle[i] = EDIT_VALUE_TYPE_STR[m_eType][i];			
					m_editMin[i].ShowWindow(i < 1 ? SW_SHOW : SW_HIDE);
					m_editMax[i].ShowWindow(i < 1 ? SW_SHOW : SW_HIDE);
				}
			}
			break;
		}
	}
	catch(...)
	{
	}
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDlgEditValue::OnBnClickedMin()
{	
	UpdateData();
	for(int i = 0; i < 4; i++)
	{
		m_fMax[i] = m_fMin[i];
	}
	UpdateData(FALSE);
}

void CDlgEditValue::OnBnClickedMax()
{
	UpdateData();
	for(int i = 0; i < 4; i++)
	{
		m_fMin[i] = m_fMax[i];
	}
	UpdateData(FALSE);
}
