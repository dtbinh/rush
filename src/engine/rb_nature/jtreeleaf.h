/***********************************************************************************/
//  File:   JTreeLeaf.h
//  Date:   10.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JTREELEAF_H__
#define __JTREELEAF_H__

/***********************************************************************************/
//  Class:  JTreeLeaf
//  Desc:   
/***********************************************************************************/
class JTreeLeaf : public JObject
{
public:
       
    expose( JTreeLeaf )
    {
        parent( JObject );
    }
}; // class JTreeLeaf

#endif //__JTREELEAF_H__