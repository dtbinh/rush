/***********************************************************************************/
//  File:   JShadowPass.h
//  Date:   11.10.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JSHADOWPASS_H__
#define __JSHADOWPASS_H__

/***********************************************************************************/
//  Class:  JShadowPass
//  Desc:   Tells shadow manager to draw shadows
/***********************************************************************************/
class JShadowPass : public JObject
{
public:
                        JShadowPass   ();
                        ~JShadowPass  ();
    virtual void        Render        ();
    virtual void        Init          ();

    expose(JShadowPass)
    {
        parent(JObject);
    }
}; // class JShadowPass

#endif //__JSHADOWPASS_H__