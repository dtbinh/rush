//****************************************************************************/
//  File:  VertexType.h
//****************************************************************************/
#ifndef __VERTEXTYPE_H__
#define __VERTEXTYPE_H__

//****************************************************************************/
//  Enum:   VertexType
//  Desc:   Stock common vertex declaration types
//****************************************************************************/
enum VertexType
{
    VertexType_Unknown      = 0,
    VertexType_XYZDUV2      = 1,
    VertexType_XYZWDSUV     = 2,
    VertexType_XYZDUV       = 3,
    VertexType_XYZNUV       = 4,
    VertexType_XYZUV        = 5,
    VertexType_XYZD         = 6,
    VertexType_XYZDNUV      = 7,
    VertexType_XYZNUVW4     = 8,

    VertexType_Last         = 9
}; // enum VertexType

//****************************************************************************/
//  Class:  VertexW
//  Desc:   Common use world-space vertex type
//****************************************************************************/
class VertexW
{
public:
    float                   x;
    float                   y;
    float                   z;

    uint32_t                    diffuse;
    float                   u, v;
    float                   u2, v2;

    static const VertexType Type = VertexType_XYZDUV2;
}; // class VertexW

//****************************************************************************/
//  Class:  VertexS
//  Desc:   Common use screen-space vertex type
//****************************************************************************/
class VertexS
{
public:
    float               x, y;
    float               z;
    float               w;

    uint32_t                diffuse;
    uint32_t                specular;
    float               u, v;

    static const VertexType Type = VertexType_XYZWDSUV;
}; // class VertexS

//****************************************************************************/
//  Class:  VertexN
//  Desc:   Common use world-space vertex type
//****************************************************************************/
class VertexN
{
public:
    float               x, y, z;
    float               nx, ny, nz;
    float               u, v;

    static const VertexType Type = VertexType_XYZNUV;
}; // class VertexN

//****************************************************************************/
//  Class:  VertexW4
//  Desc:   4-bones skinned vertex with normal
//****************************************************************************/
class VertexW4
{
public:
    float               x, y, z;
    float               nx, ny, nz;
    float               u, v;
    uint8_t             weights[4];
    uint8_t             boneIdx[4];

    int GetNBones() const
    {
        if (boneIdx[0] == 0xFF) return 0;
        if (boneIdx[1] == 0xFF) return 1;
        if (boneIdx[2] == 0xFF) return 2;
        if (boneIdx[3] == 0xFF) return 3;
        return 4;
    }

    static const VertexType Type = VertexType_XYZNUVW4;
}; // class VertexS

#endif // __VERTEXTYPE_H__


