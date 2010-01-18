/*****************************************************************************/
/*	File:  shadow_plane.fx
/*	Desc:  
/*****************************************************************************/

Technique FFP
{
    Pass P0 
    {
        alphablendenable        = true;
        srcblend                = zero;
        destblend               = srccolor;
        alphatestenable         = true;
        alphafunc               = greaterequal;
        alpharef                = 0x01;
        ditherenable            = false;
        specularenable          = true;
        lighting                = false;
        zenable                 = true;
        zwriteenable            = false;
        zfunc                   = lessequal;
        cullmode                = none;
        fillmode                = solid;
        texturefactor           = 0xFFFFFFFF;
        
        colorop[0]              = SelectArg1;
        colorarg1[0]            = Texture;
        colorarg2[0]            = Diffuse;
        
        alphaop[0]              = SelectArg1;
        alphaarg1[0]            = TFactor;
        alphaarg2[0]            = Diffuse;
        
        addressu[0]             = clamp;
        addressv[0]             = clamp;
        magfilter[0]            = linear;
        minfilter[0]            = linear;
        mipfilter[0]            = linear;
    }
};
