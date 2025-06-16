#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include  <fcntl.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>

#define LOG_FILE "/home/frano/Documents/projekti/webblocker_deamon/temp/simple_daemon.log"
#define DAEMON_NAME "symple_daemon"

static volatile int keep_running = 1;

void handle_signal(int sig){
    if (sig == SIGTERM || sig == SIGINT){
        keep_running = 0;
    }
}

void daemonize(){
    pid_t pid;

    //fork of the parent process
    pid = fork();
    if (pid < 0) exit(EXIT_FAILURE);
    if (pid > 0) exit(EXIT_SUCCESS);
    
    //child contunues
    if (setsid()<0) exit(EXIT_FAILURE); //create a new SID

    //catch, ignore and handle signals
    signal(SIGCHLD, SIG_IGN);
    signal(SIGHUP, SIG_IGN);

    //fork again for safety
    pid = fork();
    if (pid < 0) exit(EXIT_FAILURE); 
    if (pid > 0) exit(EXIT_FAILURE);
    
    //set some file premissions
    umask(0);
    //change the working directory to root
    chdir("/");
    
    //close the ope file descriptors
    for (int x = sysconf(_SC_OPEN_MAX); x>= 0; x--){
        close(x);
    }
    //ovo preusmjerava potencijanla pisana u file, dohvat s tipkovnice i ispis grešaka
    // u file /dev/null koji uvijek vrača "prazno" i tako daemon ostaje daemon i vrti se
    // samo u pozadini
    open("/dev/null", O_RDONLY); //stdin
    open("/dev/null", O_WRONLY); //stdout
    open("/dev/null", O_RDWR); //stderr

}
int main(){
    FILE *log_file;
    time_t now;

    //register signal handler
    signal(SIGTERM, handle_signal);
    signal(SIGINT, handle_signal);

    daemonize();

    //glavni dio program
    while(keep_running){
        log_file = fopen(LOG_FILE, "a+");
        if(log_file){
            time(&now); 
            fprintf(log_file,"[%s] Daemon is alive", ctime(&now));
            fclose(log_file);
        }
        sleep(5);
    }
    //clean shotdown
    log_file = fopen(LOG_FILE,"a+");
    if(log_file){
        if(time)
        time(&now);
        fprintf(log_file, "[%s], Daemon shutting down\n",ctime(&now));
        fclose(log_file);
    }
    if(!log_file){
        perror("greska");
        exit(1);
    }
    return EXIT_SUCCESS;
}
