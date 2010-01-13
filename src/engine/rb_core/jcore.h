//****************************************************************************/
//  File:  JCore.h
//****************************************************************************/
#ifndef __JCORE_H__
#define __JCORE_H__

#include "jobject.h"

class JMetaClass;
class JObjectServer;
class JWeakRefReg;
class JSignalServer; 
class JPersistServer;
class JFileServer;   

//****************************************************************************/
//  Class:  JCore 
//  Desc:
//****************************************************************************/
class JCore : public JObject
{
public:
                   JCore           ();    
    virtual        ~JCore          ();
    virtual void   Render          ();
    virtual void   Init            ();
    JObject*       AddServer       ( const char* className );
    const char*    GetRootObject   () const { return m_RootObject.c_str(); }
    void           SetRootObject   ( const char* objName );

    void           SaveRoot        ();

    expose(JCore)
    {
        parent(JObject);
        prop( "RootObject", GetRootObject, SetRootObject );
        method( "SaveRoot", SaveRoot );
    }
    
    JObject*            GetServer( const char* name ) const;


    static JCore*       s_pInstance;

private:
    JString             m_RootObject;
}; // class JCore

#endif // __JCORE_H__


