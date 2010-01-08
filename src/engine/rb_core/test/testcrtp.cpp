#include "precompile.h"

#include <cstdio>
#include <typeinfo>
#include <string>

#define GetComponent(T) (*static_cast<T<TObject>*>(static_cast<TObject*>(this)))
#define GetObject() (*static_cast<TObject*>(this))

// forward declaration - если B и C используют друг друга
template<typename TObject>
struct C;

// общий код и/или интерфейс и т.п.
// поиграться с virtual очевидно полезно
struct BBase
{
    /*virtual*/ void fromB2() {}
};

template<typename TObject>
struct B : BBase
{
    void fromB()
    {
        std::printf("%s::fromB()\n", typeid(*this).name());
        GetComponent(C).fromC();
    }
    void fromB2()
    {
        std::printf("%s::fromB2()\n", typeid(*this).name());
    }
};

struct CBase
{
    /*virtual*/ void fromC() {}
};

template<typename TObject>
struct C : CBase
{
    void fromC()
    {
        std::printf("%s::fromC()\n", typeid(*this).name());
        GetComponent(B).fromB2();
        GetObject().fromA();
    }
};

class IComponent
{
    virtual void Init()
    {
    
    }
};

template<typename TObject>
struct Position : public IComponent
{
    float x, y, z;
    bool  m_bScreenSpace;
    
    Position() : x( 1.0f ), y( 2.0f ), z( 3.0f )
    {
    
    }
};

template<typename TObject>
struct Animation : public IComponent
{
    float m_CurTime;
    bool  m_bPlaying;

};

template<typename TObject>
struct ColorTint : public IComponent
{
    int m_Color;
};

template<typename TObject>
struct Attached : public IComponent
{
    std::string     m_ObjName;
    std::string     m_BoneName;

    int Update()
    {
        float x = GetComponent( Position ).x;
        float y = GetComponent( Position ).y;
        float z = GetComponent( Position ).z;
        return x + y + z;
    }
};

struct IResUser : public IComponent
{
    virtual void InitResource()
    {
    
    }
};

template<typename TObject>
struct ResUser : public IResUser
{
    void InitResource()
    {
        
    }
};

template<typename TObject>
struct Sprite : public ResUser<TObject>
{
    int     m_FrameID;
    int     m_PackID;
};

template<typename TObject>
struct Bounded 
{

};

template<typename TObject>
struct Picked 
{

};

struct JSprite :    Position    <JSprite>, 
                    Sprite      <JSprite>, 
                    Animation   <JSprite>, 
                    ColorTint   <JSprite>,
                    Attached    <JSprite>,
                    Bounded     <JSprite>,
                    Picked      <JSprite>
{
    void test()
    {
        Sprite<JSprite>::InitResource();
        Attached<JSprite>::Update();
    }
    void fromA()
    {
    }
};

TEST(CRTP)
{
    JSprite a;
    int t = sizeof( a );
    a.test();
    CHECK_EQUAL( 6, a.Update() );
}