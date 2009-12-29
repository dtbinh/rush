//****************************************************************************/
//  File:   PSRender.h
//  Date:   12.07.2006
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __PSRender_H__
#define __PSRender_H__

//****************************************************************************/
//  Class:  PSRender
//  Desc:   Base class for particle system operators
//****************************************************************************/
class PSRender : public PSOperator
{
public: 
    PSRender();

    expose( PSRender )
    {
        parent(PSOperator);
    }
}; // class PSRender

#endif // __PSRender_H__


