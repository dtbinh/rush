/***********************************************************************************/
//  File:   JAnmCheckPoint.cpp
//  Date:   27.10.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "precompile.h"
#include <algorithm>
#include "jupobjectiterator.h"
#include "janimation.h"
#include "janimserver.h"
#include "janmskip.h"
#include "janmcheckpoint.h"

/***********************************************************************************/
/*  JAnmCheckPoint implementation
/***********************************************************************************/
decl_class(JAnmCheckPoint);
JAnmCheckPoint::JAnmCheckPoint()
{
    m_pAnmSkip = NULL;
    SetDuration( 0.01f );
} // JAnmCheckPoint::JAnmCheckPoint

void JAnmCheckPoint::OnPlay()
{
    if (m_pAnmSkip) m_pAnmSkip->StopRewind();

    if (!GetContext().m_bStateOnly)
    {
        Stop();
    }
} // JAnmCheckPoint::OnPlay

void JAnmCheckPoint::Init()
{
    JUpObjectIterator it( this );
    while (it)
    {
        m_pAnmSkip = obj_cast<JAnmSkip>( *it );
        if (m_pAnmSkip) break;
        ++it;
    }
} // JAnmCheckPoint::Init
