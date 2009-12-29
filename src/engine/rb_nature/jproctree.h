/***********************************************************************************/
//  File:   JProcTree.h
//  Date:   10.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JPROCTREE_H__
#define __JPROCTREE_H__

/***********************************************************************************/
//  Class:  JProcTree
//  Desc:   
/***********************************************************************************/
class JProcTree : public JObject
{
public:
       
    expose( JProcTree )
    {
        parent( JObject );
    }
}; // class JProcTree

#endif //__JPROCTREE_H__