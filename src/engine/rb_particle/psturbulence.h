//****************************************************************************/
//  File:   PSTurbulence.h
//  Date:   12.07.2006
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __PSTurbulence_H__
#define __PSTurbulence_H__

//****************************************************************************/
//  Class:  PSTurbulence
//  Desc:   Base class for particle system operators
//****************************************************************************/
class PSTurbulence : public PSOperator
{
public: 
    PSTurbulence();

    expose( PSTurbulence )
    {
        parent(PSOperator);
    }
}; // class PSTurbulence

#endif // __PSTurbulence_H__


