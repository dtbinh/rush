#include "stdafx.h"
#include "JReflect.h"

export(rb_physics)
{
    link_class( PhysicsServer   );
    link_class( PhysEntity      );
    link_class( PhysObject      );
    link_class( PhysBody        );
    link_class( PhysJoint       );
    link_class( PhysMaterial     );
    link_class( ColGeom         );
    link_class( ColSphere       );
    link_class( ColPlane        );
    link_class( ColBox          );
    link_class( ColCapsule      );
    link_class( ColCylinder     );
    link_class( ColMesh         );

    link_class( JAnmPhysics     );
    link_class( JForceControl   );
}
