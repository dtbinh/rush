/***********************************************************************************/
//  File:   JVideoServer.h
//  Date:   27.10.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JVIDEOSERVER_H__
#define __JVIDEOSERVER_H__

#include "theora/theora.h"
#include "ogg/ogg.h"

//****************************************************************************/
//  Class:  JVideoServer
//  Desc:   
//****************************************************************************/
class JVideoServer : public JObject
{
public:
                    JVideoServer    ();
    virtual         ~JVideoServer   ();

    virtual void    Init            ();

    int             GetVideoID      ( const char* fileName );



    expose(JVideoServer)
    {
        parent(JObject);
    }

    static JVideoServer* s_pInstance;

protected:
}; // class JVideoServer

#endif //  __JVIDEOSERVER_H__