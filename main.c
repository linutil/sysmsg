#include <stdio.h>
#include <uuid/uuid.h>
#include <dirent.h>
#include <unistd.h>
#include <pwd.h>
#include <string.h>
#include <stdlib.h>


int send_message(char* user){


    const char* MESSAGE_ROOT_DIR = "/usr/local/share/sysmsg/";
    char *uuid = malloc(37);
    uuid_t binuuid;
    uuid_generate(binuuid);

    uuid_unparse_upper(binuuid, uuid);

    size_t user_msg_dir_size = strlen(user) + strlen(MESSAGE_ROOT_DIR) + 1 + strlen(uuid);
    char* file_path = malloc(user_msg_dir_size);

    for(int i = 0; i < user_msg_dir_size; i++){
        file_path[i] = 0;
    }

    strcpy(file_path, MESSAGE_ROOT_DIR);
    strcat(file_path, user);
    strcat(file_path, "/");
    strcat(file_path, uuid);

    FILE* msg = fopen(file_path, "w+");

    struct passwd *p = getpwuid(getuid());

    fputs(p->pw_name, msg);
    fputs(" - ", msg);

    char c;
    while((c = getchar()) != EOF){
        fputc(c, msg);
    }
    fclose(msg);

}

int read_messages(){

    const char* MESSAGE_ROOT_DIR = "/usr/local/share/sysmsg/";

    struct passwd *p = getpwuid(getuid());

    size_t user_msg_dir_size = strlen(p->pw_name) + strlen(MESSAGE_ROOT_DIR) + 1;
    char* user_message_dir = malloc(user_msg_dir_size);

    for(int i = 0; i < user_msg_dir_size; i++){
        user_message_dir[i] = 0;
    }

    strcpy(user_message_dir, MESSAGE_ROOT_DIR);
    strcat(user_message_dir, p->pw_name);
    strcat(user_message_dir, "/");

    DIR *d;
    struct dirent *dir;
    d = opendir(user_message_dir);
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            if(dir->d_name[0] != '.'){
                char file_path[user_msg_dir_size + strlen(dir->d_name)];
                strcpy(file_path, user_message_dir);
                strcat(file_path, dir->d_name);

                FILE* msg = fopen(file_path, "r");
                char c;
                while((c = fgetc(msg)) != EOF){
                    putchar(c);
                }
                fclose(msg);
            }

        }
        closedir(d);
    } else {
        printf("could not open message dir(%s), attempting creation\n", user_message_dir);
        char command[user_msg_dir_size + strlen("mkdir -p ")];
        strcpy(command, "mkdir -p ");
        strcat(command, user_message_dir);

        system(command);

        read_messages();
    }

    free(user_message_dir);

    return 0;
}

int main(int argc, char *argv[]) {

    if(argc == 1)
        return read_messages();
    if(argc == 2)
        return send_message(argv[1]);

    printf("too many arguments!");
    return 1;
}
