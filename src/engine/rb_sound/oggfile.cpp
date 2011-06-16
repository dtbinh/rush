//****************************************************************************/
//  File:  OggFile.cpp
//****************************************************************************/
#include "stdafx.h"
#include "SoundFile.h"
#include "OggFile.h"

//****************************************************************************/
/*  OggFile implementation
//****************************************************************************/
decl_class(OggFile);
OggFile::OggFile()
{
    memset( &m_OggFile, 0, sizeof( OggVorbis_File ) );
} // OggFile::OggFile

bool OggFile::LoadHeader()
{
    if (m_Path.size() == 0) return false;
    FILE* fp = fopen( m_Path.c_str(), "rb" );

    if (!fp) 
    {
        fclose( fp );
        return false;
    }
    
    int ovRes = 0;
    ovRes = ov_open( fp, &m_OggFile, NULL, 0 );

    if (ovRes < 0)
    {
        if (ovRes == OV_ENOTVORBIS)
        {
            rlog.err( "File %s is not vorbis file.", m_Path.c_str() );
        }
        else if (ovRes == OV_EVERSION)
        {
            rlog.err( "File %s has invalid version.", m_Path.c_str() );
        }
        else if (ovRes == OV_EBADHEADER)
        {
            rlog.err( "File %s has corrupted ogg header.", m_Path.c_str() );
        }
        else 
        {
            rlog.err( "Unknown error opening ogg vorbis file %s.", m_Path.c_str() );
        }
        fclose( fp );
        return false;
    }

    m_TotalTime         = ov_time_total( &m_OggFile, -1 );
    vorbis_info* pInfo  = ov_info( &m_OggFile, -1 );

    int bps             = 16;
    int blockAlign      = pInfo->channels*bps/8;
    int avgBytesPerSec  = blockAlign*pInfo->rate;

    m_Size              = ov_pcm_total( &m_OggFile, -1 )*blockAlign;

    //  fill in pcm format description 
    m_Format.wFormatTag         = WAVE_FORMAT_PCM;
    m_Format.cbSize             = 0;
    m_Format.nChannels          = pInfo->channels;
    m_Format.nSamplesPerSec     = pInfo->rate;
    m_Format.wBitsPerSample     = bps;
    m_Format.nBlockAlign        = blockAlign;
    m_Format.nAvgBytesPerSec    = avgBytesPerSec;

    
    return true;
} // OggFile::LoadHeader

bool OggFile::Reset()
{
    int ovRes = ov_time_seek( &m_OggFile, 0.0 );
    if (ovRes == 0)
    {
        return true;
    }

    if (ovRes == OV_ENOSEEK)
    {
        rlog.err( "File %s does not have seekable beatstream.", m_Path.c_str() );
    }
    else
    {
        rlog.err( "Could not seek playing position in the file %s.", m_Path.c_str() );
    }
    return false;
} // OggFile::Reset

bool OggFile::Close()
{
    return (ov_clear( &m_OggFile ) == 0);
} // OggFile::Close

const int c_SafeSeg = 4096;
int OggFile::ReadData( BYTE* pData, int nBytes )
{
    int curByte = 0;
    do
    {
        //  there is no guarantee about how much bytes exactly ov_read is going to read.
        //  so we use some "safe segment", which is c_SafeSeg bytes long
        int nToRead = tmin( c_SafeSeg, nBytes );
        long ovRes = ov_read( &m_OggFile, (char*)pData + curByte, nToRead, 0, 2, 1, NULL );
        if (ovRes == 0)
        {
            break;
        }
        if (ovRes == OV_EBADLINK || ovRes == OV_HOLE)
        {
            rlog.err( "Error while playing sound file %s.", m_Path.c_str() );
            return 0;
        }
        curByte += ovRes;
    }
    while (curByte + c_SafeSeg < nBytes);

    //  fill bytes which are left with zeros
    int nBytesLeft = nBytes - curByte;
    assert( nBytesLeft >= 0 );
    if (nBytesLeft > 0)
    {
        memset( pData + curByte, 0, nBytesLeft );
    }

    return curByte;
} // OggFile::ReadData


