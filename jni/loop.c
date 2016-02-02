#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <errno.h>
#include <signal.h>
#include <sys/uio.h>
#include <netdb.h>
#include "loop.h"

#define CHECK(x) if((x) < 0) { perror(#x); return -1; }

#define HEADER_SYNC "LalkA123"
struct MsgHeader
{
    char sync[8];
    size_t length;
};

int loop(GetData gd)
{
    int sfd, fd;
    struct sockaddr_in addr;

    struct MsgHeader mh = { HEADER_SYNC, 0};
    struct iovec iov[2];
    iov[0].iov_base = &mh;
    iov[0].iov_len = sizeof(struct MsgHeader);

    CHECK(sfd = socket(AF_INET, SOCK_STREAM, 0));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1920);
    addr.sin_addr.s_addr = 0;
    CHECK(bind(sfd, (const struct sockaddr*)&addr, sizeof(addr)));
    CHECK(listen(sfd, 1));

    signal(SIGPIPE, SIG_IGN);

    while(1)
    {
        const void* ptr = NULL;
        int len = 0;

        socklen_t l = sizeof(addr);
        CHECK(fd = accept(sfd, (struct sockaddr*)&addr, &l));
        printf("loop: accept %s\n", gethostbyaddr(&addr, sizeof(addr), AF_INET)->h_name);

        while((len = gd(&ptr)) > 0)
        {
            iov[1].iov_base = (void*)ptr;
            iov[1].iov_len = len;
            mh.length = len;
            if(writev(fd, (const struct iovec*)&iov, 2) < 0)
            {
                if(errno == ECONNRESET || errno == ECONNABORTED || errno == EPIPE)
                    break;
                else
                {
                    perror("write(fd, ptr, len)");
                    return -1;
                }
            }
            static int i = 0;
            printf("loop: frame %i out\n", i++);
        }
        printf("loop: connection closed\n");
        shutdown(fd, SHUT_RDWR);
        close(fd);
        break;
    }
    signal(SIGPIPE, SIG_DFL);
    return 0;
}

int dummy(const void** ptr)
{
    static int i = 0;
    if(++i > 20)
    {
        i = 0;
        return -1;
    }
    *ptr = "hello world\n";
    return 12;
}
/*
int main()
{
    return loop(dummy);
}*/
