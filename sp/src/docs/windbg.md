# Debugging with WinDBG

- Open Microsoft Store
- Download WinDBG Preview
- Open WinDBG Preview
- Go to File > Launch executable (advanced)
- Enter `<steam installdir>\steamapps\common\Source SDK Base 2013 Singleplayer\hl2.exe` for executable
- Enter `-allowdebug -dev -condebug -console -toconsole -game "<path to sdk>\source-sdk-2013\sp\game\mod_hl2"` for arguments
- Run!

## Setting breakpoints

Well, WinDBG doesn't do the one thing I wanted it to do that VS did: when setting a named breakpoint, set it on every module not only on the first one.
It only set it on engine.dll :(
We can also break on LauncherMain, that's quite early too...also, apparently nobody ever calls launcher!CreateInterface, wtf? The breakpoint seems to be set correctly, but we never land there.

Export of the day: tier0!CrackSmokingCompiler
