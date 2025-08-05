//--------------------------------------------------------------------------------------
// Techniques
//--------------------------------------------------------------------------------------
technique RenderState
{
	// Âü°í »çÀÌÆ®
	// http://msdn.microsoft.com/library/default.asp?url=/archive/en-us/directx9_c_summer_03/directx/graphics/reference/effectfilereference/effectfileformat/states.asp
	pass p0
    {
		// ÀÌÆåÆ® ·»´õ¸µ
		AlphaBlendEnable	= True;
		AlphaArg2[0]		= TFactor;
		CullMode			= NONE;	
    }
    pass p1
    {
		// ¸Ê ·»´õ¸µ ( ÇÃ·¹ÀÌ¾î ·¹ÀÌ¾î º¸´Ù ¸Ö¸® ÀÖ´Â ³à¼®µé )
		ZEnable				= True;
		ZWriteEnable		= True;
		AlphaTestEnable		= True;
		AlphaFunc			= Greater;
		MagFilter[0]		= Linear;
    }
    pass p2
    {
		// ±ËÀû ·»´õ¸µ
		AlphaBlendEnable	= True;
		CullMode			= NONE;
    }
    pass p3
    {
		// P3M·»´õ¸µ
		IndexedVertexBlendEnable	= True;
		VertexBlend					= 1WEIGHTS;
    }
    pass p4
    {
		// P3M ¾ËÆÄ·»´õ¸µ
		IndexedVertexBlendEnable	= True;
		VertexBlend					= 1WEIGHTS;
		AlphaBlendEnable			= True;
		AlphaTestEnable				= True;
		AlphaArg1[0]				= Texture;
		AlphaArg2[0]				= TFactor;
    }
    pass p5
    {
		// ¸Ê ·»´õ¸µ ( ÇÃ·¹ÀÌ¾î ·¹ÀÌ¾î º¸´Ù °¡±îÀÌ ÀÖ´Â ³à¼®µé )
		ZEnable				= True;
		ZWriteEnable		= True;
		AlphaTestEnable		= True;
		AlphaBlendEnable	= True;
		AlphaFunc			= Greater;
		MagFilter[0]		= Linear;
    }
}
