/***********************************************************************************/
/*  File:   JPaintPalette.h
/*  Date:   27.05.2006
/*  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JPaintPalette_H__
#define __JPaintPalette_H__

/***********************************************************************************/
/*  Class:  JPaintPalette
/*  Desc:   
/***********************************************************************************/
class JPaintPalette : public JWidget
{
    

public:
                        JPaintPalette  ();
    virtual void        Render      ();
    virtual void        Init        ();

    expose( JPaintPalette )
    {
        parent( JWidget );
    }
}; // class JPaintPalette

#endif //__JPaintPalette_H__