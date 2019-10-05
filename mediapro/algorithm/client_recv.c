#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <sys/types.h>
#include <sys/socket.h>

int main(){
    int s = socket(PF_INET, SOCK_STREAM, 0);
    if(s=-1){
    printf("socket miss\n");
    exit;
    }
    struct sockadd_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr;
    addr.sin_port = hton(50000);
    int ret = connect(s, (struct sockaddr *)&addr, sizeof(addr));
    if(s=-1){
    printf("connect miss\n");
    exit;
    }
    int n = 10000;
    data = short[n];
    int N;
    n = send(s, data, N, 0);
    shutdown(s, SHUT_WR);
}