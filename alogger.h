#ifndef alogger_h
#define alogger_h

//struct for global variables
struct logger{
    char log_file[200];
    int log_level;
    char name[500];
    char trace_file[200];
};

//Lock used for mutxex across logging functions
pthread_mutex_t lock;


//Function takes file and application name to be used across logging functions
char alogger_init(char logg_file[500],char name[200],char strace_file[200]);

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
int alog(char msg[500],int level);

//Catch stack trace and write to the stack trace log file
char alog_strace(char msg[500]);

//Turnicate log files
int alog_turn(int log_file,int trace_log,size_t f_size,int num_files);

#endif
