# Server execution

- engine.dll loads server.dll and calls `CServerGameDLL::DLLInit()`
    - `appSystemFactory` comes from `engine.dll` and is used to get most interfaces that server needs
    - `fileSystemFactory` is an exception (though also comes from `engine.dll`), used to get `VFileSystem022`
    - `physicsFactory` is not used in `DLLInit()`, but is saved and later used in [`CPhysicsHook::Init()`](../game/server/physics.cpp#L165) which is called via `IGameSystem::InitAllSystems()`
- [test link y u only work for absolute file URIs](file:///C:/Users/Krzysztof%20Lesiak/Desktop/devel/source-sdk-2013/sp/src/game/server/gameinterface.cpp#L165)
- [test link y u only work for absolute file URIs](file:game/server/gameinterface.cpp#L200)
- [tesapiofdjf](file:///C:/Users/Krzysztof%20Lesiak/Desktop/devel/source-sdk-2013/sp/src/game/server/gameinterface.cpp#L200)

Man, fuck this. It'd be so neat if vscode could just fucking open relative links with line numbers, but noo. Fuck off.

# Map loading

```
server.dll!CMapLoadEntityFilter::CreateNextEntity(const char * pClassname) Line 184	C++
server.dll!MapEntity_ParseEntity(CBaseEntity * & pEntity, const char * pEntData, IMapEntityFilter * pFilter) Line 565	C++
server.dll!MapEntity_ParseAllEntities(const char * pMapData, IMapEntityFilter * pFilter, bool bActivateEntities) Line 354	C++
server.dll!CServerGameDLL::LevelInit(const char * pMapName, const char * pMapEntities, const char * pOldLevel, const char * pLandmarkName, bool loadGame, bool background)
```

Server creates entities it reads from BSP file. Each entity "class" (not a C++ class) has its own factory and all the factories are in some dictionary.  
It first needs to get the factory by supplying the class name and if it's found, the entity is created.