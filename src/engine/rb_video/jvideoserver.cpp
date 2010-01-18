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

    // теперь ищем начало логического потока theora
    bool bStartHeader = true;
    int nHeaderPackets = 0;  // число обработанных пакетов заголовков theora

    do
    {
      if (LoadChunk( m_File, &m_SyncState ) ==0)
      {
        // кончился файл, на данном этапе это ошибка
        assert( "!eof searched, terminate...");
      }

      // ogg_sync_pageout - формирует страницу
      while (ogg_sync_pageout( &m_SyncState, &m_Page ) > 0)
        // 1-больше данных не требуется
        // 0-требуется больше данных для создания страницы
      {

        // что страница сформирована успешно

        // это страница заголовков? если нет, кончаем искать заголовки
        if (ogg_page_bos( &m_Page ) == false)
        {
          // нет, это не страница заголовков
          // значит, страницы заголовков всех логических потоков кончились
          // и начались данные этих потоков
          // таким образом надо переходить к чтению страниц данных

          // закидываем эту страничку в логический видеопоток
          PushPage( &m_Page );
          // PushPage - закидывает страничку
          // в логический поток theora, если
          // совпадает идентификатор логического потока
          // в противном случае страница игнорируется

          // выходим из циклов
          bStartHeader = false;
          break;
        }
        else
        {
          // да, это страница заголовков

          // тестовый логический поток
          ogg_stream_state m_StreamStateTest;
          memset(&m_StreamStateTest, 0x00, sizeof(ogg_stream_state));

          // инициализируем тестовый поток на тот же поток с таким же
          // идентификатором потока, как и у текущей странички
          if(0!= ogg_stream_init(&m_StreamStateTest,ogg_page_serialno(&m_Page)) )
            assert( "!error during ogg_stream_init");
          
          // добавляем страницу в тестовый поток
          if(0!= ogg_stream_pagein(&m_StreamStateTest,&m_Page) )
            assert( "!error during ogg_stream_pagein");

          // декодируем данные из этого тестового потока в пакет
          if( ogg_stream_packetout(&m_StreamStateTest,&m_Packet) ==-1)
            assert( "!error during ogg_stream_packetout");

          // nHeaderPackets - число прочитанных
          // заголовочных ПАКЕТОВ theora (не страниц)
          // по спецификации theora таких пакетов должно быть три
          if(nHeaderPackets==0)
          {
            int dhr = theora_decode_header (&m_Info, &m_Comment, &m_Packet);
            // декодируем заголовок theora

            if(dhr<0)
            {
              // это не заголовок theora
                
              // очищаем структуру тестового потока
              ogg_stream_clear(&m_StreamStateTest);
              //и продолжаем цикл в поисках заголовков theora
            }
            else
            {
              // это заголовок theora!

              // вот таким образом "инициализируем" логический поток theora:
              memcpy(&m_StreamState, &m_StreamStateTest,
                              sizeof(m_StreamStateTest));
              // теперь из этого потока будут всегда сыпаться пакеты theora

              nHeaderPackets++;

              // после того, как мы нашли заголовочную страницу логического потока theora,
              // нам необходимо прочитать все остальные заголовочные страницы
              // других потоков и отбросить их (если таковые, конечно, имеются)
            }
          }
        }
      }

    }
    while (bStartHeader);
 
    // сейчас надо получить еще два пакета заголовков theora (см. её документацию)
    // и можно переходить к потоковому воспроизведению

    while(nHeaderPackets<3)
    {
        int result=ogg_stream_packetout(&m_StreamState,&m_Packet);
        // если функция возвращает нуль, значит не хватает данных для декодирования
        // почему то этого НЕТ в спецификации libogg, или я плохо искал

        if (result < 0)
        {
          // ошибка декодирования, поврежденный поток
          assert( "!error during ogg_stream_packetout");
        }

        if (result > 0)
        {
          // удалось успешно извлечь пакет информации theora

          int result2 = theora_decode_header( &m_Info, &m_Comment, &m_Packet );

          if(result2<0)
          {
            // ошибка декодирования, поврежденный поток
            rlog.err("VIDEO: error during theora_decode_header (corrupt stream)");
          }

          ++nHeaderPackets;
        }

      // эту страничку обработали, надо извлечь новую
      // для этого проверяем буфер чтения, вдруг там осталось что-нить похожее
      // на страничку. Если не осталось, тогда просто читаем эти данные из файла:

      if (ogg_sync_pageout( &m_SyncState, &m_Page ) > 0)
        // ogg_sync_pageout - функция, берет данные из буфера приема ogg
        // и записывает их в ogg_page
      {
        //мы нашли страничку в буфере и...
        PushPage( &m_Page );
        // ...пихаем эти данные в подходящий поток
      }
      else
      {
        // ничего мы в буфере не нашли
        int ret = LoadChunk( m_File, &m_SyncState );
        // надо больше данных! читаем их из файла
        
        if (ret == 0)
        {
          // опять файл кончился!
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
        // выводим в лог информацию о цветовом пространстве
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
    // не хватает данных в логическом потоке theora
    // надо надергать данных из физического потока и затолкать их в логический поток

    // читаем данные из файла
    int ret = LoadChunk( m_File, &m_SyncState );
    if (ret == 0)
    {
      // файл кончился, необходимо выполнить закрывающие действия
      // и выйти из приложения
      TheoraClose();
      return;
    }

    while (ogg_sync_pageout( &m_SyncState, &m_Page ) > 0)
      // декодируем данные из буфера в страницы (ogg_page)
      // пока они не кончатся в буфере
    {
      // пихаем эти страницы в соотв. логические потоки
      PushPage( &m_Page );
    }
  }


   
  // удачно декодировали. в пакете содержится декодированная ogg-информация
  // (то бишь закодированная theora-информация)

  // загружаем пакет в декодер theora
  if (theora_decode_packetin(&m_State,&m_Packet) == OC_BADPACKET)
  {
    // ошибка декодирования
      rlog.err( "error during theora_decode_packetin..." );
  }

  // все данные получены, готовим кадр

  // декодируем страничку в YUV-виде в спец. структуру yuv_buffer
  if (theora_decode_YUVout( &m_State, &m_YUVBuffer ) != 0)
  {
    // ошибка декодирования
    rlog.err( "error during theora_decode_YUVout...");
  }
    
    // если это первый кадр, то создаем буфер кадра
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