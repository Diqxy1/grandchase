template <class T0>
KFootHold<T0>::KFootHold(void) : m_fX(0), m_iIndexX(0), m_fY(0), m_iIndexY(0),
                                m_dwFHProp(KFootHold::FHP_JUST_FOOTHOLD),
                                m_dwFHType(KFootHold::FHT_INIT),
                                m_fLocalX(0), m_fLocalY(0),
                                m_fPrevX(0), m_iPrevIndexX(0), m_fPrevY(0), m_iPrevIndexY(0),
                                m_strFHValue("")
{
}

template <class T0>
KFootHold<T0>::~KFootHold(void)
{
}
