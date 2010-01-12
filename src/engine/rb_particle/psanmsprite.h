//****************************************************************************/
//  File:   PSAnmSprite.h
//  Date:   12.07.2006
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __PSANMSPRITE_H__
#define __PSANMSPRITE_H__

//****************************************************************************/
//  Class:  PSAnmSprite
//  Desc:   Base class for particle system operators
//****************************************************************************/
class PSAnmSprite : public PSOperator
{
    bool            m_bRndFrameDir;
    bool            m_bRndFirstFrame;
    int             m_StartFrame;
    int             m_NFrames;
    float           m_FPS;
    float           m_FPSVariation;

    JString         m_Package;
    int             m_PackageID;

public: 
    PSAnmSprite();

    expose( PSAnmSprite )
    {
        parent(PSOperator);
    }
}; // class PSAnmSprite

#endif // __PSAnmSprite_H__


