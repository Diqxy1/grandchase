//--------------------------------------------------------------------------------------
// Techniques
//--------------------------------------------------------------------------------------
technique RenderState
{
	// ���� ����Ʈ
	// http://msdn.microsoft.com/library/default.asp?url=/archive/en-us/directx9_c_summer_03/directx/graphics/reference/effectfilereference/effectfileformat/states.asp
	pass p0
    {
		// ����Ʈ ������
		AlphaBlendEnable	= True;
		AlphaArg2[0]		= TFactor;
		CullMode			= NONE;	
    }
    pass p1
    {
		// �� ������ ( �÷��̾� ���̾� ���� �ָ� �ִ� �༮�� )
		ZEnable				= True;
		ZWriteEnable		= True;
		AlphaTestEnable		= True;
		AlphaFunc			= Greater;
		MagFilter[0]		= Linear;
    }
    pass p2
    {
		// ���� ������
		AlphaBlendEnable	= True;
		CullMode			= NONE;
    }
    pass p3
    {
		// P3M������
		IndexedVertexBlendEnable	= True;
		VertexBlend					= 1WEIGHTS;
    }
    pass p4
    {
		// P3M ���ķ�����
		IndexedVertexBlendEnable	= True;
		VertexBlend					= 1WEIGHTS;
		AlphaBlendEnable			= True;
		AlphaTestEnable				= True;
		AlphaArg1[0]				= Texture;
		AlphaArg2[0]				= TFactor;
    }
    pass p5
    {
		// �� ������ ( �÷��̾� ���̾� ���� ������ �ִ� �༮�� )
		ZEnable				= True;
		ZWriteEnable		= True;
		AlphaTestEnable		= True;
		AlphaBlendEnable	= True;
		AlphaFunc			= Greater;
		MagFilter[0]		= Linear;
    }
}
