//****************************************************************************/
//  File:  JRef.h
//****************************************************************************/
#ifndef __JREF_H__
#define __JREF_H__

//****************************************************************************/
//  Class:  JRef
//  Desc:   Used as reference to some another object
//****************************************************************************/
class JRef : public JObject
{
    JString             m_Object;
    JWeakRef<JObject>   m_pObject;

public:
                            JRef            () : m_pObject( NULL ) {}
    virtual void            Render          ();
    virtual void            Init            ();

    const char*             GetObject       () const { return m_Object.c_str(); }
    void                    SetObject       ( const char* name );
    
    expose(JRef)
    {
        parent(JObject);
        prop( "Object", GetObject, SetObject );
    }
}; // class JRef

#endif // __JREF_H__


