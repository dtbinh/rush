//****************************************************************************/
//  File:   PSAVelRamp.h
//  Date:   12.07.2006
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __PSAVELRAMP_H__
#define __PSAVELRAMP_H__

//****************************************************************************/
//  Class:  PSAVelRamp
//  Desc:   Base class for particle system operators
//****************************************************************************/
class PSAVelRamp : public PSOperator
{
public: 
    PSAVelRamp();

    expose( PSAVelRamp )
    {
        parent(PSOperator);
    }
}; // class PSAVelRamp

#endif // __PSAVelRamp_H__


