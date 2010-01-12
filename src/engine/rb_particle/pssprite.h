//****************************************************************************/
//  File:   PSSprite.h
//  Date:   12.07.2006
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __PSSprite_H__
#define __PSSprite_H__

//****************************************************************************/
//  Class:  PSSprite
//  Desc:   Renders particle as sprite
//****************************************************************************/
class PSSprite : public PSOperator
{
public: 
    PSSprite();

    virtual void Update( EmitterInstance* pInst, float dt, uint32_t mode, ParticleServer* ps );
    virtual void Init();

    expose( PSSprite )
    {
        parent(PSOperator);
        field( "Package",       m_Package       );
        field( "Frame",         m_Frame         );
        field( "Additive",      m_bAdditive     );
        field( "Flush",         m_bFlush        );
        field( "ScreenSpace",   m_bScreenSpace  );
    }

private:
    JString         m_Package;
    int             m_PackageID;
    int             m_Frame;
    bool            m_bAdditive;
    bool            m_bFlush;
    bool            m_bScreenSpace;

}; // class PSSprite

#endif // __PSSprite_H__


