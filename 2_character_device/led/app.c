#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define WR_VALUE _IOW('a', '1', int32_t *)
#define RD_VALUE _IOR('a', '2', int32_t *)

#define CDEV_PATH "/dev/m_device"

int fd, option;
int number, status;
char write_buf[1024];
char read_buf[1024];

void printMenu()
{
    printf("**** Please Enter the Option *****\n");
    printf("        1. Turn ON LED (Write)    \n");
    printf("        2. Turn OFF LED (Write)   \n");
    printf("        3. Read LED Status        \n");
    printf("        4. Control LED via IOCTL  \n");
    printf("        5. Read LED via IOCTL     \n");
    printf("        99. Exit                  \n");
    printf("**********************************\n");
    printf(">>> ");
}

int main()
{   
    printf("**********************************\n");
    printf("******* LED Controller App *******\n\n");

    fd = open(CDEV_PATH, O_RDWR);
    if (fd < 0) {
        printf("Cannot open device file: %s...\n", CDEV_PATH);
        return -1;
    }

    while(1) {
        printMenu();

        if (scanf("%d", &option) != 1) {
            while(getchar() != '\n'); /* Clear input buffer */
            continue;
        }

        switch (option) {
            case 1:
                printf("Writing '1' to turn ON LED...\n");
                write(fd, "1", 2);
                printf("Done!\n\n");
                break;

            case 2:
                printf("Writing '0' to turn OFF LED...\n");
                write(fd, "0", 2);
                printf("Done!\n\n");
                break;

            case 3:
                printf("Reading status from driver... ");
                memset(read_buf, 0, sizeof(read_buf));
                read(fd, read_buf, sizeof(read_buf) - 1);                
                printf("Current Status: %s\n\n", read_buf);
                break;

            case 4:
                printf("Enter state via IOCTL (1: ON, 0: OFF): ");
                scanf("%d", &number); 
                ioctl(fd, WR_VALUE, (int32_t*)&number);
                printf("Done!\n\n");
                break;

            case 5:
                printf("Reading value using ioctl... ");
                ioctl(fd, RD_VALUE, (int32_t*)&status);
                printf("LED Status: %s (%d)\n\n", status ? "ON" : "OFF", status);
                break;

            case 99:
                close(fd);
                exit(0);               
                break;

            default:
                printf("Invalid option = %d\n\n", option);
                break;
        }
    }
    
    close(fd);
    return 0;
}