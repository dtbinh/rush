#include "stdafx.h"
#include "JReflect.h"

export(rb_nature)
{
    link_class( JProcTree       );
    link_class( JTreeTrunk      );
    link_class( JTreeFrond      );
    link_class( JTreeLeaf       );

    link_class( JSky            );
    link_class( JSkyPlane       );
    link_class( JCelestialBody  );
    link_class( JTerrain        );
}
