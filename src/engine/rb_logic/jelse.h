//****************************************************************************/
//  File:   JElse.h
//  Date:   22.09.2005
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __JELSE_H__
#define __JELSE_H__

//****************************************************************************/
//  Class:  JElse
//  Desc:   Used as "else" in conditional branching block 
//****************************************************************************/
class JElse : public JAnimation
{
public: 
    virtual void    Render      (); 

    expose( JElse )
    {
        parent(JAnimation);
    }
}; // class JElse

#endif // __JELSE_H__


