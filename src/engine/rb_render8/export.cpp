#include "stdafx.h"
#include "JReflect.h"

export(rb_render8)
{
    link_class(JRenderServer);
    link_class( Shader );
    link_class( RenderState );
    link_class( TexturePass );
}