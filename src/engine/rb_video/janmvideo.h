/***********************************************************************************/
//  File:   JAnmVideo.h
//  Date:   27.10.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JANMVIDEO_H__
#define __JANMVIDEO_H__

class TexRenderer;
/***********************************************************************************/
//  Class:  JAnmVideo
//  Desc:   
/***********************************************************************************/
class JAnmVideo : public JAnimation
{
    JString                 m_File;     
    int                     m_TexID;
    Frame                   m_Ext;

public:
                            JAnmVideo   ();
    virtual                 ~JAnmVideo  ();

    virtual void            Render      ();
    virtual void            OnPlay      ();
    virtual void            OnStop      ();

    expose(JAnmVideo)
    {
        parent(JAnimation);
        field( "File",      m_File      );
    }

private:
    void                    DrawSurface();
}; // class JAnmVideo

#endif //__JAnmVideo_H__