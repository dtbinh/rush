/***********************************************************************************/
//  File:   JTreeFrond.h
//  Date:   10.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JTREEFROND_H__
#define __JTREEFROND_H__

/***********************************************************************************/
//  Class:  JTreeFrond
//  Desc:   
/***********************************************************************************/
class JTreeFrond : public JObject
{
public:
       
    expose( JTreeFrond )
    {
        parent( JObject );
    }
}; // class JTreeFrond

#endif //__JTREEFROND_H__