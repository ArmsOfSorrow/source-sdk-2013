# Entities

What the fuck is an entity you might ask?
They're the "things" in a map. Interesting ones and their C++ classes:

- worldspawn -> CWorld
- add more here.

Anyway, clients and servers both have the concept of entities, but they are represented by different C++ classes.  
The server has [CBaseEntity](../game/server/baseentity.h#L344) and the client has [C_BaseEntity](../game/client/c_baseentity.h#L174).  
Both derive from different interfaces (`IServerEntity` vs `IClientEntity`).

## IServerEntity

The doc comment for IServerEntity says:
```cpp
// This class is how the engine talks to entities in the game DLL.
// CBaseEntity implements this interface.
```
So `engine.dll` talks to server entities somehow. I wonder if it does to client entities too. Maybe we'll find out someday.

## IHandleEntity

Both server and client entities implement this interface (via IServerUnknown/IClientUnknown), so both of them can go into entity lists and use `ehandles` (this looks like an old-ass C type from Quake/Goldsrc days), but the interface references `CBaseHandle`. 

## Wait a minute...

CBaseEntity and C_BaseEntity are server and client side concepts/implementations. They have lots of data and are really fucking big, but that's not what engine deals with, right? Engine talks to IServerEntity, doesn't it? That means we could make our own BaseEntity class and if we implement the interface engine want, we could still use that, right? Where's the engine interface? It's in [eiface.h](../public/eiface.h) (there's also [eifacev21.h?!](../public/eifacev21.h)).

So the only mention of `CBaseEntity` is related to something called `CGameTrace`. The rest of the interface seems to work with edicts (entity dictionaries, what are they?).

## CGameTrace

Doc comment says:
```cpp
// Purpose: A trace is returned when a box is swept through the world
```

So some form of collision detection?

Also, interesting comment:
```cpp
// The engine doesn't know what a CBaseEntity is, so it has a backdoor to 
// let it get at the edict
```

`CGameTrace` is part of the public interface between all parts (it's in the public folder). How does it compile for engine though if CBaseEntity is referenced without ifdefs?  
It does that by being forward declared and then only being used as a pointer type -> known size at compile time. I'm not sure if engine needs to define CBaseEntity as something, but even if, that's how this compiles.