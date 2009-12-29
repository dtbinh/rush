/***********************************************************************************/
//  File:   TestDriver.h
//  Date:   11.10.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __TESTDRIVER_H__
#define __TESTDRIVER_H__

#include "JDialog.h"

/***********************************************************************************/
//  Class:  TestDriver
//  Desc:   
/***********************************************************************************/
class TestDriver : public JDialog
{
public:
                            TestDriver      ();
    virtual                 ~TestDriver     ();
    virtual void            Init            ();
    virtual void            Render          ();
    virtual void            PostRender      ();

    void                    Exit            ();

    void                    SaveSettings    ();
    void                    LoadSettings    ();
    void                    GatherTests     ();

    expose(TestDriver)
    {
        parent(JDialog); 
        field( "TestNames", m_TestNames );
        field( "ActiveTests", m_ActiveTests );
        method( "Exit", Exit );
        method( "SaveSettings", SaveSettings );
        method( "LoadSettings", LoadSettings );
    }

private:
    JString                 m_ActiveTests;
    JString                 m_ConfigPath;
    JStringList             m_TestNames;
}; // class TestDriver

#endif //__TESTDRIVER_H__