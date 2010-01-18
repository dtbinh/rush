#include "stdafx.h"
#include "jvideoserver.h"

//****************************************************************************/
/*  JVideoServer implementation
//****************************************************************************/
JVideoServer* JVideoServer::s_pInstance = NULL;
decl_class( JVideoServer );
JVideoServer::JVideoServer()
{
    SetName( "video" );
}  

JVideoServer::~JVideoServer()
{
} 


ogg_stream_state    m_StreamState;
ogg_sync_state      m_SyncState;
ogg_page            m_Page;
ogg_packet          m_Packet;

theora_comment      m_Comment;
theora_info         m_Info;
theora_state        m_State;
yuv_buffer          m_YUVBuffer;

FILE* m_File = NULL;


int LoadChunk( FILE* pFile, ogg_sync_state* pOggSyncState )
{
  const int c_BufferSize = 8192;
  
  // get buffer from ogg
  char* pBuffer = ogg_sync_buffer( pOggSyncState, c_BufferSize );
  long nBytes = fread( pBuffer, 1, c_BufferSize, pFile ); 
  ogg_sync_wrote( pOggSyncState, nBytes );
  
  return nBytes;  
}

 
int PushPage(ogg_page *page)
{
  if (ogg_stream_pagein( &m_StreamState, page ) == -1)
  {
    assert( false );
  }
  return 0;
}

void TheoraClose()
{
  ogg_stream_clear      ( &m_StreamState );
  theora_clear          ( &m_State       );
  theora_comment_clear  ( &m_Comment     );
  theora_info_clear     ( &m_Info        );
  ogg_sync_clear        ( &m_SyncState   );

  fclose( m_File );
}

void JVideoServer::Init()
{return;
    memset( &m_StreamState, 0, sizeof( m_StreamState    ) );
    memset( &m_SyncState,   0, sizeof( m_SyncState      ) );
    memset( &m_Page,        0, sizeof( m_Page           ) );
    memset( &m_Packet,      0, sizeof( m_Packet         ) );
    memset( &m_Comment,     0, sizeof( m_Comment        ) );
    memset( &m_Info,        0, sizeof( m_Info           ) );
    memset( &m_State,       0, sizeof( m_State          ) );
    memset( &m_YUVBuffer,   0, sizeof( m_YUVBuffer      ) );

    ogg_stream_clear( &m_StreamState );
    ogg_sync_init( &m_SyncState );
  
    theora_comment_init( &m_Comment );
    theora_info_init( &m_Info );

    // ������ ���� ������ ����������� ������ theora
    bool bStartHeader = true;
    int nHeaderPackets = 0;  // ����� ������������ ������� ���������� theora

    do
    {
      if (LoadChunk( m_File, &m_SyncState ) ==0)
      {
        // �������� ����, �� ������ ����� ��� ������
        assert( "!eof searched, terminate...");
      }

      // ogg_sync_pageout - ��������� ��������
      while (ogg_sync_pageout( &m_SyncState, &m_Page ) > 0)
        // 1-������ ������ �� ���������
        // 0-��������� ������ ������ ��� �������� ��������
      {

        // ��� �������� ������������ �������

        // ��� �������� ����������? ���� ���, ������� ������ ���������
        if (ogg_page_bos( &m_Page ) == false)
        {
          // ���, ��� �� �������� ����������
          // ������, �������� ���������� ���� ���������� ������� ���������
          // � �������� ������ ���� �������
          // ����� ������� ���� ���������� � ������ ������� ������

          // ���������� ��� ��������� � ���������� ����������
          PushPage( &m_Page );
          // PushPage - ���������� ���������
          // � ���������� ����� theora, ����
          // ��������� ������������� ����������� ������
          // � ��������� ������ �������� ������������

          // ������� �� ������
          bStartHeader = false;
          break;
        }
        else
        {
          // ��, ��� �������� ����������

          // �������� ���������� �����
          ogg_stream_state m_StreamStateTest;
          memset(&m_StreamStateTest, 0x00, sizeof(ogg_stream_state));

          // �������������� �������� ����� �� ��� �� ����� � ����� ��
          // ��������������� ������, ��� � � ������� ���������
          if(0!= ogg_stream_init(&m_StreamStateTest,ogg_page_serialno(&m_Page)) )
            assert( "!error during ogg_stream_init");
          
          // ��������� �������� � �������� �����
          if(0!= ogg_stream_pagein(&m_StreamStateTest,&m_Page) )
            assert( "!error during ogg_stream_pagein");

          // ���������� ������ �� ����� ��������� ������ � �����
          if( ogg_stream_packetout(&m_StreamStateTest,&m_Packet) ==-1)
            assert( "!error during ogg_stream_packetout");

          // nHeaderPackets - ����� �����������
          // ������������ ������� theora (�� �������)
          // �� ������������ theora ����� ������� ������ ���� ���
          if(nHeaderPackets==0)
          {
            int dhr = theora_decode_header (&m_Info, &m_Comment, &m_Packet);
            // ���������� ��������� theora

            if(dhr<0)
            {
              // ��� �� ��������� theora
                
              // ������� ��������� ��������� ������
              ogg_stream_clear(&m_StreamStateTest);
              //� ���������� ���� � ������� ���������� theora
            }
            else
            {
              // ��� ��������� theora!

              // ��� ����� ������� "��������������" ���������� ����� theora:
              memcpy(&m_StreamState, &m_StreamStateTest,
                              sizeof(m_StreamStateTest));
              // ������ �� ����� ������ ����� ������ �������� ������ theora

              nHeaderPackets++;

              // ����� ����, ��� �� ����� ������������ �������� ����������� ������ theora,
              // ��� ���������� ��������� ��� ��������� ������������ ��������
              // ������ ������� � ��������� �� (���� �������, �������, �������)
            }
          }
        }
      }

    }
    while (bStartHeader);
 
    // ������ ���� �������� ��� ��� ������ ���������� theora (��. � ������������)
    // � ����� ���������� � ���������� ���������������

    while(nHeaderPackets<3)
    {
        int result=ogg_stream_packetout(&m_StreamState,&m_Packet);
        // ���� ������� ���������� ����, ������ �� ������� ������ ��� �������������
        // ������ �� ����� ��� � ������������ libogg, ��� � ����� �����

        if (result < 0)
        {
          // ������ �������������, ������������ �����
          assert( "!error during ogg_stream_packetout");
        }

        if (result > 0)
        {
          // ������� ������� ������� ����� ���������� theora

          int result2 = theora_decode_header( &m_Info, &m_Comment, &m_Packet );

          if(result2<0)
          {
            // ������ �������������, ������������ �����
            rlog.err("VIDEO: error during theora_decode_header (corrupt stream)");
          }

          ++nHeaderPackets;
        }

      // ��� ��������� ����������, ���� ������� �����
      // ��� ����� ��������� ����� ������, ����� ��� �������� ���-���� �������
      // �� ���������. ���� �� ��������, ����� ������ ������ ��� ������ �� �����:

      if (ogg_sync_pageout( &m_SyncState, &m_Page ) > 0)
        // ogg_sync_pageout - �������, ����� ������ �� ������ ������ ogg
        // � ���������� �� � ogg_page
      {
        //�� ����� ��������� � ������ �...
        PushPage( &m_Page );
        // ...������ ��� ������ � ���������� �����
      }
      else
      {
        // ������ �� � ������ �� �����
        int ret = LoadChunk( m_File, &m_SyncState );
        // ���� ������ ������! ������ �� �� �����
        
        if (ret == 0)
        {
          // ����� ���� ��������!
          rlog.err("VIDEO: eof searched. terminate...");
        }
      }
    }

    // init videostream
    theora_decode_init( &m_State, &m_Info );

    switch(m_Info.colorspace)
    {
      case OC_CS_UNSPECIFIED:
        // nothing to report
        break;
      case OC_CS_ITU_REC_470M:
        rlog.msg("Encoder specified ITU Rec 470M (NTSC) color.");
        // ������� � ��� ���������� � �������� ������������
        break;
      case OC_CS_ITU_REC_470BG:
        rlog.msg("Encoder specified ITU Rec 470BG (PAL) color.");
        break;
      default:
        rlog.msg("Warning: encoder specified unknown colorspace.");
        break;
    }

  // theora processing...
  while (ogg_stream_packetout( &m_StreamState, &m_Packet ) <= 0)
  {
    // �� ������� ������ � ���������� ������ theora
    // ���� ��������� ������ �� ����������� ������ � ��������� �� � ���������� �����

    // ������ ������ �� �����
    int ret = LoadChunk( m_File, &m_SyncState );
    if (ret == 0)
    {
      // ���� ��������, ���������� ��������� ����������� ��������
      // � ����� �� ����������
      TheoraClose();
      return;
    }

    while (ogg_sync_pageout( &m_SyncState, &m_Page ) > 0)
      // ���������� ������ �� ������ � �������� (ogg_page)
      // ���� ��� �� �������� � ������
    {
      // ������ ��� �������� � �����. ���������� ������
      PushPage( &m_Page );
    }
  }


   
  // ������ ������������. � ������ ���������� �������������� ogg-����������
  // (�� ���� �������������� theora-����������)

  // ��������� ����� � ������� theora
  if (theora_decode_packetin(&m_State,&m_Packet) == OC_BADPACKET)
  {
    // ������ �������������
      rlog.err( "error during theora_decode_packetin..." );
  }

  // ��� ������ ��������, ������� ����

  // ���������� ��������� � YUV-���� � ����. ��������� yuv_buffer
  if (theora_decode_YUVout( &m_State, &m_YUVBuffer ) != 0)
  {
    // ������ �������������
    rlog.err( "error during theora_decode_YUVout...");
  }
    
    // ���� ��� ������ ����, �� ������� ����� �����
    BYTE* frame = new BYTE[m_YUVBuffer.y_height*m_YUVBuffer.y_width*4];

  // yuv to rgb
  for (int cy = 0; cy < m_YUVBuffer.y_height; cy++)
  {
    int nYShift  = m_YUVBuffer.y_stride*cy;
    int nUVShift = m_YUVBuffer.uv_stride*(cy >> 1);
    
    for (int cx = 0; cx < m_YUVBuffer.y_width; cx++)
    {
      int nHX = (cx >> 1);

      BYTE nY = *(BYTE*)(m_YUVBuffer.y + nYShift  + cx  );
      BYTE nU = *(BYTE*)(m_YUVBuffer.u + nUVShift + nHX );
      BYTE nV = *(BYTE*)(m_YUVBuffer.v + nUVShift + nHX );

      int index = (cy*m_YUVBuffer.y_width + cx)*4;

      float r = nY + 1.371f*(nV - 128);
      float g = nY - 0.698f*(nV - 128) - 0.336f*(nU - 128);
      float b = nY + 1.732f*(nU - 128);

      frame[index + 0] = (BYTE)clamp( r, 0.0f, 255.0f );
      frame[index + 1] = (BYTE)clamp( g, 0.0f, 255.0f );
      frame[index + 2] = (BYTE)clamp( b, 0.0f, 255.0f );
      frame[index + 3] = 255;
    }
  }
} // JVideoServer::Init

int JVideoServer::GetVideoID( const char* fileName )
{
    return -1;
}