// MinMax.h: interface for the CMinMax class.
//
//////////////////////////////////////////////////////////////////////

//#if !defined(AFX_MINMAX_H__2AD7E04C_7547_48A8_AA0D_6CE46D01770C__INCLUDED_)
//#define AFX_MINMAX_H__2AD7E04C_7547_48A8_AA0D_6CE46D01770C__INCLUDED_

#ifndef _MINMAX_H_
#define _MINMAX_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

__forceinline float ASMfabs(float src)
{
	/* C++ fabs for better results.. */
	return fabs(src);
}

__forceinline float ASMsqrt(float src)
{
	/* C++ sqrt better results.. */
	return sqrt(src);
}

int	RandomNumber(int iMin, int iMax);
float	RandomNumber( float fMin, float fMax);

inline D3DXVECTOR3 RandomNumber( D3DXVECTOR3 vMin, D3DXVECTOR3 vMax)
{
	D3DXVECTOR3 vecOut;
	vecOut.x = RandomNumber( vMin.x, vMax.x);
	vecOut.y = RandomNumber( vMin.y, vMax.y);
	vecOut.z = RandomNumber( vMin.z, vMax.z);
	return vecOut;
}

inline D3DXVECTOR2 RandomNumber(D3DXVECTOR2 vMin, D3DXVECTOR2 vMax)
{
	D3DXVECTOR2 vecOut;
	vecOut.x = RandomNumber( vMin.x, vMax.x);
	vecOut.y = RandomNumber( vMin.y, vMax.y);
	return vecOut;
}

inline D3DXCOLOR RandomNumber( D3DXCOLOR& Min, D3DXCOLOR& Max)
{
	D3DXCOLOR colorOut;
	colorOut.r = RandomNumber( Min.r, Max.r);
	colorOut.g = RandomNumber( Min.g, Max.g);
	colorOut.b = RandomNumber( Min.b, Max.b);
	colorOut.a = RandomNumber( Min.a, Max.a);
	return colorOut;
}

inline int RandomNumberTable( int iMin, int iMax )
{
	// 일단 테이블 안쓴다
	if (iMin == iMax) return(iMin);
	int m_iValue = ((rand() % (abs(iMax-iMin)+1))+iMin);
	return m_iValue;
}

inline float RandomNumberTable( float fMin, float fMax )
{
	static float random_table[] = {0.760335769f,
		0.78972326f,
		0.813092925f,
		0.634766477f,
		0.213374996f,
		0.579970483f,
		0.816556452f,
		0.670036516f,
		0.017380305f,
		0.916287718f,
		0.591425365f,
		0.588864375f,
		0.633372727f,
		0.261348543f,
		0.711030638f,
		0.829816421f,
		0.323970739f,
		0.715295295f,
		0.483957402f,
		0.896129804f,
		0.352902591f,
		0.374289994f,
		0.136213692f,
		0.802164647f,
		0.114804454f,
		0.68058216f,
		0.578330832f,
		0.190276552f,
		0.411072454f,
		0.270484074f,
		0.422331149f,
		0.417428729f,
		0.158811533f,
		0.383268087f,
		0.454588101f,
		0.702432458f,
		0.711908953f,
		0.501214747f,
		0.827224722f,
		0.292307875f,
		0.234411352f,
		0.855379208f,
		0.913321067f,
		0.404484384f,
		0.474208209f,
		0.413829063f,
		0.378893943f,
		0.939633982f,
		0.022753489f,
		0.009677673f,
		0.634068555f,
		0.629035666f,
		0.17394317f,
		0.797848108f,
		0.162497097f,
		0.214302874f,
		0.235831007f,
		0.391301715f,
		0.961292059f,
		0.30817297f,
		0.767131621f,
		0.626823958f,
		0.176450408f,
		0.207684268f,
		0.128197631f,
		0.607743945f,
		0.749374253f,
		0.808616543f,
		0.186813307f,
		0.807654262f,
		0.56288874f,
		0.409344021f,
		0.685186666f,
		0.600899753f,
		0.986348543f,
		0.844203824f,
		0.464735039f,
		0.629774138f,
		0.132924502f,
		0.951253045f,
		0.306516879f,
		0.511835015f,
		0.727553244f,
		0.184005038f,
		0.385686634f,
		0.783835697f,
		0.254117723f,
		0.073329777f,
		0.650792912f,
		0.150212618f,
		0.763350601f,
		0.286308482f,
		0.70239852f,
		0.511636155f,
		0.293784458f,
		0.493045826f,
		0.894528262f,
		0.517054008f,
		0.933874186f,
		0.000815168f,
		0.087890562f,
		0.458257226f,
		0.591490801f,
		0.50745716f,
		0.63823394f,
		0.184890712f,
		0.86035644f,
		0.866112748f,
		0.102999321f,
		0.419725714f,
		0.871152935f,
		0.594084969f,
		0.182156541f,
		0.571616656f,
		0.774723454f,
		0.414051981f,
		0.202642821f,
		0.041190091f,
		0.484236445f,
		0.30640818f,
		0.660707723f,
		0.115968309f,
		0.155562699f,
		0.825515574f,
		0.344707269f,
		0.964699374f,
		0.524244794f,
		0.296621913f,
		0.699769961f,
		0.892412054f,
		0.417001095f,
		0.193803041f,
		0.781359218f,
		0.70595345f,
		0.474288464f,
		0.571537234f,
		0.678209205f,
		0.321468578f,
		0.57742151f,
		0.395177741f,
		0.839077128f,
		0.795415813f,
		0.265601964f,
		0.402074501f,
		0.921801777f,
		0.74124538f,
		0.972297458f,
		0.773980414f,
		0.48579083f,
		0.641062913f,
		0.497737842f,
		0.337896998f,
		0.741697755f,
		0.383852337f,
		0.0544702f,
		0.891238417f,
		0.432956607f,
		0.197420907f,
		0.283873637f,
		0.011473649f,
		0.73664201f,
		0.972812698f,
		0.977993893f,
		0.326580415f,
		0.423030003f,
		0.959405401f,
		0.083581973f,
		0.250249709f,
		0.388082014f,
		0.328750086f,
		0.259478235f,
		0.587204176f,
		0.904628163f,
		0.554856781f,
		0.41372825f,
		0.984901137f,
		0.770062975f,
		0.405068062f,
		0.133132674f,
		0.666064311f,
		0.38058849f,
		0.065107974f,
		0.743282734f,
		0.505275558f,
		0.914221382f,
		0.354188216f,
		0.643041686f,
		0.502412663f,
		0.856459485f,
		0.117087381f,
		0.725931088f,
		0.045219547f,
		0.334420986f,
		0.062582528f,
		0.243588548f,
		0.355343874f,
		0.563723636f,
		0.239922693f,
		0.225698629f,
		0.200671364f,
		0.209759403f,
		0.049006333f,
		0.514107879f,
		0.741542342f,
		0.69069288f,
		0.576247933f,
		0.581189918f,
		0.432233708f,
		0.585489756f,
		0.88596909f,
		0.272981732f,
		0.577612172f,
		0.331519476f,
		0.618492812f,
		0.169046222f,
		0.762228868f,
		0.513151185f,
		0.24909409f,
		0.924746086f,
		0.191074026f,
		0.961530279f,
		0.152483736f,
		0.635010516f,
		0.237717005f,
		0.836625654f,
		0.243896062f,
		0.017658773f,
		0.53596463f,
		0.848400009f,
		0.652562928f,
		0.059684569f,
		0.925965821f,
		0.73890138f,
		0.085631495f,
		0.628144339f,
		0.84932199f,
		0.756105341f,
		0.02327832f,
		0.737493219f,
		0.10065083f,
		0.39047359f,
		0.824127573f,
		0.432391911f,
		0.343417715f,
		0.293751727f,
		0.706526702f,
		0.293935707f,
		0.37229624f,
		0.980125732f,
		0.00671058f,
		0.07861836f,
		0.104475891f,
		0.730615709f,
		0.590994649f,
		0.635575856f,
		0.627649069f,
		0.936014105f,
		0.492615512f,
		0.816447441f,
		0.780144319f,
		0.58617492f,
		0.340664114f,
		0.18593659f,
		0.366197811f,
		0.46986344f,
		0.265013721f,
		0.140657442f,
		0.731175517f,
		0.907775656f,
		0.561563056f,
		0.169873658f,
		0.798438734f,
		0.649660876f,
		0.078465288f,
		0.350285001f,
		0.911686429f,
		0.621355829f,
		0.210487815f,
		0.303150581f,
		0.1075104f,
		0.691631709f,
		0.949740126f,
		0.866927473f,
		0.076505698f,
		0.989804646f,
		0.558873987f,
		0.812016123f,
		0.630065819f,
		0.958299654f,
		0.488668522f,
		0.580690244f,
		0.40936222f,
		0.96471161f,
		0.309620262f,
		0.790915251f,
		0.782812296f,
		0.358487838f,
		0.185946958f,
		0.545546656f,
		0.916326128f,
		0.670652046f,
		0.337092212f,
		0.804978223f,
		0.60102852f,
		0.885951026f,
		0.977070453f,
		0.48328983f,
		0.689619238f,
		0.566703954f,
		0.253910812f,
		0.898962406f,
		0.93247208f,
		0.859115514f,
		0.655406611f,
		0.325455979f,
		0.530462996f,
		0.474930589f,
		0.883302963f,
		0.777439085f,
		0.983666474f,
		0.826392813f,
		0.489489827f,
		0.660340854f,
		0.335854747f,
		0.506866589f,
		0.489097444f,
		0.421985827f,
		0.999870319f,
		0.255983552f,
		0.382934265f,
		0.563428543f,
		0.797430513f,
		0.138029291f,
		0.358904869f,
		0.025936172f,
		0.779403095f,
		0.35798927f,
		0.800451189f,
		0.568240301f,
		0.166681509f,
		0.880213486f,
		0.701867176f,
		0.123948958f,
		0.388365443f,
		0.213655094f,
		0.821744576f,
		0.236262849f,
		0.292230189f,
		0.163576958f,
		0.49102575f,
		0.595710179f,
		0.487917012f,
		0.873935531f,
		0.453382861f,
		0.123997243f,
		0.855949345f,
		0.087394898f,
		0.270808164f,
		0.961026926f,
		0.950806838f,
		0.423424611f,
		0.802211414f,
		0.398723351f,
		0.839692442f,
		0.330272781f,
		0.376997091f,
		0.025057301f,
		0.927255268f,
		0.843953451f,
		0.461522757f,
		0.992225007f,
		0.534030114f,
		0.314183209f,
		0.842231419f,
		0.569308282f,
		0.529495292f,
		0.388379846f,
		0.361143958f,
		0.38130075f,
		0.141639274f,
		0.164558954f,
		0.149444331f,
		0.75218395f,
		0.788416486f,
		0.637637246f,
		0.969255229f,
		0.450866452f,
		0.728801836f,
		0.157749245f,
		0.145886558f,
		0.509789016f,
		0.0135947f,
		0.058198696f,
		0.001187739f,
		0.657169373f,
		0.849956032f,
		0.900533319f,
		0.413931106f,
		0.539055295f,
		0.144755835f,
		0.952679007f,
		0.914689044f,
		0.16311825f,
		0.61670675f,
		0.684315171f,
		0.271500243f,
		0.282124924f,
		0.276569416f,
		0.229657981f,
		0.933419154f,
		0.705782323f,
		0.670767138f,
		0.77701273f,
		0.57852898f,
		0.49099864f,
		0.991127385f,
		0.199998797f,
		0.961140488f,
		0.680787262f,
		0.434383648f,
		0.697333639f,
		0.559994187f,
		0.567963318f,
		0.674551948f,
		0.160061164f,
		0.052382488f,
		0.398803851f,
		0.622281283f,
		0.923350741f,
		0.075441317f,
		0.322366707f,
		0.724374489f,
		0.393195212f,
		0.156478797f,
		0.250760985f,
		0.354032116f,
		0.816360914f,
		0.808707237f,
		0.20739215f,
		0.276861387f,
		0.714270976f,
		0.985942927f,
		0.324148477f,
		0.355036914f,
		0.729327982f,
		0.635849666f,
		0.455791773f,
		0.226616248f,
		0.393895606f,
		0.273008945f,
		0.260989594f,
		0.436768458f,
		0.220089337f,
		0.444742835f,
		0.300427205f,
		0.307613153f,
		0.754454856f,
		0.794345415f,
		0.348871879f,
		0.24906126f,
		0.12463571f,
		0.349237389f,
		0.647187558f,
		0.466832881f,
		0.918671669f,
		0.92139297f,
		0.27191343f,
		0.649747234f,
		0.181684805f,
		0.48557471f,
		0.312015153f,
		0.256312498f,
		0.317420404f,
		0.624930234f,
		0.962510695f,
		0.433301383f,
		0.289274204f,
		0.006227514f,
		0.73671084f,
		0.061746683f,
		0.444308354f,
		0.064696335f,
		0.83534627f,
		0.896672254f,
		0.197387108f,
		0.77465648f,
		0.997812731f,
		0.345098777f,
		0.624445128f,
		0.543954202f,
		0.912271814f,
		0.05269625f,
		0.110174239f,
		0.688389393f,
		0.647467196f,
		0.341459055f,
		0.835936429f,
		0.968693663f,
		0.985014149f,
		0.55401904f,
		0.048142692f,
		0.332896065f,
		0.985988084f,
		0.314774569f,
		0.162487466f,
		0.257706641f,
		0.956595194f,
		0.127396817f,
		0.168375844f,
		0.909381334f,
		0.750154567f,
		0.648383843f };
	static int index = 0;
	if(++index>511)index = 0;
	return (random_table[index] * ASMfabs(fMax-fMin))+fMin;
}

inline D3DXVECTOR3 RandomNumberTable( D3DXVECTOR3 vMin, D3DXVECTOR3 vMax)
{
	D3DXVECTOR3 vecOut;
	vecOut.x = RandomNumberTable( vMin.x, vMax.x);
	vecOut.y = RandomNumberTable( vMin.y, vMax.y);
	vecOut.z = RandomNumberTable( vMin.z, vMax.z);
	return vecOut;
}

inline D3DXVECTOR2 RandomNumberTable(D3DXVECTOR2 vMin, D3DXVECTOR2 vMax)
{
	D3DXVECTOR2 vecOut;
	vecOut.x = RandomNumberTable( vMin.x, vMax.x);
	vecOut.y = RandomNumberTable( vMin.y, vMax.y);
	return vecOut;
}

inline D3DXCOLOR RandomNumberTable( D3DXCOLOR& Min, D3DXCOLOR& Max)
{
	D3DXCOLOR colorOut;
	colorOut.r = RandomNumberTable( Min.r, Max.r);
	colorOut.g = RandomNumberTable( Min.g, Max.g);
	colorOut.b = RandomNumberTable( Min.b, Max.b);
	colorOut.a = RandomNumberTable( Min.a, Max.a);
	return colorOut;
}

template <class T> class CMinMax
{
public:
	CMinMax()				{ m_Min = T(); m_Max = T(); m_Value = T(); }
	CMinMax(T tMin, T tMax) { m_Min = tMin; m_Max = tMax; }
	~CMinMax()				{}

	std::string toString(float fMin, float fMax)
	{
		std::stringstream stream;
		if( fMin == fMax )
		{
			stream << fMax;
		}
		else
		{
			stream << "random(" << fMin << "," << fMax << ")";
		}
		return stream.str();
	}
	std::string toString(D3DXVECTOR2 v2Min, D3DXVECTOR2 v2Max)
	{
		std::stringstream stream;
		stream << "XY(" << toString(v2Min.x,v2Max.x) << "," << toString(v2Min.y, v2Max.y) << ")";
		return stream.str();
	}
	std::string toString(D3DXVECTOR3 v3Min, D3DXVECTOR3 v3Max)
	{
		std::stringstream stream;
		stream << "XYZ("	<< toString(v3Min.x,v3Max.x) << ","
			<< toString(v3Min.y, v3Max.y) << ","
			<< toString(v3Min.z, v3Max.z) << ")";
		return stream.str();
	}
	std::string toString(D3DXCOLOR   colMin, D3DXCOLOR colMax)
	{
		std::stringstream stream;
		stream << "rgba("	<< toString(colMin.r,colMax.r) << ","
			<< toString(colMin.g,colMax.g) << ","
			<< toString(colMin.b,colMax.b) << ","
			<< toString(colMin.a,colMax.a) << ")";
		return stream.str();
	}

	std::string toString()
	{
		return toString(m_Min, m_Max);
	}

	T m_Min;
	T m_Max;
	T m_Value;

	T GetRandomNumInRange(void) { return(RandomNumber(m_Min, m_Max)); }
	inline T GetRandomNumInRangeTable(void) { return(RandomNumberTable(m_Min, m_Max)); }
	T GetRange(void)			{ return(abs(m_Max-m_Min)); }
};

#endif // _MINMAX_H_
