//****************************************************************************/
//  File:  CommandLine.h
//  Desc:  
//****************************************************************************/
#ifndef __COMMANDLINE_H__
#define __COMMANDLINE_H__

#include "jstring.h"

//****************************************************************************/
//  Class:  CommandLine
//  Desc:   
//****************************************************************************/
class CommandLine
{
public:
                        CommandLine ( const char* cmdLine );

    inline int          GetNParam   () const;
    inline const char*  GetName     ( int idx ) const;
    inline const char*  GetValue    ( int idx ) const;

    const char*         GetValue    ( const char* name ) const;

private:
    void                Parse       ( const char* cmdLine );

    struct Parameter
    {
        JString     m_Name;
        JString     m_Value;
    };

    std::vector<Parameter>     m_Params;

}; // class CommandLine

//****************************************************************************/
/*	inlines
//****************************************************************************/
inline int CommandLine::GetNParam() const
{
    return m_Params.size();
}

inline const char* CommandLine::GetName( int idx ) const
{
    if (idx < 0 || idx >= m_Params.size())
    {
        return NULL;
    }
    return m_Params[idx].m_Name.c_str();
}

inline const char* CommandLine::GetValue( int idx ) const
{
    if (idx < 0 || idx >= m_Params.size())
    {
        return NULL;
    }
    return m_Params[idx].m_Value.c_str();
}

#endif // __COMMANDLINE_H__


