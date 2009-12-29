//****************************************************************************/
//  File:   PSLightning.h
//  Date:   12.07.2006
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __PSLightning_H__
#define __PSLightning_H__

//****************************************************************************/
//  Class:  PSLightning
//  Desc:   Base class for particle system operators
//****************************************************************************/
class PSLightning : public PSOperator
{
public: 
    PSLightning();

    expose( PSLightning )
    {
        parent(PSOperator);
    }
}; // class PSLightning

#endif // __PSLightning_H__


