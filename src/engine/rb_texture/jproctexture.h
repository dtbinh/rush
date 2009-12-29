/***********************************************************************************/
//  File:   JProcTexture.h
//  Date:   10.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JPROCTEXTURE_H__
#define __JPROCTEXTURE_H__
#include "JReflect.h"
#include "JObject.h"

/***********************************************************************************/
//  Class:  JProcTexture
//  Desc:   
/***********************************************************************************/
class JProcTexture : public JObject
{
public:
       
    expose( JProcTexture )
    {
        parent( JObject );
    }
}; // class JProcTexture

#endif //__JPROCTEXTURE_H__