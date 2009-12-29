/***********************************************************************************/
//  File:   JTerrain.h
//  Date:   10.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JTERRAIN_H__
#define __JTERRAIN_H__

/***********************************************************************************/
//  Class:  JTerrain
//  Desc:   
/***********************************************************************************/
class JTerrain : public JObject
{
    std::string     m_HeightFile;
    int             m_TileSide;
    BYTE*           m_pHeight;
    AABox           m_AABB;


public:
                    JTerrain();
    virtual void    Init    ();
    virtual void    Render  ();

    expose( JTerrain )
    {
        parent( JObject );
    }
}; // class JTerrain

#endif //__JTERRAIN_H__