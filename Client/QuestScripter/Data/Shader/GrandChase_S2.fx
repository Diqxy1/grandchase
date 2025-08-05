float4   g_LightDir = {-1.0,-1.0,1.0,0.0};                  // Light's direction in world space
float4x4 g_mWorld;                  // World matrix for object
float4x4 g_mViewProj;    // World * View * Projection matrix
float4x4 g_mWorldViewProj;    // World * View * Projection matrix

float4x4 g_BoneMatrix[40];
int		 g_PositionBone = 16;
float	 fOutLineWide = 2.0f;
float4   g_vEdgeColor = {0.0, 0.0, 0.0, 1.0};

struct VS_SKININPUT
{
    float4 vPos : POSITION;
    int    vBlendIndices    : BLENDINDICES;
    float3 vNormal : NORMAL;
    float2 vTexCoord0 : TEXCOORD0; 	
};

struct VS_EDGEINPUT
{
    float4 vPos : POSITION;
    int    vBlendIndices    : BLENDINDICES;
    float3 vNormal : NORMAL;
};

struct VS_OUTPUT
{
    float4 Position		: POSITION;   // vertex position    
    float2 Tex0			: TEXCOORD0;  // vertex texture coords
    float2 Tex1			: TEXCOORD1;  // vertex texture coords
};

struct VS_OUTPUT2
{
    float4 Position		: POSITION;   // vertex position
    float2 Tex0			: TEXCOORD0;  // vertex texture coords
};

struct VS_OUTPUT_EDGE
{
    float4 Position		: POSITION;   // vertex position
    float4 Color		: COLOR;	  // vertex color
};

VS_OUTPUT RenderCartoonVS( VS_SKININPUT input )
{
    VS_OUTPUT Output;
    float3 vNormalWorldSpace;    
    float4 temp;
    
    // Transform the position from object space to homogeneous projection space    
    temp = mul(input.vPos, g_BoneMatrix[input.vBlendIndices - g_PositionBone] ); 
    Output.Position = mul(temp, g_mWorldViewProj);
    
    
    // Transform the normal from object space to world space    
    temp = mul(input.vNormal, g_BoneMatrix[input.vBlendIndices - g_PositionBone] );
    vNormalWorldSpace = normalize(mul(temp, (float3x3)g_mWorld)); // normal (world space)
    
    // Just copy the texture coordinate through
    Output.Tex0 = input.vTexCoord0;
    Output.Tex1 = dot( -vNormalWorldSpace, g_LightDir );
    return Output;
}

VS_OUTPUT2 RenderNoCartoonVS( VS_SKININPUT input )
{
	VS_OUTPUT2 Output;
    float4 temp;
    
    // Transform the position from object space to homogeneous projection space    
    temp = mul(input.vPos, g_BoneMatrix[input.vBlendIndices - g_PositionBone] ); 
    Output.Position = mul(temp, g_mWorldViewProj);
    
    
    // Just copy the texture coordinate through
    Output.Tex0 = input.vTexCoord0;

    return Output;
}

// 외각선을 그리기 위하여 normal방향으로 fOutLineWide만큼 곱하여 그린다.
VS_OUTPUT_EDGE RenderSkinBackVS( VS_EDGEINPUT input )
{
    VS_OUTPUT_EDGE Output;
    float4 vNormalWorldSpace;
    
    // Transform the position from object space to homogeneous projection space    
    Output.Position = mul( input.vPos, g_BoneMatrix[input.vBlendIndices - g_PositionBone] ); 
    Output.Position = mul( Output.Position, g_mWorld );

    // Transform the normal from object space to world space    
    vNormalWorldSpace = mul(input.vNormal, g_BoneMatrix[input.vBlendIndices - g_PositionBone] );
    vNormalWorldSpace = normalize(mul( vNormalWorldSpace, g_mWorld ));
    
    Output.Position += vNormalWorldSpace * fOutLineWide;
    
    Output.Position = mul(Output.Position, g_mViewProj );
    
    Output.Color = g_vEdgeColor;
    
    /*
    // Transform the position from object space to homogeneous projection space    
    Output.Position = mul(input.vPos, g_BoneMatrix[input.vBlendIndices - g_PositionBone] ); 
    Output.Position = mul(Output.Position, g_mWorldViewProjection);
    
    // Transform the normal from object space to world space    
    vNormalWorldSpace = mul(input.vNormal, g_BoneMatrix[input.vBlendIndices - g_PositionBone] );
    vNormalWorldSpace = normalize(mul(vNormalWorldSpace, (float3x3)g_mWorld)); // normal (world space)
    
    Output.Position = Output.Position * 10.0f + float4( vNormalWorldSpace , 1.0f ) * fOutLineWide;
    */
    
    return Output;
}


//--------------------------------------------------------------------------------------
// Techniques
//--------------------------------------------------------------------------------------

technique CartoonBlackEdge
{
	
	pass p0
    {
		AlphaBlendEnable	= True;
        SrcBlend			= One;
        DestBlend			= Zero;
        CullMode			= CW; 
        ZEnable				= TRUE;
		ZWriteEnable		= True;
		
        ColorOp[0]			= SelectArg1;
        ColorArg1[0]		= Diffuse;
		
        VertexShader = compile vs_1_1 RenderSkinBackVS();
    }
    
    pass p1
    {
		AlphaBlendEnable = False;
        CullMode        = CCW;        
        ZEnable			= TRUE;
		ZWriteEnable	= True;
        
		// Stage0
		ColorOp[0]		= SelectArg1;
		ColorArg1[0]	= Texture;
		
		// Stage1
        ColorOp[1]      = Modulate;
        ColorArg1[1]    = Current;
        ColorArg2[1]    = Texture;
        AddressU[1]     = Clamp;
		
		VertexShader = compile vs_1_1 RenderCartoonVS();
    }
    
    pass p2
    {
		AlphaBlendEnable	= False;
        CullMode			= CCW;
        ZEnable				= TRUE;
		ZWriteEnable		= True;
		
		// Stage0
		ColorOp[0]			= SelectArg1;
		ColorArg1[0]		= Texture;
		
        VertexShader = compile vs_1_1 RenderNoCartoonVS();
    }
}
//--------------------------------------------------------------------------------------
// Techniques
//--------------------------------------------------------------------------------------
technique RenderState
{
    pass P0
    {   
		AlphaBlendEnable	= True;
    }
    pass p1
    {
		// UI, 2D, Font, 미니맵 렌더링
		CullMode			= NONE;
		AlphaBlendEnable	= True;
    }
    pass p2
    {
		AlphaTestEnable  = True;
		
        ZEnable			= TRUE;
		ZWriteEnable	= True;
    }
    pass p3
    {
		// 최상위 색상 오버레이 기능 사용시...
		AlphaBlendEnable	= True;
		AlphaArg1[0]		= TFactor;
		ColorArg1[0]		= TFactor;	
		//VertexShader = compile vs_1_1 RenderCartoonVS();
    }
    pass p4
    {
		// 맵 렌더링 ( 플레이어 레이어 보다 멀리 있는 녀석들 )
		ZEnable				= True;
		ZWriteEnable		= True;
		AlphaTestEnable		= True;
    }
}
