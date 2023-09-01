#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<execinfo.h>
#include"alogger.h"
#include<pthread.h>
#include<dirent.h>
#include<zlib.h>


pthread_mutex_t lock;
pthread_mutex_t slock;
struct logger log_s;

//Custom conversion of the time structre to asci time
char* atime(struct tm *timeptr)
{
  static const char mon_name[][4] = {
    "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
  };
  static char result[26];
  sprintf(result, "[%3s/%d/%d %.2d:%.2d:%.2d]",
    mon_name[timeptr->tm_mon],
    timeptr->tm_mday,1900 + timeptr->tm_year,timeptr->tm_hour,
    timeptr->tm_min, timeptr->tm_sec);
  return result;
}


//Function that returns local time in asci
char* log_tm(){
    struct tm* asctm;
    time_t raw_tm;
    time(&raw_tm);
    asctm = localtime(&raw_tm);
    return atime(asctm);
}



//Function takes file and application name to be used across logging functions
char alogger_init(char logg_file[500],char name[200],char strace_file[200]){
    if (sizeof(*name)<16 && sizeof(*name) < 9)
    {
        if(name != NULL){
        strncpy(log_s.name,"\n",200);
    }else{
    strncpy(log_s.name,__FILE__,50);
    }
    }
    if(strace_file  != NULL){
        strncpy(log_s.trace_file,strace_file,200);
    }
    strncpy(log_s.log_file,logg_file,500);

}

/*This function logs the recived message in the log file declared by function alogger_init,It is logging the message according to the severity level specified by the integer level
Severity levels:
1 - EMERGENCY
2 - ALERT
3 - CRITICAL
4 - ERROR
5 - WARNING
6 - NOTICE
7 - INFORMATIONAL(logged as INFO)
8 - DEBUG
0 - LOG
*/
int alog(char msg[500],int level){
    int total;
    int msg_len = strlen(msg);
    char msg_verf[msg_len];
    strncpy(msg_verf,msg,msg_len);
    FILE* log_f;
    log_f = fopen(log_s.log_file,"a+");
    char msg_f[850];
    char log_time[250];
    strncpy(log_time,log_tm(),250);
    total = strlen(msg_verf) + log_time + 10;
    switch (level)
    {
    case 1:
        if(log_s.name == NULL){
        snprintf(msg_f,total,"%s  EMERGENCY   %s\n",log_time,msg_verf);
        }else{
        snprintf(msg_f,total,"%s  EMERGENCY   %s\n",log_time,msg_verf);
        }
        break;
    case 2:
        if(log_s.name == NULL){
        snprintf(msg_f,total,"%s  ALERT       %s\n",log_time,msg_verf);
        }else{
        snprintf(msg_f,total,"%s  ALERT       %s\n",log_time,msg_verf);
        }
        break;
    case 3:
        if(log_s.name == NULL){
        snprintf(msg_f,total,"%s  CRITICAL    %s\n",log_time,msg_verf);
        }else{
        snprintf(msg_f,total,"%s  CRITICAL    %s\n",log_time,msg_verf);
        }
        break;
    case 4:
        if(log_s.name == NULL){
        snprintf(msg_f,total,"%s  ERROR       %s\n",log_time,msg_verf);
        }else{
        snprintf(msg_f,total,"%s  ERROR       %s\n",log_time,msg_verf);
        }
        break;
    case 5:
        if(log_s.name == NULL){
        snprintf(msg_f,total,"%s  WARNING     %s\n",log_time,msg_verf);
        }else{
        snprintf(msg_f,total,"%s  WARNING     %s\n",log_time,msg_verf);
        }
        break;
    case 6:
        if(log_s.name == NULL){
        snprintf(msg_f,total,"%s  NOTICE      %s\n",log_time,msg_verf);
        }else{
        snprintf(msg_f,total,"%s  NOTICE      %s\n",log_time,msg_verf);
        }
        break;
    case 7:
        if(log_s.name == NULL){
        snprintf(msg_f,total,"%s  INFO        %s\n",log_time,msg_verf);
        }else{
        snprintf(msg_f,total,"%s  INFO        %s\n",log_time,msg_verf);
        }
        break;
    case 8:
        if(log_s.name == NULL){
        snprintf(msg_f,total,"%s  DEBUG       %s\n",log_time,msg_verf);
        }else{
        snprintf(msg_f,total,"%s  DEBUG       %s\n",log_time,msg_verf);
        }
        break;
    default:
        if(log_s.name == NULL){
        snprintf(msg_f,total,"%s  LOG         %s\n",log_time,msg_verf);
        }else{
        snprintf(msg_f,total,"%s  LOG         %s\n",log_time,msg_verf);
        }
        break;
    }
    pthread_mutex_lock(&lock);
    fputs(msg_f,log_f);
    fclose(log_f);
    pthread_mutex_unlock(&lock);
}


//Catch stack trace and write to the stack trace log file
char alog_strace(char msg[500]){
    int total;
    int msg_len = strlen(msg);
    void *trace[500];
    int i,size = backtrace(trace,500);
    char** strings = backtrace_symbols(trace,size);
    char stack_trace[500];
    for (i = 0; i < size; i++)
    {
        strcat(stack_trace,strings[i]);
        strcat(stack_trace,"\n");
    }
    char msg_verf[msg_len];
    strncpy(msg_verf,msg,msg_len);
    FILE* log_f;
    log_f = fopen(log_s.trace_file,"a+");
    char msg_f[3000];
    char log_time[250];
    strncpy(log_time,log_tm(),250);
    if(log_s.name == NULL)
    {
        snprintf(msg_f,3000,"\n%s %s  TRACE:\n------\n\n%s\n------\n",log_time,msg_verf,stack_trace);
    }else{
        snprintf(msg_f,3000,"\n%s  %s  %s  TRACE:\n------\n\n%s\n------\n",log_time,log_s.name,msg_verf,stack_trace);
    }
    pthread_mutex_lock(&slock);
    fputs(msg_f,log_f);
    fclose(log_f);
    pthread_mutex_unlock(&slock);
}


    

int main(){
    alogger_init("log.txt","Logger","strace.txt");
    
    alog("TES",1);
    alog("TES",2);
    alog("TES",3);
    alog("TES",4);
    alog("TES",5);
    alog("TES",6);
    alog("TES",7);
    alog("TES",8);
    alog("TES",0);
    alog_strace("TEST");
    
}