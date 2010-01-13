//****************************************************************************/
//  File:  JFileServer.h
//****************************************************************************/
#ifndef __JFILESERVER_H__
#define __JFILESERVER_H__

#include "jreflect.h"
#include "jobject.h"
#include "directorycache.h"
#include "ifileserver.h"

class FInStream;
class JString;
//****************************************************************************/
//  Class:  JFileServer, singleton
//  Desc:
//****************************************************************************/
class JFileServer : public JObject, public IFileServer
{
    DirectoryCache          m_DirCache;

public:
                            JFileServer     ();
    virtual                 ~JFileServer    ();

    virtual bool            OpenMedia       ( const char* name, const char* ext, FInStream& is );
    virtual bool            FindMedia       ( const char* name, const char* ext, JString* path = NULL );
    virtual bool            AddMediaPath    ( const char* path );
    virtual int             GetNMediaPaths  () const;
    virtual const char*     GetMediaPath    ( int idx ) const;

    expose( JFileServer )
    {
        parent( JObject );
    }

    static JFileServer*     s_pInstance;

}; // class JFileServer

#endif // __JFILESERVER_H__


