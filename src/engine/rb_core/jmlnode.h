//****************************************************************************/
//  File:	JMLNode.h
//  Desc:	
//****************************************************************************/
#ifndef __JMLNODE_H__
#define __JMLNODE_H__

#include "buffer.h"

class InStream;
class OutStream;
//****************************************************************************/
//  Class:	JMLNode
//  Desc:	Single node of the JML script parsed tree
//****************************************************************************/
class JMLNode
{
	JMLNode*				m_pChild;		//  pointer to the first subnode
	JMLNode*				m_pAttr;		//  pointer to the first attribute
	JMLNode*				m_pParent;		//  pointer to the parent tag
	
    bool                    m_bComplete;

    JString				    m_Class;			
    JString				    m_Name;
    JString                 m_Value;

	JMLNode*				m_pNext;		//  pointer to the next sibling node
	static int				s_Indent;		//  indentation counter when writing 

    static Buffer           s_SaveBuffer;
public:
							JMLNode			();
							JMLNode			( InStream& is );
							JMLNode			( char* buf );
							~JMLNode		();


	int						GetNChildren	() const;
	JMLNode*				GetChild		( int idx );

	int						GetNAttr		() const;
    
	JMLNode*				FirstChild	    () { return m_pChild; }
	JMLNode*				FirstAttr	    () { return m_pAttr;  }
	
    const JMLNode*			FirstChild	    () const { return m_pChild; }
    const JMLNode*		    FirstAttr	    () const { return m_pAttr;  }


	JMLNode*				FindChild		( const char* tag ) const;
	JMLNode*				FindAttr		( const char* name ) const;
	JMLNode*				NextSibling	    () { return m_pNext; }
    const JMLNode*			NextSibling	    () const { return m_pNext; }

    bool                    HasClass        ( const char* tag ) { return (stricmp( m_Class.c_str(), tag ) == 0); } 
    bool                    HasName         ( const char* tag ) { return (stricmp( m_Name.c_str(), tag ) == 0); } 

    void                    SetClass        ( const char* cname ) { m_Class = cname; }
    const char*             GetClass        () const { return m_Class.c_str(); }
	
    void                    SetName         ( const char* name ) { m_Name = name; }
    void                    SetValue        ( const char* val  ) { m_Value = val; }

    void                    SetComplete     ( bool bComplete = true ) { m_bComplete = bComplete; }
    bool                    IsComplete      () const { return m_bComplete; }

    const char*             GetName         () const { return m_Name.c_str(); }

	JMLNode*				AddChild		( const char* className, const char* name = NULL );
	JMLNode*				AddChild		( JMLNode* pChild );
	JMLNode*                AddAttr	        ( const char* name );
    void                    AddValue        ( const char* val );
	bool					Read			( InStream& is );
	void					Write			( OutStream& os, int level = 0 );

	operator				bool			(){ return false; }
    const char*             GetValue        () const { return m_Value.c_str(); }

	template <class TVal> bool GetValue( const char* name, TVal& val )
	{
		JMLNode* pChild = FindChild( name );
		if (!pChild) return false;
		return cast( val, pChild->GetValue() );
	}
	
	template <class TVal> bool GetValue( TVal& val )
	{
		return cast( val, GetValue() );
	}

	template <class TVal> bool GetAttr( const char* name, TVal& val )
	{
		JMLNode* pAttr = FindAttr( name );
		if (!pAttr) return false;
		return cast( val, pAttr->GetValue() );
	}
	
protected:
    char*                Write( char* buf, int level );
}; // class JMLNode

#endif // __JMLNode_H__