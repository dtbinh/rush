Technique FFP
{
    Pass P0 
    {
        alphablendenable    = true;
        srcblend            = srcalpha;
        destblend           = invsrcalpha;
        alphatestenable     = true;
        alphafunc           = greaterequal;
        alpharef            = 0x01;
        ditherenable        = true;
        lighting            = false;
        zenable             = false;
        zwriteenable        = false;
        zfunc               = lessequal;
        fillmode            = solid;
        cullmode            = none;
        colorvertex         = false;
        fogenable           = false;
        specularenable      = false;
        
        colorop[0]          = Modulate;
        colorarg1[0]        = Texture;
        colorarg2[0]        = Diffuse;
        
        alphaop[0]          = Modulate;
        alphaarg1[0]        = Texture;
        alphaarg2[0]        = Diffuse;
        
        addressu[0]         = Mirror;
        addressv[0]         = Mirror;
        magfilter[0]        = Point;
        minfilter[0]        = Point;
        mipfilter[0]        = Point;
        texturetransformflags[0]    = Disable;
        texcoordindex[0]            = 0;
        
        colorop[1]          = disable;
        alphaop[1]          = disable; 
        
        VertexShader        = NULL;
        PixelShader         = NULL; 
    }
};
