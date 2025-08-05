#pragma once

class KGCDepotChar : public KD3DWnd    // extends
	, public KActionListener   // implements
{
public:
	DECLARE_CLASSNAME(KGCDepotChar);
	/// Default constructor
	KGCDepotChar(void);
	/// Default destructor
	virtual ~KGCDepotChar(void);
	/// Copy constructor
	KGCDepotChar(const KGCDepotChar&)
	{
		ASSERT(false && "Do not use copy constructor!");
	}
	/// Assignment operator
	const KGCDepotChar& operator=(const KGCDepotChar&)
	{
		ASSERT(false && "Do not use assignment operator!"); return *this;
	}

public:
	virtual void ActionPerformed(const KActionEvent& event);
	virtual void OnCreate(void);
	void SetChar(int iChar);
private:
	std::map<int, KD3DWnd*> m_mapCharInfo;
};

DEFINE_WND_FACTORY(KGCDepotChar);
DECLARE_WND_FACTORY(KGCDepotChar);
DECLARE_WND_FACTORY_NAME(KGCDepotChar);
