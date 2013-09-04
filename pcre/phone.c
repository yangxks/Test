/**************   PCRE接口介绍 *******************************
（1）. pcre_compile
 
pcre *pcre_compile(const char *pattern, int options,
            const char **errptr, int *erroffset,
            const unsigned char *tableptr);
功能：编译指定的正则表达式
参数：pattern, 输入参数，将要被编译的字符串形式的正则表达式
      options, 输入参数，用来指定编译时的一些选项
      errptr, 输出参数，用来输出错误信息
      erroffset, 输出参数，pattern中出错位置的偏移量
      tableptr, 输入参数，用来指定字符表，一般情况用NULL, 使用缺省的字符表
返回值：被编译好的正则表达式的pcre内部表示结构
 
（2）. pcre_exec
 
int pcre_exec(const pcre *code, const pcre_extra *extra,
            const char *subject, int length, int startoffset,
            int options, int *ovector, int ovecsize);
功能：用来检查某个字符串是否与指定的正则表达式匹配
参数： code, 输入参数，用pcre_compile编译好的正则表达结构的指针
      extra, 输入参数，用来向pcre_exec传一些额外的数据信息的结构的指针
      subject, 输入参数，要被用来匹配的字符串
      length, 输入参数， 要被用来匹配的字符串的指针
      startoffset, 输入参数，用来指定subject从什么位置开始被匹配的偏移量
      options, 输入参数， 用来指定匹配过程中的一些选项
      ovector, 输出参数，用来返回匹配位置偏移量的数组
      ovecsize, 输入参数， 用来返回匹配位置偏移量的数组的最大大小
返回值：匹配成功返回非负数，没有匹配返回负数
**********************************************************/
#include <pcre.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OVECCOUNT 30    /* should be a multiple of 3 */
#define EBUFLEN 128            
#define BUFLEN 1024

int main()
{              
    pcre *reCM, *reUN, *reTC, *reCDMA; 
    const char *error;
    int erroffset;
    int ovector[OVECCOUNT];
    int rcCM, rcUN, rcTC, rcCDMA; 
/*        
          常用号段前三位
          中国移动：134.135.136.137.138.139.150.151.152.157.158.159.187.188 ,147(数据卡)
          中国联通：130.131.132.155.156.185.186 
          中国电信：133.153.180.189 
          CDMA ：133,153
*/
    char src[22];  
    char pattern_CM[] = "^1(3[4-9]|5[012789]|8[78])\\d{8}$";
    char pattern_UN[] = "^1(3[0-2]|5[56]|8[56])\\d{8}$";
    char pattern_TC[] = "^18[09]\\d{8}$";
    char pattern_CDMA[] = "^1[35]3\\d{8}$";

    printf("please input your telephone number \n");
    scanf("%s", src);
    printf("String : %s\n", src);
    printf("Pattern_CM: \"%s\"\n", pattern_CM);
    printf("Pattern_UN: \"%s\"\n", pattern_UN);
    printf("Pattern_TC: \"%s\"\n", pattern_TC);
    printf("Pattern_CDMA: \"%s\"\n", pattern_CDMA);

    reCM = pcre_compile(pattern_CM, 0, &error, &erroffset, NULL);  //将正则表达式编译成pcre内部表示结构
    reUN = pcre_compile(pattern_UN, 0, &error, &erroffset, NULL);
    reTC = pcre_compile(pattern_TC, 0, &error, &erroffset, NULL);
    reCDMA = pcre_compile(pattern_CDMA, 0, &error, &erroffset, NULL);

    if (reCM==NULL && reUN==NULL && reTC==NULL && reCDMA==NULL) {
        printf("PCRE compilation telephone failed at offset %d: %s\n", erroffset,  error);
        return 1;        
    }

    rcCM = pcre_exec(reCM, NULL, src, strlen(src), 0, 0, ovector, OVECCOUNT);  //匹配pcre编译好的模式，成功返回正数，失败返回负数
    rcUN = pcre_exec(reUN, NULL, src, strlen(src), 0, 0, ovector, OVECCOUNT);
    rcTC = pcre_exec(reTC, NULL, src, strlen(src), 0, 0, ovector, OVECCOUNT);
    rcCDMA = pcre_exec(reCDMA, NULL, src, strlen(src), 0, 0, ovector, OVECCOUNT);

    if (rcCM<0 && rcUN<0 && rcTC<0 && rcCDMA<0) {      //若没匹配返回错误信息
        if (rcCM==PCRE_ERROR_NOMATCH && rcUN==PCRE_ERROR_NOMATCH &&
            rcTC==PCRE_ERROR_NOMATCH && rcTC==PCRE_ERROR_NOMATCH) {
            printf("Sorry, no match ...\n");        
        }

        else {
            printf("Matching error %d\n", rcCM);
            printf("Matching error %d\n", rcUN);
            printf("Matching error %d\n", rcTC);
            printf("Matching error %d\n", rcCDMA);
        }           

        free(reCM);
        free(reUN);
        free(reTC);
        free(reCDMA);          
        return 1;
    }

    printf("\nOK, has matched ...\n\n");

    if (rcCM > 0) {
        printf("Pattern_CM: \"%s\"\n", pattern_CM);
        printf("String : %s\n", src);
    }

    if (rcUN > 0) {
        printf("Pattern_UN: \"%s\"\n", pattern_UN);  
        printf("String : %s\n", src);     
    }

    if (rcTC > 0) {
        printf("Pattern_TC: \"%s\"\n", pattern_TC);
        printf("String : %s\n", src);
    }    

    if (rcCDMA > 0) {
        printf("Pattern_CDMA: \"%s\"\n", pattern_CDMA);   
        printf("String : %s\n", src);
    }
    
    free(reCM);        //释放内存
    free(reUN);
    free(reTC);
    free(reCDMA);        

    return 0;
}
