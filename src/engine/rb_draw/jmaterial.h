/***********************************************************************************/
//  File:   JMaterial.h
//  Date:   10.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JMATERIAL_H__
#define __JMATERIAL_H__

/***********************************************************************************/
//  Class:  JMaterial
//  Desc:   
/***********************************************************************************/
class JMaterial : public JObject
{
    JString             m_ShaderName;

    JString             m_DiffuseMap;
    JString             m_DetailMap;
    JString             m_Diffuse2Map;
    JString             m_NormalMap;
    JString             m_EnvMap;

    DWORD               m_Diffuse;
    DWORD               m_Specular;
    float               m_SpecularPower;
    float               m_Transparency;
    bool                m_bHasAlpha;

    int                 m_ShaderID;
    int                 m_DiffuseMapID;
    int                 m_DetailMapID;
    int                 m_Diffuse2MapID;
    int                 m_NormalMapID;
    int                 m_EnvMapID;

public:
                        JMaterial        ();

    void                SetDiffuseMap    ( const char* mapName )    { if (mapName) m_DiffuseMap = mapName; }
    void                SetEnvMap        ( const char* mapName )    { if (mapName) m_EnvMap = mapName; }
    void                SetShaderName    ( const char* name )       { if (name) m_ShaderName = name; }
    const char*         GetShaderName    () const                   { return m_ShaderName.c_str(); }
    void                SetDiffuse       ( DWORD color )            { m_Diffuse = color; }
    void                SetSpecular      ( DWORD color )            { m_Specular = color; }
    void                SetSpecularPower ( float power )            { m_SpecularPower = power; }

    int                 GetShaderID      () const { return m_ShaderID; }
    int                 GetDiffuseMapID  () const { return m_DiffuseMapID; }
    int                 GetNormalMapID   () const { return m_NormalMapID; }
    int                 GetEnvMapID      () const { return m_EnvMapID; }

    void                Render           ( bool bIgnoreShader = false );
    virtual void        Init             ();

    bool                IsEqual          ( JMaterial* pMtl ) const;
    

    expose( JMaterial )
    {
        parent( JObject );
        field( "Shader",        m_ShaderName    );
        field( "DiffuseMap",    m_DiffuseMap    );
        field( "DetailMap",     m_DetailMap     );
        field( "Diffuse2Map",   m_Diffuse2Map   );
        field( "NormalMap",     m_NormalMap     );
        field( "EnvMap",        m_EnvMap        );
        field( "DiffuseColor",  m_Diffuse       );
        field( "SpecularColor", m_Specular      );
        field( "SpecularPower", m_SpecularPower );
        field( "Transparency",  m_Transparency  );
        field( "HasAlpha",      m_bHasAlpha     );
    }
}; // class JMaterial

#endif //__JMATERIAL_H__