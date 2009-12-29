/***********************************************************************************/
//  File:   ExpFace.h
//  Desc:   
//  Date:   13.08.2006
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __EXPFACE_H__
#define __EXPFACE_H__

//****************************************************************************/
//  Class:  ExpFace
//  Desc:    
//****************************************************************************/
struct ExpFace
{
    ExpVertex*      pV0;
    ExpVertex*      pV1;
    ExpVertex*      pV2;


    ExpFace() : pV0( NULL ), pV1( NULL ), pV2( NULL ) {}
    ExpFace( ExpVertex* a, ExpVertex* b, ExpVertex* c ) : pV0(a), pV1(b), pV2(c) {}

    bool operator <( const ExpFace& face ) const
    {
        return (pV0->index < face.pV0->index);
    }

}; // struct ExpFace

#endif // __EXPFACE_H__
