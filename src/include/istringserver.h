//****************************************************************************/
//  File:  IStringServer.h
//****************************************************************************/
#ifndef __ISTRINGSERVER_H__
#define __ISTRINGSERVER_H__

//****************************************************************************/
//  Class:  IStringServer 
//  Desc:
//****************************************************************************/
class IStringServer  
{
public:
    virtual             ~IStringServer () {}
    virtual const char* GetString      ( const char* id ) const = 0;

}; // class IStringServer

extern IStringServer* g_pStringServer;

#endif // __ISTRINGSERVER_H__




