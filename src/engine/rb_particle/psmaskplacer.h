//****************************************************************************/
//  File:   PSMaskPlacer.h
//  Date:   12.07.2006
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __PSMASKPLACER_H__
#define __PSMASKPLACER_H__

//****************************************************************************/
//  Class:  PSMaskPlacer
//  Desc:   Base class for particle system operators
//****************************************************************************/
class PSMaskPlacer : public PSOperator
{
public: 
    PSMaskPlacer();

    expose( PSMaskPlacer )
    {
        parent(PSOperator);
    }
}; // class PSMaskPlacer

#endif // __PSMaskPlacer_H__


