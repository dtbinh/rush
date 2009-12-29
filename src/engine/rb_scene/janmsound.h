/***********************************************************************************/
//  File:   JAnmSound.h
//  Date:   27.10.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JANMSOUND_H__
#define __JANMSOUND_H__

/***********************************************************************************/
//  Class:  JAnmSound
//  Desc:   
/***********************************************************************************/
class JAnmSound : public JAnimation
{
public:
                            JAnmSound   ();

    virtual void            Render      ();
    virtual void            OnPlay      ();
    virtual void            OnStop      ();
    virtual void            ResInit     ();

    expose(JAnmSound)
    {
        parent(JAnimation);
        field( "File",      m_File      );
    }

private:
    JString                 m_File;         //  sound file name
    int                     m_SoundID;
}; // class JAnmSound

#endif //__JANMSOUND_H__