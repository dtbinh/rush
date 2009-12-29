/*****************************************************************************/
/*	File:  shadow.fx
/*	Desc:  
/*****************************************************************************/

Technique FFP
{
    Pass P0 
    {
        alphablendenable            = false;
        alphatestenable             = true;
        alphafunc                   = greaterequal;
        alpharef                    = 0x01;
        ditherenable                = false;
        lighting                    = false;
        zenable                     = false;
        zwriteenable                = false;
        cullmode                    = none;
        fillmode                    = solid;
        texturefactor               = 0xFFAAAAAA;
        colorvertex                 = false;
        
        colorop[0]                  = SelectArg1;
        colorarg1[0]                = TFactor;
        colorarg2[0]                = Diffuse;
        
        alphaop[0]                  = SelectArg1;
        alphaarg1[0]                = Texture;
        alphaarg2[0]                = Diffuse;
        
        addressu[0]                 = clamp;
        addressv[0]                 = clamp;
        magfilter[0]                = point;
        minfilter[0]                = point;
        mipfilter[0]                = linear;
        
        texturetransformflags[0]    = Count4|Projected;
        texcoordindex[0]            = CameraSpacePosition;
        
        colorop[1]                  = disable;
        alphaop[1]                  = disable; 
    }
};
