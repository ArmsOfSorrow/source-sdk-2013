# Parts

Source is made of lots of parts. Some are static libs, some are DLLs.

## CreateInterface

Many parts are "glued together" via the function `void* CreateInterface(const char *pName, int *pReturnCode)`.
If you want to break and inspect interface names, create a new bp and set this into watch window: `(char*)(*(int*)(esp+4))`.

Here's the interfaces queried from source DLLs:

- FileSystem_Stdio.dll
    - QueuedLoaderVersion004
    - VFileSystem022
- engine.dll
    - VCvarQuery001
    - VENGINE_LAUNCHER_API_VERSION004
    - VTOOLFRAMEWORKVERSION002
- inputsystem.dll
    - InputSystemVersion001
- MaterialSystem.dll
    - VMaterialSystem080
- datacache.dll
    - VDataCache003
    - MDLCache004
    - VStudioDataCache005
- StudioRender.dll
    - VStudioRender025
- VPhysics.dll
    - VPhysics031
- video_services.dll
    - IVideoServices002
- vguimatsurface.dll
    - VGUI_Surface030
- vgui2.dll
    - VGUI_ivgui008
- sourcevr.dll
    - SourceVirtualReality001
- shaderapidx9.dll
    - VEngineCvar004
    - VProcessUtils001
    - VFileSystem022
    - NetworkSystemVersion001
    - MaterialSystemHardwareConfig012
    - DebugTextureInfo001
    - VBAllocTracker001
    - COLORCORRECTION_VERSION_1
    - VP4001
    - VMDLLIB001
    - ShaderDeviceMgr001
    - ShaderApi030
    - ShaderDevice001
    - ShaderShadow010
    - VMaterialSystem080
    - InputSystemVersion001
    - VGUI_Input005
    - VGUI_Panel009
    - VGUI_Scheme010
    - VGUI_System010
    - VGUI_Localize005
    - MatSystemSurface008
    - VDmeMakeFileUtils001
    - VPhysicsCollision007
    - VSoundEmitter002
    - VTEX_003
    - VGUI_InputInternal001
    - Hammer001
    - VEngineServer021
    - VoiceServer002
    - VEngineServerStringTable001
    - StaticPropMgrServer002
    - VEngineRandom001
    - IEngineSoundServer003
    - SpatialPartition001
    - VModelInfoServer004
    - EngineTraceServer003
    - GAMEEVENTSMANAGER002
    - ServerUploadGameStats001
    - ISERVERPLUGINHELPERS001
    - VSERVERENGINETOOLS001
    - VDebugOverlay003
    - VPhysicsSurfaceProps001
    - IEngineSoundClient003
    - VEngineClient014
    - VEngineVGui001
    - VENGINE_GAMEUIFUNCS_VERSION005
    - VENGINE_MATCHMAKING_VERSION001
    - XboxSystemInterface001
    - EngineClientReplay001
    - VEngineModel016
    - VEngineEffects001
    - EngineTraceClient003
    - VEngineRenderView014
    - VModelInfoClient006
    - VEngineClientStringTable001
    - VEngineShadowMgr002
    - StaticPropMgrClient004
    - IEngineSoundClient003
    - ServerUploadGameStats001
    - VMaterialSystemStub001
    - VMaterialSystem080
    - VPhysicsSurfaceProps001
    - RunGameEngine005
- video_quicktime.dll
    - IVideoSubSystem002
- video_bink.dll
    - IVideoSubSystem002
- crashhandler.dll
    - crashhandler003
- stdshader_dbg.dll
    - ShaderDLL004
- stdshader_dx6.dll
    - ShaderDLL004
- stdshader_dx7.dll
    - ShaderDLL004
- stdshader_dx8.dll
    - ShaderDLL004
- stdshader_dx9.dll
    - ShaderDLL004
- chromehtml.dll
    - ChromeHTML_Controller_001
- unicode.dll
    - VENGINEUNICODE001
- client.dll
    - VClient017
    - ClientVirtualReality001
    - ClientRenderTargets001
    - VClientDllSharedAppSystems001
    - Shadow_IMaterialProxy003
    - ShadowModel_IMaterialProxy003
    - VClientPrediction001
    - VClientEntityList003
    - VCENTERPRINT002
    - ClientLeafSystem002
    - GameClientExports001
    - VProcessUtils001
    - NetworkSystemVersion001
    - VMDLLIB001
    - VP4001
    - VDmeMakeFileUtils001
    - VTEX_003
    - RunGameEngine005
- server.dll
    - ServerGameDLL009
    - ServerGameEnts001
    - ServerGameClients004
    - HLTVDirector001
    - ServerGameTags001
    - VServerDllSharedAppSystems001
    - PluginHelpersCheck001
- SoundEmitterSystem.dll
    - VSoundEmitter002
- scenefilecache.dll
    - SceneFileCache002
- GameUI.dll
    - GameUI011
    - GameConsole004
    - VProcessUtils001
    - NetworkSystemVersion001
    - VP4001
    - VTEX_003
    - RunGameEngine005
- haptics.dll
    - HapticsInterface001
- vaudio_miles.dll
    - VAudio002
- ServerBrowser.dll
    - VGuiModuleServerBrowser001

Keep in mind that these interfaces only get queried from the listed DLL. We don't know yet if they're actually supported.
(Look here for possible way to find out: https://docs.microsoft.com/en-us/windows-hardware/drivers/debugger/javascript-debugger-scripting#conditional-breakpoints-with-javascript)

Anyway, this list isn't even complete and all of these interfaces get queried on the way to the main menu. Oof.
A lot more happens when actually loading a map.

## fgdlib

Wiki entry: https://developer.valvesoftware.com/wiki/FGD

So it has to do with Hammer. But what does it do? It tells Hammer about entities and what they can do according to the wiki.  
Let's look into the library code.

According to the vpc project it only references its own code, plus headers from `utils/common`. Looks relatively self-contained, I guess.
Wait, in the VS project `$BASE` expands to `..\common;..\public;..\public\tier0;..\public\tier1` on my machine. So all of these seem to be considered "base", though I didn't find where this variable is defined in VPC. Maybe it's hardcoded? `SRCDIR` for example is defined in multiple scripts, e.g. `$Macro SRCDIR` but nothing for base. Well, let's go ahead.

Terms we encounter:
- Classes
- KeyValues
- Input/Output
- GameData
- Variables

[MDKeyvalue](../public/fgdlib/wckeyvalues.h) uses `strcpy()` which suggests we might be able to easily crash it by giving it a source string greater than destination can fit. Where is this type used?
A quick search suggests that only vbsp uses fgdlib, at least in the 2013 SDK. It could very well be that some binaries also use it, we just don't know. :)

Following files in `utils/vbsp` include it:
- manifest.cpp
- map.cpp

I wonder if MDKeyvalue is reachable from there. Maybe we can crash the map compiler somehow. AFAIK it takes vmf files as input and those are text. Maybe worth a try.

## raytrace

Used by vrad (map lighting tool). Rad == radiosity. More info: https://en.wikipedia.org/wiki/Radiosity_(computer_graphics)