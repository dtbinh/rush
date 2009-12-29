#ifndef __RBOOT_H__
#define __RBOOT_H__

class CommandLine;
class JObject;
/*****************************************************************************/
/*  Class:  RBoot
/*  Desc:   Engine entry point manager
/*****************************************************************************/
class RBoot
{
public:
            RBoot   ();
    bool    Init    ( const CommandLine& cmd );
    int     Run     ();
    void    Shut    ();

    void    Update  ();

private:
    JObject*        m_pCore;

    void    AddModuleMediaPath();
    void    AddCommonMediaPath();
    void    AddScriptMediaPath( const char* scriptFile );

}; // class RBoot

#endif

