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