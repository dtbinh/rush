//****************************************************************************/
//  File:  JStringServer.h
//****************************************************************************/
#ifndef __JSTRINGSERVER_H__
#define __JSTRINGSERVER_H__
#include "istringserver.h"
#include "jobject.h"
#include "jstring.h"

typedef hash_map_t<JString, JString> JStringDictionary;

//****************************************************************************/
//  Class:  JStringServer
//  Desc:
//****************************************************************************/
class JStringServer : public JObject, public IStringServer
{
public:
                    JStringServer   ();
    virtual         ~JStringServer  ();

    virtual void    Init            ();

    const char*     GetString       ( const char* id ) const;

    void            LoadDictionary  ( const char* pFileName = NULL );
    void            Reset           ();
    void            ExtractStrings  ();

    int             GetNumStrings   () const { return m_Dictionary.size(); }

    expose( JStringServer )
    {
        parent( JObject );
        rprop( "NumStrings", GetNumStrings );
        method( "ExtractStrings", ExtractStrings );
    }
private:

    JStringDictionary       m_Dictionary;

}; // class JStringServer

#endif // __JSTRINGSERVER_H__




