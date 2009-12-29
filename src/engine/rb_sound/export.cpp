#include "stdafx.h"
#include "JReflect.h"

export(rb_sound)
{
    link_class(SoundServer);
    link_class(SoundFile);
    link_class(OggFile);
    link_class(WavFile);
}