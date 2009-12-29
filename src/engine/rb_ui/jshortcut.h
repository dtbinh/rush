/***********************************************************************************/
//  File:   JShortcut.h
//  Date:   11.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JSHORTCUT_H__
#define __JSHORTCUT_H__

#include <windows.h>

/***********************************************************************************/
/*  Enum:  JXKey
//  Desc:  Enumeration for special keys on the keyboard
/***********************************************************************************/
enum JXKey
{
    nkBACK          = VK_BACK,      
    nkTAB           = VK_TAB,       
    nkCLEAR         = VK_CLEAR,     
    nkRETURN        = VK_RETURN,    
    nkSHIFT         = VK_SHIFT,     
    nkCONTROL       = VK_CONTROL,   
    nkMENU          = VK_MENU,      
    nkPAUSE         = VK_PAUSE,     
    nkCAPITAL       = VK_CAPITAL,   
    nkESCAPE        = VK_ESCAPE,
    nkPRIOR         = VK_PRIOR,   
    nkNEXT          = VK_NEXT,    
    nkEND           = VK_END,     
    nkHOME          = VK_HOME,    
    nkLEFT          = VK_LEFT,    
    nkUP            = VK_UP,      
    nkRIGHT         = VK_RIGHT,   
    nkDOWN          = VK_DOWN,    
    nkSELECT        = VK_SELECT,  
    nkPRINT         = VK_PRINT,   
    nkEXECUTE       = VK_EXECUTE, 
    nkSNAPSHOT      = VK_SNAPSHOT,
    nkINSERT        = VK_INSERT,  
    nkDELETE        = VK_DELETE,  
    nkHELP          = VK_HELP,    
    nkF1            = VK_F1,  
    nkF2            = VK_F2,  
    nkF3            = VK_F3,  
    nkF4            = VK_F4,  
    nkF5            = VK_F5,  
    nkF6            = VK_F6,  
    nkF7            = VK_F7,  
    nkF8            = VK_F8,  
    nkF9            = VK_F9,  
    nkF10           = VK_F10, 
    nkF11           = VK_F11, 
    nkF12           = VK_F12, 
    nkNUMLOCK       = VK_NUMLOCK,  
    nkSCROLL        = VK_SCROLL,   
}; // enum JXKey

enum_beg(JXKey)
enum_val( "BACK",       nkBACK     ),
enum_val( "TAB",        nkTAB      ),
enum_val( "CLEAR",      nkCLEAR    ),
enum_val( "RETURN",     nkRETURN   ),
enum_val( "SHIFT",      nkSHIFT    ),
enum_val( "CONTROL",    nkCONTROL  ),
enum_val( "MENU",       nkMENU     ),
enum_val( "PAUSE",      nkPAUSE    ),
enum_val( "CAPITAL",    nkCAPITAL  ),
enum_val( "ESCAPE",     nkESCAPE   ),
enum_val( "PRIOR",      nkPRIOR    ),
enum_val( "NEXT",       nkNEXT     ),
enum_val( "END",        nkEND      ),
enum_val( "HOME",       nkHOME     ),
enum_val( "LEFT",       nkLEFT     ),
enum_val( "UP",         nkUP       ),
enum_val( "RIGHT",      nkRIGHT    ),
enum_val( "DOWN",       nkDOWN     ),
enum_val( "SELECT",     nkSELECT   ),
enum_val( "PRINT",      nkPRINT    ),
enum_val( "EXECUTE",    nkEXECUTE  ),
enum_val( "SNAPSHOT",   nkSNAPSHOT ),
enum_val( "INSERT",     nkINSERT   ),
enum_val( "DELETE",     nkDELETE   ),
enum_val( "HELP",       nkHELP     ),
enum_val( "F1",         nkF1       ),
enum_val( "F2",         nkF2       ),
enum_val( "F3",         nkF3       ),
enum_val( "F4",         nkF4       ),
enum_val( "F5",         nkF5       ),
enum_val( "F6",         nkF6       ),
enum_val( "F7",         nkF7       ),
enum_val( "F8",         nkF8       ),
enum_val( "F9",         nkF9       ),
enum_val( "F10",        nkF10      ),
enum_val( "F11",        nkF11      ),
enum_val( "F12",        nkF12      ),
enum_val( "NUMLOCK",    nkNUMLOCK  ),
enum_val( "SCROLL",     nkSCROLL   )
enum_end(JXKey)

/***********************************************************************************/
//  Class:  JShortcut
//  Desc:   Keyboard shortcut processor
/***********************************************************************************/
class JShortcut : public JWidget
{
    DWORD           m_Key;          
    bool            m_bControl;
    bool            m_bAlt;
    bool            m_bShift;

public:
                    JShortcut       ();
    virtual void    OnKey           ( JKeyEvent&   e );
    void            Action          ();
    
    const char*     GetKey() const { return ((const char*)&(m_Key)); }
    void            SetKey( const char* key ) { m_Key = *key; }

    JXKey           GetXKey() const { return JXKey( m_Key ); }
    void            SetXKey( JXKey key ) { m_Key = (DWORD)key; }

    expose( JShortcut )
    {
        parent( JWidget );
        prop  ( "Key",      GetKey,  SetKey  );
        prop  ( "XKey",     GetXKey, SetXKey );
        field ( "Control",  m_bControl      );
        field ( "Alt",      m_bAlt          );
        field ( "Shift",    m_bShift        );
        method( "Action",   Action          );
    }
}; // class JShortcut

#endif //__JSHORTCUT_H__