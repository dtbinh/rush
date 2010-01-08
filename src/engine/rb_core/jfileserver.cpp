//****************************************************************************/
//  File:  JFileServer.cpp
//****************************************************************************/
#include "precompile.h"
#include "Path.h"
#include "FStream.h"
#include "JFileServer.h"

//****************************************************************************/
/*  JFileServer implementation
//****************************************************************************/
decl_class(JFileServer)
JFileServer* JFileServer::s_pInstance = NULL;

JFileServer::JFileServer()
{
    SetName( "file" );
    s_pInstance = this;
    g_pFileServer = this;
} // JFileServer::JFileServer

JFileServer::~JFileServer()
{
} // JFileServer::JFileServer

bool JFileServer::OpenMedia( const char* name, const char* ext, FInStream& is ) 
{
    //  look in directory tree cache
    char realPath[_MAX_PATH];
    Path path( name );
    if (ext && ext[0] != 0)
    {
        path.SetExt( ext );
    }
    if (!m_DirCache.FindFile( path.GetFile(), path.GetExt(), realPath )) return false;
    is.OpenFile( realPath );
    return is;
} // JFileServer::OpenMedia

bool JFileServer::FindMedia( const char* name, const char* ext, JString* path )
{
    char pathBuf[_MAX_PATH];
    sprintf( pathBuf, "%s.%s", name, ext );

    if (!m_DirCache.FindFile( name, ext, pathBuf )) return false;
    if (path)
    {
        *path = pathBuf;
    }
    return true;
} // JFileServer::FindMedia

bool JFileServer::AddMediaPath( const char* path )
{
    if (path == NULL || path[0] == 0)
    {
        return false;
    }
    
    Path mPath( path );
    mPath.Normalize();
    if (!mPath.IsDirectory())
    {
        mPath.SetFileExt( "" );
    }
    m_DirCache.AddRoot( mPath.GetFullPath() );
    return true;
} // JFileServer::AddMediaPath

int JFileServer::GetNMediaPaths() const
{
    return m_DirCache.GetNRoots();
}

const char* JFileServer::GetMediaPath( int idx ) const
{
    return m_DirCache.GetRoot( idx );
}
