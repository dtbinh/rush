#include "stdafx.h"
#include "JReflect.h"

export(rb_particle)
{
    link_class(ParticleServer);

    link_class(PSOperator);
    link_class(PSAlphaRamp);
    link_class(PSAnmSprite);
    link_class(PSAttract);
    link_class(PSAVelRamp);
    link_class(PSColorRamp);
    link_class(PSEmitter);
    link_class(PSSelector);
    link_class(PSFlock);
    link_class(PSForce);
    link_class(PSLightning);
    link_class(PSModel);
    link_class(PSPath);
    link_class(PSPath2);
    link_class(PSShot);
    link_class(PSRampShot);

    link_class(PSBoxPlacer);
    link_class(PSModelPlacer);
    link_class(PSSpherePlacer);
    link_class(PSPolyPlacer);
    link_class(PSMaskPlacer);

    link_class(PSSizeRamp);
    link_class(PSSprite);
    link_class(PSStrip);
    link_class(PSText);
    link_class(PSTurbulence);
    link_class(PSVelRamp);
    link_class(PSWind);
    link_class(PSLinePlacer);
    link_class(PSCylPlacer);
    link_class(PSSpiralPlacer);

    link_class(PSMoveRate);
}

