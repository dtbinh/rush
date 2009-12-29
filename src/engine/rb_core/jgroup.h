//****************************************************************************/
//  File:  JGroup.h
//****************************************************************************/
#ifndef __JGROUP_H__
#define __JGROUP_H__

#include "JObject.h"

//****************************************************************************/
//  Class:  JGroup 
//  Desc:   Dummy class to represent grouping node
//****************************************************************************/
class JGroup : public JObject
{
public:
    expose(JGroup)
    {
        parent(JObject);
    }
}; // class JGroup

#endif // __JGROUP_H__


