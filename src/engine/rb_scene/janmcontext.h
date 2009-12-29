/***********************************************************************************/
//  File:   JAnmContext.h
//  Date:   27.05.2006
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JANMCONTEXT_H__
#define __JANMCONTEXT_H__

#include "Mat4.h"
/***********************************************************************************/
//  Class:  JAnmContext
//  Desc:   
/***********************************************************************************/
struct JAnmContext
{   
    Mat4                m_TM;
    Vec3                m_Pos;
    Vec3                m_Target;
    float               m_Angle;
    float               m_Scale;
    double              m_Time;
    double              m_TimeDelta;
    bool                m_bStateOnly;
    JString             m_State;


    JAnmContext() : 
        m_Time( 0.0 ), 
        m_TimeDelta( 0.0f ),
        m_State( "" ),
        m_Angle( 0 ), 
        m_Pos( Vec3::null ), 
        m_Target( Vec3::null ),
        m_Scale( 1.0f ), 
        m_bStateOnly( false ),
        m_TM( Mat4::identity )
    {}

    void                Reset()
    {
        m_State         = "";
        m_TM            = Mat4::identity;
        m_Pos           = Vec3::null;
        m_Target        = Vec3::null;
        m_Angle         = 0.0f;
        m_Scale         = 1.0f;
        m_Time          = 0.0f;
        m_TimeDelta     = 0.0f;
        m_bStateOnly    = false;
    }

    void operator = ( const JAnmContext& ctx )
    {
        m_State         = ctx.m_State;     
        m_TM            = ctx.m_TM;        
        m_Pos           = ctx.m_Pos; 
        m_Target        = ctx.m_Target;
        m_Angle         = ctx.m_Angle;     
        m_Scale         = ctx.m_Scale;     
        m_Time          = ctx.m_Time;   
        m_TimeDelta     = ctx.m_TimeDelta;
        m_bStateOnly    = ctx.m_bStateOnly;
    }
}; // class JAnmContext

#endif //__JAnmContext_H__