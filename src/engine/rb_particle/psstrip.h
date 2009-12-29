//****************************************************************************/
//  File:   PSStrip.h
//  Date:   12.07.2006
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __PSStrip_H__
#define __PSStrip_H__

//****************************************************************************/
//  Class:  PSStrip
//  Desc:   Base class for particle system operators
//****************************************************************************/
class PSStrip : public PSOperator
{
public: 
    PSStrip();

    expose( PSStrip )
    {
        parent(PSOperator);
    }
}; // class PSStrip

#endif // __PSStrip_H__


