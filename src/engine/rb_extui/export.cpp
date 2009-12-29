#include "stdafx.h"
#include "JReflect.h"

export(rb_extui)
{
    link_class  ( JArrow            );
    link_class  ( JTimeLabel        );
    link_class  ( JRatchet          );
    link_class  ( JObjectInspector  );
    link_class  ( JPropertyEditor   );
    link_class  ( JObjectTree       );
    link_class  ( JObjectFrontEnd   );
    link_class  ( JChartSeries      );
    link_class  ( JChart            );
    link_class  ( JBoundsEditor     );
    link_class  ( JVec3Editor       );
    link_class  ( JPolyLineEditor   );
    link_class  ( JFrameEditor      );
    link_class  ( JPivotEditor      );
    link_class  ( JColorBox         );
    link_class  ( JToolTip          );
    link_class  ( JBevel            );
    link_class  ( JHeartBit         );
    link_class  ( JTextureView      );
    link_class  ( JAngleEditor      );
}
