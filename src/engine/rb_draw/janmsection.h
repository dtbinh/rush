/***********************************************************************************/
//  File:   JAnmSection.h
//  Date:   27.10.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JANMSECTION_H__
#define __JANMSECTION_H__

/***********************************************************************************/
//  Class:  JAnmSection
//  Desc:   
/***********************************************************************************/
class JAnmSection : public JObject
{
public:
    JAnmSection()
        : m_Start( 0.0f ),
          m_End  ( 0.0f )
    {}

    JAnmSection( const char* id, float start, float end, const char* descr = NULL)
        : m_Start( start ),
          m_End  ( end )
    {
        SetName( id );
        if (descr)
        {
            m_Description = descr;
        }
    }

    expose(JAnmSection)
    {
        parent(JObject);
        field ( "Start",        m_Start         ); 
        field ( "End",          m_End           );
        field ( "Description",  m_Description   );
    }

    float           GetStart        () const { return m_Start; }
    float           GetEnd          () const { return m_End; }
    const char*     GetDescription  () const { return m_Description.c_str(); }

    void            SetStart        ( float val ) { m_Start = val; }
    void            SetEnd          ( float val ) { m_End = val; }
    void            SetDescription  ( const char* desc ) { m_Description = desc; }

private:
    float           m_Start;
    float           m_End;
    JString         m_Description;
}; // class JAnmSection

#endif //__JANMSECTION_H__