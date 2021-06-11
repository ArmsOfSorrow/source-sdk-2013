# Premise

We got the entirety of HL2's client and server code and the means to compile it. It interfaces with engine and all other parts. Cool so far.

But what if we wanted to make a wholly different game that's got nothing to do with HL2? We could do that in theory.

Let's try and see what we run into.

# How is server.dll built?

The .vpc file for building a base server.dll is [here](../game/server/server_base.vpc). Kinda unfortunate, but this does include LOTS of files, one of which is `gameinterface.cpp` which we kinda sorta want to replicate on our own, don't we?  
We could copy-paste much of the build file and get rid of stuff we don't need, hm...

Yeah, let's make a copy of server_base.vpc and strip it down as much as we can, then create a not_so_hl2.vpc on top.

Now we can use vpc to generate our VS2013 project like this: `.\devtools\bin\vpc.exe +not_so_server`

VS 2019 does open it without complaining, but we'll need VS2013 to actually build it because of v120_xp toolset :/

## Minimizing server.dll

I'm using the existing gameinterface.cpp and copy-pasted the vpc file and commented out lots of stuff, now just trying to build gameinterface.cpp and everything it needs, but...that thing is a messy hairball of code, man. It even pulls in AI and physics and all that shit, wtf.

Maybe it's easier to start from zero after all?

## Starting from zero (well, not entirely zero)

Let's try a different way. Empty folder + a copy of gameinterface.h/.cpp. Now let's delete everything and replace it with debug prints to build a really bare-bones server DLL.

This thing is massive. It's probably gonna blow up the first time I try to load it, if it even compiles.  
Maybe making a bare-bones server DLL is too much for now.

Maybe it's easier to copy-paste gameinterface.cpp to back it up, then delete shit from it one by one. Such as AI.

## Deleting shit from gameinterface.cpp

Generating projects can be done with `creategameprojects.bat`. Cool.

Lots of it is gone already. I deleted AI, bunch of other stuff and it's down from 3441 lines to 2243 at the time of writing.  
Lots of global variables in there that are referenced in wholly different files. Man. Such a fucking hairball. But interesting nonetheless.

Some of it we'll never be able to delete (unless we exclude the offending files from the build)...but that's okay. We're learning here :)

So we can start into the main menu now, set breakpoints and all that shit...but map loading is broken because we removed scenefilecache and some other part (sceneentity.cpp) unconditionally expects it to be there. smh.  
Do we want to make that work again? Or do we go ahead and delete even more shit?

Neither Msg() nor DevMsg() seem to end up in the conosle. Do I need to set developer convar for it to work? Wiki says no for Msg(). Maybe it's server vs client console?
DLLInit Msg() shows up in debug output. Maybe it uses OutputDebugString? Yes it does. OutputDebugStringA it is.