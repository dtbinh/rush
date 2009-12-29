//****************************************************************************/
//  File:  WavFile.h
//****************************************************************************/
#ifndef __WAVFILE_H__
#define __WAVFILE_H__

//****************************************************************************/
//  Class:  WavFile
//  Desc:   
//****************************************************************************/
class WavFile : public SoundFile
{
    HMMIO               m_hMMIO;     //  MM I/O handle for the WAVE
    MMCKINFO            m_Chunk;     //  multimedia RIFF chunk
    MMCKINFO            m_Riff;      //  use in opening a WAVE file
public:

    expose(WavFile)
    {
        parent(SoundFile);

    }

protected:
    virtual bool        LoadHeader  ();
    virtual bool        Reset       ();
    virtual bool        Close       ();
    virtual int         ReadData    ( BYTE* pData, int nBytes );
}; // class WavFile


#endif // __WAVFILE_H__


