#include "stdafx.h"
#include "JReflect.h"

export(rb_logic)
{
    link_class  ( JIfVar                );
    link_class  ( JElse                 );
    link_class  ( JSetVar               );
    link_class  ( JIfProp               );
    link_class  ( JSetProp              );
    link_class  ( JToggleProp           );
    link_class  ( JSwapProp             );
    link_class  ( JVar                  );
    link_class  ( JMove                 );
    link_class  ( JCall                 );
}