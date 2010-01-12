//****************************************************************************/
//  File:   PSSelector.cpp
//  Date:   12.07.2006
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#include "precompile.h"
#include "particle.h"
#include "emitterinstance.h"
#include "particleserver.h"
#include "psoperator.h"
#include "psemitter.h"
#include "psselector.h"

//****************************************************************************/
/*  PSSelector implementation
//****************************************************************************/
decl_class(PSSelector);
PSSelector::PSSelector()
{
} // PSSelector::PSSelector

PSEmitter* PSSelector::AssignEmitter( EmitterInstance* pInst, Particle* pHostParticle )
{
    int nCh = GetNChildren();
    int nChances = m_Chances.size();

    int cEmitter = 0;
    if (nChances == 0)
    {
        cEmitter = rand()%nCh;
    }
    else
    {
        float val = frand( 0.0f, 1.0f );
        for (int i = 0; i < nChances; i++)
        {
            val -= m_Chances[i];
            if (val <= 0.0f)
            {
                cEmitter = i;
                break;
            }
            cEmitter++;
        }
    }

    return obj_cast<PSEmitter>( GetChild( cEmitter ) );
} // PSSelector::AssignEmitter
