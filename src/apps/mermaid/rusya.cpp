#include "stdafx.h"
#include "rusya.h"
#include "JWidgetServer.h"
#include "IScriptServer.h"
#include "JDialog.h"

void InitWidgetServer();
void InitQuest();
void InitScene();
void InitParticleServer();
void InitRenderServer();
void InitDrawServer();
void InitSoundServer();
void InitVideoServer();

IScriptServer * g_pScriptServer = NULL;

int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow )
{
    JCore* pCore = new JCore();
    if (pCore != JCore::s_pInstance)
    {
        return 1;
    }
    
    InitWidgetServer();
    InitQuest();
    InitScene();
    InitParticleServer();
    InitRenderServer();
    InitDrawServer();
    InitSoundServer();
    InitVideoServer();

    pCore->Init();
    pCore->AddServer( "animserver"      );
    pCore->AddServer( "widgetserver"    );
    pCore->AddServer( "renderserver"    );
    pCore->AddServer( "drawserver"      );
    pCore->AddServer( "particleserver"  );
    pCore->AddServer( "soundserver"     );
    pCore->AddServer( "videoserver"     );

    //  rusya-specific classes
    link_class( JSwimChar       );
    link_class( JPuzzleGame     );
    link_class( JPuzzlePiece    );
    link_class( JMazeControl    );
    link_class( JPaintGame      );
    link_class( JPaintArea      );
    link_class( JBlotGame		);
    link_class( JBlotPiece		);
    link_class( JSlideGame		);
    link_class( JSlidePiece		);
    link_class( JNumber  		);

    JObject* pGame = JObjectServer::inst().CreateFromFile( "RusyaGame" );
    JDialog* pUI   = dynamic_cast<JDialog*>( JObjectServer::inst().CreateFromFile( "RusyaUI" ) );
    JWidgetServer::s_pInstance->AddChild( pGame );    
    JWidgetServer::s_pInstance->AddChild( pUI );
    pGame->InitTree();
    pUI->InitTree();
    pUI->Show();

	int res = JWidgetServer::s_pInstance->RunApplicationLoop();

    pCore->Release();
    return res;
}