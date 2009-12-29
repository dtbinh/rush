/***********************************************************************************/
/*  File:   JPaintArea.h
/*  Date:   27.05.2006
/*  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JPaintArea_H__
#define __JPaintArea_H__

/***********************************************************************************/
/*  Class:  JPaintArea
/*  Desc:   
/***********************************************************************************/
class JPaintArea : public JButton
{
public:
                        JPaintArea  ();
    virtual void        Render      ();
    virtual void        Init        ();
    DWORD               GetColor    () const { return GetBgColor(); }
    virtual bool        PtIn        ( int px, int py ) const;

    expose( JPaintArea )
    {
        parent( JButton );
    }

private:
    bool                PtInArea   ( int px, int py ) const;
}; // class JPaintArea

#endif //__JPaintArea_H__