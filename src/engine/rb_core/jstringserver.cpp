//****************************************************************************/
//  File:  JStringServer.cpp
//****************************************************************************/
#include "precompile.h"
#include "JStringServer.h"

#include "IPersistServer.h"
#include "IFileServer.h"
#include "JCore.h"
#include "JObjectIterator.h"
#include "FStream.h"
#include "Buffer.h"
#include "hash.h"

//****************************************************************************/
/*  JStringServer implementation
//****************************************************************************/
decl_class(JStringServer);
JStringServer::JStringServer()
{
    g_pStringServer = this;
    SetName( "string" );
}

JStringServer::~JStringServer()
{
}

const char* JStringServer::GetString( const char* id ) const
{
    const char* pID = id;
    if (*pID == '#')
    {
        pID++;
    }
    JStringDictionary::const_iterator location = m_Dictionary.find( pID );
    if (location != m_Dictionary.end())
    {
        return (*location).second.c_str();
    }
    return id;
}

void JStringServer::Init()
{
    LoadDictionary( "strings" );
}

const char* c_DividerSymbols = " \t\n\r;,\0";
const char* c_LineBreakSymbols = "\n\r\0";

void JStringServer::LoadDictionary( const char* pFileName )
{
    FInStream is;
    if (!g_pFileServer->OpenMedia( pFileName, "txt", is ))
    {
        return;
    }
    int size = is.GetTotalSize();
    Buffer buf( size );
    is.Read( buf.GetData(), size );
    is.Close();
    const char* pStr = (const char*)buf.GetData();
    while (*pStr)
    {
        pStr += strspn( pStr, c_DividerSymbols );
        int nChar = strcspn( pStr, c_DividerSymbols );
        JString key( pStr, nChar  );
        pStr += nChar;
        pStr += strspn( pStr, c_DividerSymbols );
        nChar = strcspn( pStr, c_LineBreakSymbols );
        JString value( pStr, nChar );
        pStr += nChar;
        pStr += strcspn( pStr, c_LineBreakSymbols );
        m_Dictionary[key] = value;
    }
}

void JStringServer::Reset()
{
    m_Dictionary.clear();
}

void HashString( const char* str, JString& hash )
{
    hash = "XXXX";
    uint32_t nHash = PHHash( (const uint8_t*)str, strlen( str ) );
    const uint8_t* pHash = (const uint8_t*)&nHash;
    for (int i = 0; i < 4; i++)
    {
        hash[i] = 'A' + pHash[i]%('Z' - 'A');
    }
}

bool HasCyrillics( const char* str )
{
    const uint8_t* pStr = (const uint8_t*)str;
    while (*pStr != 0)
    {
        if (*pStr > 0xBF)
        {
            return true;
        }
        pStr++;
    }
    return false;
}

void JStringServer::ExtractStrings()
{
    FILE* fp = fopen( "strings.txt", "wt" );
    if (!fp)
    {
        return;
    }
    m_Dictionary.clear();

    //  extract text strings
    JObjectIterator it( JCore::s_pInstance );
    JString val, hash;
    while (it)
    {
        JObject* pObj = *it;
        if (pObj->HasName( "system" ))
        {
            it.breadth_next();
            continue;
        }
        ++it;
        bool bRes = pObj->GetProperty( "text", val );
        if (!bRes || val.size() == 0 || val[0] == '#')
        {
            continue;
        }
        if (!HasCyrillics( val.c_str() ))
        {
            continue;
        }
        HashString( val.c_str(), hash );
        JStringDictionary::iterator location = m_Dictionary.find( hash );

        JString taggedHash = "#";
        taggedHash += hash;
        pObj->SetProperty( "text", taggedHash.c_str() );

        if (location != m_Dictionary.end())
        {
            if (val == (*location).second)
            {
                continue;
            }
            assert( false );
        }
        fprintf( fp, "%s %s\n", hash.c_str(), val.c_str() );
        m_Dictionary[hash] = val;
    }
    fclose( fp );

    //  save scripts
    g_pPersistServer->SaveScripts();
}
