#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include  <fcntl.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>

#define LOG_FILE "/etc/hosts"
#define DAEMON_NAME "daemon_hosts_file_blokirani_web"

#define LOKALCI "/usr/local/bin/filovi_za_blokirani_web/lokalci.txt"
#define PRAZNO "/usr/local/bin/filovi_za_blokirani_web/prazno.txt"

static volatile int keep_running = 1;

void handle_signal(int sig){
    if (sig == SIGTERM || sig == SIGINT){
        keep_running = 0;
    }
}
int main(){
    FILE *log_file;
    time_t now;
    struct tm *local;
    int hour;
    //register signal handler
    signal(SIGTERM, handle_signal);
    signal(SIGINT, handle_signal);
    
    //citanje filova
    //blokirane stranice
    FILE *blokirani_f;
    //prazan local host
    FILE *prazno_f;
    blokirani_f = fopen(LOKALCI,"r");
    if(!blokirani_f){
        perror("Error opening LOKALCI");
        exit(EXIT_FAILURE);
    }
    prazno_f = fopen(PRAZNO,"r");
    if(!prazno_f){
        perror("Error opening PRAZNO");
        exit(EXIT_FAILURE);
    }
    char myString_prazno[1000];
    char myString_blokirano[1000];
    myString_blokirano[0]='\0';
    myString_prazno[0]='\0';
    char pom[1000];

    while(fgets(pom, 1000, blokirani_f)){
        //printf("%s", myString_blokirano);
        strcat(myString_blokirano, pom);
    }
    //printf("%s", myString_blokirano);
    fclose(blokirani_f);

    while(fgets(pom, 1000, prazno_f)){
        //printf("%s",myString_prazno);
        strcat(myString_prazno, pom);
    }
    //printf("%s",myString_prazno);
    fclose(prazno_f);
    time(&now);
    local = localtime(&now);
    hour = local->tm_hour;
       if (hour < 20){
        log_file = fopen(LOG_FILE, "w");
        if(log_file){
            fprintf(log_file, "%s", myString_blokirano);
            fclose(log_file);
        }
    }


    //glavni dio program
    while(keep_running){
        time(&now);
        local = localtime(&now);
        hour = local->tm_hour;
        if(hour >= 20){
        log_file = fopen(LOG_FILE, "w");
        if(log_file){
            fprintf(log_file,"%s", myString_prazno);
            fclose(log_file);
        }
        }else{
            log_file = fopen(LOG_FILE, "w");
            if(log_file){
                fprintf(log_file,"%s",myString_blokirano);
                fclose(log_file);
            }
        }
        sleep(10);
    }
    return EXIT_SUCCESS;
}
