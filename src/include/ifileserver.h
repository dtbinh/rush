/*****************************************************************************/
/*	File:  IFileServer.h
/*****************************************************************************/
#ifndef __IFILESERVER_H__
#define __IFILESERVER_H__

class FInStream;
class JString;
/*****************************************************************************/
/*  Class:  IFileServer
/*  Desc:   Interface to the file system
/*****************************************************************************/
class IFileServer
{
public:
    virtual bool            OpenMedia       ( const char* name, const char* ext, FInStream& is ) = 0;
    virtual bool            FindMedia       ( const char* name, const char* ext, JString* path = NULL ) = 0;
    virtual bool            AddMediaPath    ( const char* path ) = 0;
    virtual int             GetNMediaPaths  () const = 0;
    virtual const char*     GetMediaPath    ( int idx ) const = 0;
};

extern IFileServer* g_pFileServer;

#endif // __IFILESERVER_H__


