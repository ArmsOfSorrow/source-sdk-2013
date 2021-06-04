# Dependencies (Windows VS 2013)

When we compile `server` DLL (HL2), the compiler gives us a bunch of log files where we can see interesting things.

To follow along, build the server project with Visual Studio 2013 in Debug mode and open the `source-sdk-2013\sp\src\game\server\Debug_mod_hl2\Server (HL2).tlog\CL.read.1.tlog`. This file seems to contain all the files the compiler has read during compilation.

Highlights:

- Visual Studio 12 CRT, e.g. `C:\PROGRAM FILES (X86)\MICROSOFT VISUAL STUDIO 12.0\VC\INCLUDE\ASSERT.H`
- Windows 7 SDK, e.g. `C:\PROGRAM FILES (X86)\MICROSOFT SDKS\WINDOWS\V7.1A\INCLUDE\WINERROR.H`, `C:\PROGRAM FILES (X86)\MICROSOFT SDKS\WINDOWS\V7.1A\INCLUDE\WINDOWS.H`
  - I wonder if we could make it use a newer Windows SDK. Hm...who knows. The last VC toolset that supported XP is v141_xp: https://docs.microsoft.com/en-us/cpp/build/configuring-programs-for-windows-xp. But if all the DLLs in source use this and link it statically it won't work due to ABI breakage between 2012 and 2015.
  - The platform toolset is set up in [this VPC script](../vpc_scripts/source_win32_base.vpc).
    - This references v110 also. Did ABI break between that and v120? If not, how does this work? Usually major versions mean ABI breakage...then again, maybe source doesn't care? Let's look at DLL deps to see if a CRT is there.
    - In `source-sdk-2013\sp\src\game\server\Debug_mod_hl2\Server (HL2).tlog\cl.command.1.tlog` the `/MTd` switch is referenced, which means [static link, debug](https://docs.microsoft.com/en-us/cpp/build/reference/md-mt-ld-use-run-time-library?view=msvc-160#remarks). So they do statically link the CRT. What about other static libs inside the source code, such as `appframework.lib`? Let's look at them with dumpbin. Maybe they don't use the CRT? Maybe that's why they don't need to include versioned libs, like they do with protobuf. Then shouldn't we find CRT references in protobuf? We do with `dumpbin /all 2012\libprotobuf.lib | findstr /i crt`.
    - We also find some CRT strings with `dumpbin /all appframework.lib | findstr /i crt`, so I don't know. This may warrant further investigation.
    - So the static linking CRT option (`/MT`) is referenced in all win32 vpc scripts (for lib, dll and exe targets). This means each component gets its own CRT bits? I guess the linker strips out unneeded stuff when *everything* is built.

## Bonus trivia

A built server.dll exports a bunch of C++ symbols (maybe by accident?):
- `public: virtual void __thiscall CGrenadeAR2::Detonate(void)` ([Jump to code](../game/server/hl2/grenade_ar2.h#l42))
- `public: virtual void __thiscall CGrenadeHomer::Detonate(void)` ([Jump to code](../game/server/hl2/grenade_homer.h))
- `public: virtual void __thiscall CGrenadePathfollower::Detonate(void)` ([Jump to code](../game/server/hl2/grenade_pathfollower.h))

I wonder if somebody ever does `GetProcAddress()` with these. It'd be oddly specific and interesting to find out. Maybe a data breakpoint would help? Or any breakpoint even. Let's try!

### Breaking on exports

- Start debugging with Visual Studio
- Set breakpoint on `CGrenadeAR2::Detonate(void)`
- Open Source Console, enter `impulse 101`
- Switch weapon to SMG and fire alt with right mouse
- Debugger breaks!
  - Call stack reveals that the call comes from server.dll itself

This is the "normal" usage of the Detonate method, but theoretically it could also come from anywhere in the process because it's exported. Some dev tool could even call it if it wanted to. Super weird, but ok :D

Let's try this:
- Set a breakpoint on GetProcAddress
  - We need a condition, but the function is from Windows itself...how do we do this? We're in x86 Windows using __stdcall.
  - It'd be best if we could check its offset from the current stack pointer, but how?
  - So PUSH decrements the stack pointer (ESP)
  - looking for 0x75C56B08 on the stack (it seems to be at ESP+8)
  - how do we dereference shit in VS debugger? -> `*(int*)(esp+8)` gets us the address! Let's try with conditions now! We know that some windows dll looks for InitializeCriticalSectionEx. Let's try that!
  - Ok our expression only gives us the address...we still need to cast to char*. Let's try with a single char first, for symbols starting with I. ASCII I is 73.
  - This seems to work: `*(char*)(*(int*)(esp+8)) == 73`
    - C gibberish is terrible. I'm not even sure how we'd go about comparing strings. Probably fucking around with array subscripts.
    - How do we load a C++ symbol though? It has mangling for sure. The unmangled symbol is `?Detonate@CGrenadeHomer@@UAEXXZ`, so let's look for symbols starting with ASCII '?', which is 63.
    - And this fucks shit up because imports can also be done via ordinals and this way we deref invalid memory. [The docs](https://docs.microsoft.com/en-us/windows/win32/api/libloaderapi/nf-libloaderapi-getprocaddress#parameters) say that ordinal values must be in the low-order word, so if the high-order word is zero don't break at that location...oof. That's for another day.
- Start debugging
