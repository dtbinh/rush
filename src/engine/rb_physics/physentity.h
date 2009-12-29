/***********************************************************************************/
//  File:   PhysEntity.h
//  Date:   25.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __PHYSENTITY_H__
#define __PHYSENTITY_H__

/***********************************************************************************/
//  Class:  PhysEntity
//  Desc:   
/***********************************************************************************/
class PhysEntity : public JObject
{
    int             m_ModelInstID;
public:
                    PhysEntity         ();

    virtual void    Synchronize        ( bool bFromSolver ) {}
    void            SetModelInstID     ( int modelInstID );
    int             GetModelInstID     () const;
    virtual void    Reset              () {}
    virtual void    BindToSolver       () {}

    expose( PhysEntity )
    {
        parent( JObject );
        rprop( "ModelInstID", GetModelInstID );
    }

}; // class PhysEntity

/***********************************************************************************/
/*  PhysEntity inlines 
/***********************************************************************************/
inline PhysEntity::PhysEntity() :
    m_ModelInstID( -1 )
{
}

inline int PhysEntity::GetModelInstID() const
{ 
    return m_ModelInstID; 
}


#endif //__PHYSENTITY_H__