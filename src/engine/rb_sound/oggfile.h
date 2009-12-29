//****************************************************************************/
//  File:  OggFile.h
//****************************************************************************/
#ifndef __OGGFILE_H__
#define __OGGFILE_H__

#include "vorbis/vorbisfile.h"

//****************************************************************************/
//  Class:  OggFile
//  Desc:   
//****************************************************************************/
class OggFile : public SoundFile
{
    OggVorbis_File      m_OggFile;

public:
                        OggFile     ();

    expose(OggFile)
    {
        parent(SoundFile);
    }

protected:
    virtual bool        LoadHeader  ();
    virtual bool        Reset       ();
    virtual bool        Close       ();
    virtual int         ReadData    ( BYTE* pData, int nBytes );
}; // class OggFile


#endif // __OGGFILE_H__


