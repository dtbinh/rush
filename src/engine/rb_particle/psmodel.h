//****************************************************************************/
//  File:   PSModel.h
//  Date:   12.07.2006
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __PSMODEL_H__
#define __PSMODEL_H__

//****************************************************************************/
//  Class:  PSModel
//  Desc:   Base class for particle system operators
//****************************************************************************/
class PSModel : public PSOperator
{
public: 
    PSModel();

    expose( PSModel )
    {
        parent(PSOperator);
    }
}; // class PSModel

#endif // __PSModel_H__


