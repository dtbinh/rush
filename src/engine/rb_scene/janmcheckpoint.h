/***********************************************************************************/
//  File:   JAnmCheckPoint.h
//  Date:   27.10.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JANMCHECKPOINT_H__
#define __JANMCHECKPOINT_H__

/***********************************************************************************/
//  Class:  JAnmCheckPoint
//  Desc:   
/***********************************************************************************/
class JAnmCheckPoint : public JAnimation
{
    JAnmSkip*       m_pAnmSkip;
public:
                    JAnmCheckPoint    ();
    virtual void    OnPlay            ();
    virtual void    Init              ();

    expose(JAnmCheckPoint)
    {
        parent(JAnimation);

    }
}; // class JAnmCheckPoint

#endif //__JANMCHECKPOINT_H__