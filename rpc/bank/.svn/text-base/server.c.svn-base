#include "account.h"  

int total =  10000;  

int *deposit_1_svc(int *argp, struct svc_req *rqstp){  
    static int result;
    
    total = total + *argp;  
    result = total;  
    printf("new total = %d \n",total);  
    return &result;  
}  

int *withdraw_1_svc(int *argp, struct svc_req *rqstp){  
    static int result;
    
    total = total - *argp;  
    result = total;  
    printf("new total = %d \n",total);  
    return &result;    
}  
