//****************************************************************************/
//  File:  JMetaClass.h
//****************************************************************************/
#ifndef __JPROPERTYMAP_H__
#define __JPROPERTYMAP_H__

#include "jtypecast.h"
#include "jclassattr.h"
#include "jfieldattr.h"
#include "jpropertyattr.h"
#include "jtagpropertyattr.h"
#include "jtagmethodattr.h"
#include "jmethodattr.h"

class JMetaClass;

//****************************************************************************/
//  Class: JAttrIterator
//****************************************************************************/
class JAttrIterator
{
public:
    inline JAttrIterator( JMetaClass* pClass );
    inline void         operator++      ();
    inline JClassAttr*  operator *      () const;
    inline              operator bool   () const;

private:
    JMetaClass*         m_pClass;
    int                 m_CurAttr;
}; // class JAttrIterator

class JObject;
typedef std::vector<JClassAttr*>    JAttrList;
typedef JObject* (*ObjCreator)();
//****************************************************************************/
//  Class: JMetaClass
//****************************************************************************/
class JMetaClass
{
public:
                    JMetaClass      ();
                    ~JMetaClass     ();
    void            SetName         ( const char* name );
    void            SetParent       ( JMetaClass* pParent );
    const char*     GetName         () const;
    const char*     GetParentName   () const;
    void            SetCreator      ( ObjCreator creator );
    JObject*        Create          () const;
    JObject*        GetTemplate     () const;
    JMetaClass*     GetParent       () const;
    void            SetParent       ( const char* parentName );
    JClassAttr*     FindAttr        ( const char* name );
    int             GetNAttr        () const;
    int             GetNOwnAttr     () const;
    JClassAttr*     GetAttr         ( int idx );

    /*template <class FieldT>
    inline void Field( const char* name, std::vector<FieldT*>& field_arr )
    {
        JClassAttr* pExposed = new JPtrArrayAttr<FieldT>( name, offset, field_arr );
        m_Attr.push_back( pExposed );
    }*/

    template <class FieldT>
    inline void Field( const char* name, FieldT& field, int offset )
    {
        JClassAttr* pExposed = new JFieldAttr<FieldT>( name, offset );
        m_Attr.push_back( pExposed );
    }

    template <class BaseT, class PropT>
    inline void Property( const char* name, PropT (BaseT::*get)() const,
                    void  (BaseT::*set)( PropT val ) = NULL )
    {
        JClassAttr* pExposed = new JPropertyAttr<BaseT, PropT>( name, get, set );
        m_Attr.push_back( pExposed );
    }

    template <class BaseT, class PropT>
    inline void Property( const char* name, PropT (BaseT::*get)(int tag) const,
        void  (BaseT::*set)( int tag, PropT val ) = NULL )
    {
        JClassAttr* pExposed = new JTagPropertyAttr<BaseT, PropT>( name, get, set );
        m_Attr.push_back( pExposed );
    }

    template <class BaseT>
    inline void Method( const char* name, void (BaseT::*call)() )
    {
        JClassAttr* pExposed = new JMethodAttr<BaseT>( name, call );
        m_Attr.push_back( pExposed );
    }

    template <class BaseT>
    inline void Method( const char* name, void (BaseT::*call)(int tag) )
    {
        JClassAttr* pExposed = new JTagMethodAttr<BaseT>( name, call );
        m_Attr.push_back( pExposed );
    }

private:
    JString         m_ParentName;
    JMetaClass*     m_pParent;

    JAttrList       m_Attr;

    JString         m_Name;
    JObject*        m_pTemplate;
    ObjCreator      m_Creator;


    void            Expose          ( JObject* pObject );
}; // class JMetaClass

//****************************************************************************/
//  JMetaClass implementation
//****************************************************************************/
inline JMetaClass::JMetaClass() :
    m_pParent( NULL ),
    m_Creator( NULL ),
    m_pTemplate( NULL )
{}

inline JMetaClass::~JMetaClass()
{
    for (int i = 0; i < m_Attr.size(); i++)
    {
        delete m_Attr[i];
    }
    m_Attr.clear();
}

inline void JMetaClass::SetName( const char* name )
{
    m_Name = name;
}

inline void JMetaClass::SetParent( JMetaClass* pParent )
{
    m_pParent = pParent;
}

inline const char* JMetaClass::GetName() const
{
    return m_Name.c_str();
}

inline const char* JMetaClass::GetParentName() const
{
    return m_ParentName.c_str();
}

inline void JMetaClass::SetCreator( ObjCreator creator )
{
    m_Creator = creator;
    m_pTemplate = creator();
    Expose( m_pTemplate );
}

inline JObject* JMetaClass::Create() const
{
    return m_Creator ? m_Creator() : NULL;
}

inline JObject* JMetaClass::GetTemplate() const
{
    return m_pTemplate;
}

inline JMetaClass* JMetaClass::GetParent() const
{
    return m_pParent;
}

inline void JMetaClass::SetParent( const char* parentName )
{
    if (parentName[0] == 'j' || parentName[0] == 'J')
    {
        parentName++;
    }
    m_ParentName = parentName;
    m_ParentName.to_lower();
}

inline int JMetaClass::GetNAttr() const
{
    int nAttr = m_Attr.size();
    if (m_pParent)
    {
        nAttr += m_pParent->GetNAttr();
    }
    return nAttr;
}

inline int JMetaClass::GetNOwnAttr() const
{
    return m_Attr.size();
}

inline JClassAttr* JMetaClass::GetAttr( int idx )
{
    if (idx < m_Attr.size())
    {
        return m_Attr[idx];
    }
    return m_pParent ? m_pParent->GetAttr( idx - m_Attr.size() ) : NULL;
}

//****************************************************************************/
//  JAttrIterator implementation
//****************************************************************************/
inline JAttrIterator::JAttrIterator( JMetaClass* pClass )
    : m_pClass( pClass )
{
    m_CurAttr = -1;
    operator ++();
}

inline void JAttrIterator::operator++()
{
    m_CurAttr++;
    if (m_CurAttr < m_pClass->GetNOwnAttr())
    {
        return;
    }
    m_CurAttr = -1;
    m_pClass = m_pClass->GetParent();
    if (m_pClass)
    {
        m_CurAttr = 0;
        if (m_pClass->GetNOwnAttr() == 0)
        {
            operator ++();
        }
    }
}

inline JClassAttr* JAttrIterator::operator *() const
{
    return m_CurAttr >= 0 ? m_pClass->GetAttr( m_CurAttr ) : NULL;
}

inline JAttrIterator::operator bool() const
{
    return (m_CurAttr >= 0);
}

#endif // __JPROPERTYMAP_H__


