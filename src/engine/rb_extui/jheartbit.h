/***********************************************************************************/
//  File:   JHeartBit.h
//  Date:   02.09.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JHEARTBIT_H__
#define __JHEARTBIT_H__

#include "JWidget.h"

/***********************************************************************************/
//  Class:  JHeartBit
//  Desc:   Draws a graph, tracking some value, which changes in time
/***********************************************************************************/
class JHeartBit : public JWidget
{
public:
                            JHeartBit   ();
    virtual void            Render      ();

    float                   GetLastValue() const;
    void                    SetLastValue( float val );

    expose(JHeartBit)
    {
        parent(JWidget);
        field( "MinVal",    m_MinVal      );
        field( "MaxVal",    m_MaxVal      );
        field( "AutoScale", m_bAutoScale  );
        prop( "Value",     GetLastValue, SetLastValue );
    }

private:
    bool                    m_bAutoScale;
    float                   m_MinVal;
    float                   m_MaxVal;
    std::vector<float>      m_Values;

    int                     GetMaxValuesNum() const;
}; // class JHeartBit

#endif //__JHEARTBIT_H__