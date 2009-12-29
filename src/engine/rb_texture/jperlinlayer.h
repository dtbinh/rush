/***********************************************************************************/
//  File:   JPerlinLayer.h
//  Date:   10.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JPERLINLAYER_H__
#define __JPERLINLAYER_H__

/***********************************************************************************/
//  Class:  JPerlinLayer
//  Desc:   
/***********************************************************************************/
class JPerlinLayer : public JObject
{
public:
       
    expose( JPerlinLayer )
    {
        parent( JObject );
    }
}; // class JPerlinLayer

#endif //__JPERLINLAYER_H__