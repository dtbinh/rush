//****************************************************************************/
//  File:   PSWind.h
//  Date:   12.07.2006
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __PSWind_H__
#define __PSWind_H__



//****************************************************************************/
//  Class:  PSWind
//  Desc:   Base class for particle system operators
//****************************************************************************/
class PSWind : public PSOperator
{
public: 
    PSWind();

    expose( PSWind )
    {
        parent(PSOperator);
    }
}; // class PSWind

#endif // __PSWind_H__


