/***********************************************************************************/
//  File:   JRenderTarget.h
//  Date:   13.05.2006
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JRENDERTARGET_H__
#define __JRENDERTARGET_H__

/***********************************************************************************/
//  Class:  JRenderTarget
//  Desc:   
/***********************************************************************************/
class JRenderTarget : public JObject
{
    float           m_Width;
    float           m_Height;
    uint32_t        m_ClearColor;

    int             m_TexID;

public:
                    JRenderTarget();
    virtual void    Render       ();
    virtual void    PostRender   ();
    virtual void    Init         ();


    expose( JRenderTarget )
    {
        parent(JObject);
        field( "Width",         m_Width         );
        field( "Height",        m_Height        );
        field( "ClearColor",    m_ClearColor    );
    }
protected:
   

private:

}; // class JRenderTarget

#endif //__JRenderTarget_H__