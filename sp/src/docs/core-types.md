# Core types

## CUserCmd

Used in client and server DLLs (and probably engine).
https://developer.valvesoftware.com/wiki/Usercmd

## bf_read, bf_write

Used for serializing/delta compressing user commands. Basically a bit buffer it seems at first glance.

# Core files

Some files are everywhere.
One of them is the precompiled header, [`cbase.h`](../game/client/cbase.h). It exists in both `server` and `client` projects, included in `stdafx.cpp`.

[`gameinterface.cpp`](../game/server/gameinterface.cpp) exports functionality to `engine.dll` (and potentially other parts of Source).