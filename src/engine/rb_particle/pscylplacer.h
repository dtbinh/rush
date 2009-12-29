//****************************************************************************/
//  File:   PSCylPlacer.h
//  Date:   12.07.2006
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __PSCylPlacer_H__
#define __PSCylPlacer_H__

//****************************************************************************/
//  Class:  PSCylPlacer
//  Desc:   Base class for particle system operators
//****************************************************************************/
class PSCylPlacer : public PSOperator
{
public: 
    PSCylPlacer();

    expose( PSCylPlacer )
    {
        parent(PSOperator);
    }
}; // class PSCylPlacer

#endif // __PSCylPlacer_H__


