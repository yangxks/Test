#include "account.h"   
#include <unistd.h>

void account_1(char *host) {   
    CLIENT *clnt;   
    int *result_1;   
    int deposit_1_arg=10;   
    int *result_2;   
    int withdraw_1_arg=20;
    
    clnt=clnt_create(host, ACCOUNT, ACCOUNT_VER,"tcp");   
    result_1=deposit_1(&deposit_1_arg,clnt);   
    printf("result=%d\n", *result_1);   
    result_2=withdraw_1(&withdraw_1_arg,clnt);   
    printf("result=%d\n", *result_2);   
    clnt_destroy(clnt);
}

int main (int argc, char *argv[]) {   
    char *host;
    
    if (argc < 2) {  printf ("usage:%s server_host\n", argv[0]);exit (1); }   
    host=argv[1];
    
    while(1) {
        account_1(host);
        sleep(2);
    }
    
    return 0;
}   
