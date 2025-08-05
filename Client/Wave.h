// Wave.h: interface for the CWave class.
//
//////////////////////////////////////////////////////////////////////

//#if !defined(AFX_WAVE_H__8C5C61A7_1063_4C61_89A4_C4D018EB4A1D__INCLUDED_)
//#define AFX_WAVE_H__8C5C61A7_1063_4C61_89A4_C4D018EB4A1D__INCLUDED_

#ifndef _WAVE_H_
#define _WAVE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



class CWave  
{

	public:
		CWave();
		virtual ~CWave();

		void Init( float height );
		void FrameMove();
		void Render();

	private:
		SLVERTEX				m_FrontWaveVertex[4];
		SLVERTEX				m_BackWaveVertex[4];
		GCDeviceTexture*	    m_pWaterBack;
		GCDeviceTexture*	    m_pWaterFront;
};

#endif // _WAVE_H_
