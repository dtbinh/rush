/***********************************************************************************/
//  File:   JTexLayer.h
//  Date:   10.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JTEXLAYER_H__
#define __JTEXLAYER_H__

/***********************************************************************************/
//  Class:  JTexLayer
//  Desc:   
/***********************************************************************************/
class JTexLayer : public JObject
{
public:
       
    expose( JTexLayer )
    {
        parent( JObject );
    }
}; // class JTexLayer

#endif //__JTEXLAYER_H__