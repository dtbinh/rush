#include "precompile.h"
#include "jreflect.h"

export(rb_scene)
{
    link_class  ( JAnimServer           );
    link_class  ( JCamera               );
    link_class  ( JModelInstance        );
    link_class  ( JAnimInstance         );
    link_class  ( JEffectInstance       );
    link_class  ( JGridPlane                 );

    link_class  ( JLight                );
    link_class  ( JAmbient              );
    link_class  ( JPointLight           );
    link_class  ( JDirLight             );
    link_class  ( JUVScroll             );

    link_class  ( JAnimation            );
    link_class  ( JAnmSkip              );
    link_class  ( JAnmCheckPoint        );
    link_class  ( JAnmSound             );
    link_class  ( JAnmSprite            );

    link_class  ( JFade                 );
    link_class  ( JScript               );
    link_class  ( JScriptEvent          );
    link_class  ( JLocator              );
    link_class  ( JSensor               );
    link_class  ( JShadowPass           );
    link_class  ( JRenderTarget         );

    link_class  ( JCameraController     );
    link_class  ( JEditCamera           );
    link_class  ( JCharCamera           );

    link_class  ( JCursor               );
    link_class  ( JPath                 );
}
