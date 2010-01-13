#include "precompile.h"
#include "JReflect.h"

export(rb_draw)
{
    link_class ( JSpritePack    )
    link_class ( JSpriteProbe   );
    link_class ( JDrawServer    );
    link_class ( JAnmSection    );

    link_class ( JModel         );  
    link_class ( JModelAnim     ); 
    link_class ( JMaterial      );  
    link_class ( JBone          );  
    link_class ( JMesh          );  
    link_class ( JModelServer   );
}
