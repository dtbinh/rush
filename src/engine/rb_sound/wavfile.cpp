//****************************************************************************/
//  File:  WavFile.cpp
//****************************************************************************/
#include "stdafx.h"
#include "SoundFile.h"
#include "WavFile.h"

//****************************************************************************/
/*  WavFile implementation
//****************************************************************************/
decl_class(WavFile);
bool WavFile::LoadHeader()
{
    m_hMMIO = mmioOpen( (LPSTR)m_Path.c_str(), NULL, MMIO_ALLOCBUF | MMIO_READ );
    
    MMCKINFO        ckIn;           //  chunk info for general use
    PCMWAVEFORMAT   pcmWaveFormat;  //  temp PCM structure to load in     

    if (0 != mmioDescend( m_hMMIO, &m_Riff, NULL, 0 ))
    {
        rlog.err( "mmioDescend failed in sound file %s", m_Name.c_str() );
        return false;
    }

    // Check to make sure this is a valid wave file
    if( (m_Riff.ckid != FOURCC_RIFF) ||
        (m_Riff.fccType != mmioFOURCC('W', 'A', 'V', 'E') ) )
    {
        rlog.err( "File %s is not a WAV file.", m_Name.c_str() );
        return false;
    }

    // Search the input file for for the 'fmt ' chunk
    ckIn.ckid = mmioFOURCC('f', 'm', 't', ' ');
    if (0 != mmioDescend( m_hMMIO, &ckIn, &m_Riff, MMIO_FINDCHUNK ))
    {
        rlog.err( "mmioDescend failed in sound file %s", m_Name.c_str() );
        return false;
    }

    if( ckIn.cksize < (LONG) sizeof(PCMWAVEFORMAT) )
    {
        rlog.err( "<fmt > chunk has to be at least as large as <PCMWAVEFORMAT> in sound file %s", m_Name.c_str() );
        return false;
    }

    // Read the 'fmt ' chunk into <pcmWaveFormat>
    if (mmioRead( m_hMMIO, (HPSTR) &pcmWaveFormat, sizeof(pcmWaveFormat)) != sizeof(pcmWaveFormat))
    {
        rlog.err( "Error reading wave data in sound file %s", m_Name.c_str() );
        return false;
    }

    memcpy( &m_Format, &pcmWaveFormat, sizeof(pcmWaveFormat) );
    m_Format.cbSize = 0;

    if (pcmWaveFormat.wf.wFormatTag != WAVE_FORMAT_PCM)
    {
        WORD nExtraBytes = 0L;
        mmioRead( m_hMMIO, (CHAR*)&nExtraBytes, sizeof( WORD ) );
        CHAR* extra = new CHAR[nExtraBytes];
        mmioRead( m_hMMIO, extra, nExtraBytes );
        delete []extra;
    }

    if (0 != mmioAscend( m_hMMIO, &ckIn, 0 ))
    {
        rlog.err( "mmioAscend failed in sound file %s", m_Name.c_str() );
        return false;
    }

    //  reset file
    Reset();

    m_Size = m_Chunk.cksize;
    m_TotalTime = float( m_Size )/float( m_Format.nAvgBytesPerSec );

    return true;
} // WavFile::LoadHeader

 bool WavFile::Reset()
{
    if (-1 == mmioSeek( m_hMMIO, m_Riff.dwDataOffset + sizeof(FOURCC), SEEK_SET ))
    {
        rlog.err( "Error reading sound file %s", m_Name.c_str() );
        return false;
    }
    m_Chunk.ckid = mmioFOURCC('d', 'a', 't', 'a');
    if (0 != mmioDescend( m_hMMIO, &m_Chunk, &m_Riff, MMIO_FINDCHUNK ))
    {
        rlog.err( "No data chink in sound file %s", m_Name.c_str() );
        return false;
    }
    return true;
} // WavFile::Reset

bool WavFile::Close()
{
    mmioClose( m_hMMIO, 0 );
    m_hMMIO = NULL;
    return true;
} // WavFile::Close

int WavFile::ReadData( BYTE* pData, int nBytes )
{
    if (m_hMMIO == NULL || pData == NULL || nBytes == 0) return 0;

    MMIOINFO mmioinfoIn;  
    mmioGetInfo( m_hMMIO, &mmioinfoIn, 0 );
    int nBytesLeft = 0;
    if (nBytes > m_Chunk.cksize) 
    {
        nBytesLeft = nBytes - m_Chunk.cksize;
        nBytes = m_Chunk.cksize;     
    }

    m_Chunk.cksize -= nBytes;
    
    for (int i = 0; i < nBytes; i++)
    {
        //  copy the bytes from the io to the buffer.
        if (mmioinfoIn.pchNext == mmioinfoIn.pchEndRead)
        {
            mmioAdvance( m_hMMIO, &mmioinfoIn, MMIO_READ );
        }
        *(pData + i) = *((BYTE*)mmioinfoIn.pchNext);
        mmioinfoIn.pchNext++;
    }

    //  fill bytes which are left with zeros
    if (nBytesLeft > 0)
    {
        memset( pData + nBytes, 0, nBytesLeft );
    }

    mmioSetInfo( m_hMMIO, &mmioinfoIn, 0 );
    return nBytes;
} // WavFile::ReadData



