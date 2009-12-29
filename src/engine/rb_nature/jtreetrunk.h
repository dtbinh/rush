/***********************************************************************************/
//  File:   JTreeTrunk.h
//  Date:   10.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JTREETRUNK_H__
#define __JTREETRUNK_H__

/***********************************************************************************/
//  Class:  JTreeTrunk
//  Desc:   
/***********************************************************************************/
class JTreeTrunk : public JObject
{
public:
       
    expose( JTreeTrunk )
    {
        parent( JObject );
    }
}; // class JTreeTrunk

#endif //__JTREETRUNK_H__