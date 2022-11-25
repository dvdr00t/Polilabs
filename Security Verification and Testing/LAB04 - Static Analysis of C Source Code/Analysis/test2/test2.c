#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <inttypes.h>
#include <unistd.h>
#include <ctype.h>
#include <syslog.h>

#define RBUFLEN		128
#define	MAXSIZE		138

/* GLOBAL VARIABLES */
char buf[RBUFLEN];		 /* reception buffer */

/* Provides service on the passed socket */
void service(int s)
{
    int	 n;

    for (;;)
    {
        n=read(s, buf, RBUFLEN-1);
        if (n < 0)
        {
            printf("Read error\n");
            close(s);
            printf("Socket %d closed\n", s);
            break;
        }
        else if (n==0)
        {
            printf("Connection closed by party on socket %d\n",s);
            close(s);
            break;
        }
        else
        {
            char local[MAXSIZE];
            char log[MAXSIZE];
            buf[RBUFLEN-1]='\0';
            strcpy(local,"script.sh ");
            strcat(local,buf);
            system(local);
            strncpy(log,local,140);
            syslog(1,"%s",local);
            strncpy(buf,log,MAXSIZE);
            if(write(s, buf, strlen(buf)) != strlen(buf))
              printf("Write error while replying\n");
            else
              printf("Reply sent\n");
        }
    }
}
