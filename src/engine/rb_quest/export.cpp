#include "stdafx.h"
#include "JReflect.h"

export(rb_quest)
{
    link_class( JQuestGame          );
    link_class( JQuestSettings      );
    link_class( JQuestState         );
    link_class( JLocation           );
    link_class( JCharacter          );
    link_class( JCharControl        );
    link_class( JHotSpot            );
    link_class( JAreaControl        );
    link_class( JSetLocation        );
    link_class( JMoveChar           );
    link_class( JGlyph              );
    link_class( JSegment            );

    link_class( JRipplePicture      );
    link_class( JSwayPicture        );
}