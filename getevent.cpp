/*************************************************************************
	> File Name: getevent.cpp
	> Author: 
	> Mail: 
	> Created Time: 2019年05月18日 星期六 13时43分38秒
 ************************************************************************/

#include <iostream>
#include <stdio.h>  
#include <stdlib.h>  
#include <unistd.h>  
#include <string.h>  
#include <sys/types.h>  
#include <fcntl.h>  
#include <errno.h>  
#include <time.h>  


#include "getevent.h"

using namespace std;

void set_non_blocking(int fd)
{
    int flags;

    flags = fcntl(fd, F_GETFL, NULL);

    if (flags < 0) {
        return ;
    }

    flags |= O_NONBLOCK;

    if (fcntl(fd, F_SETFL, flags) < 0) {
        return ;
    }
}

InputSubSystem::InputSubSystem()
{

}


InputSubSystem::~InputSubSystem()
{

}


int InputSubSystem::init()
{
    char name[64];
    int i;
    int fd;
    int maxfd = 0;


    fd_list.clear();

    for (i = 0; i < MAX_INPUT_NUM; i++) {  
        sprintf(name, "/dev/input/event%d", i);  
        printf("%s get input name %s \n", __FUNCTION__, name);
        if ((fd = open(name, O_RDONLY, 0)) >= 0) {  
            fd_list.push_back(fd);
            maxfd = maxfd > fd ? maxfd : fd;
            printf("get fd %d , maxfd  = %d\n", fd, maxfd);
            set_non_blocking(fd);
       }  
    }  

    return 0;
}

int InputSubSystem::get_input_event(struct input_event *event)
{
    if (event == NULL)
        return -1;

    int rc;
    char          *tmp;  

    for (list<int>::iterator it = fd_list.begin(); it != fd_list.end(); it++)
    {
        while ((rc = read(*it, event, sizeof(struct input_event))) > 0) 
        {  
            //                        printf("%-24.24s.%06lu type 0x%04x; code 0x%04x;"  
            //                               " value 0x%08x; ",  
            //                               ctime(&event.time.tv_sec),  
            //                               event.time.tv_usec,  
            //                               event.type, event.code, event.value);  
            switch (event->type) 
            {  
                case EV_KEY:  
                if (event->code > BTN_MISC) {  
                    printf("Button %d %s",  
                    event->code & 0xff,  
                    event->value ? "press" : "release");  
                } else {  
                    printf("Key %d (0x%x) %s",  
                    event->code & 0xff,  
                    event->code & 0xff,  
                    event->value ? "press" : "release");  
                }  
                    break;  
                case EV_REL:  
                    switch (event->code) {  
                    case REL_X:      tmp = (char *)"X";       break;  
                    case REL_Y:      tmp = (char *)"Y";       break;  
                    case REL_HWHEEL: tmp = (char *)"HWHEEL";  break;  
                    case REL_DIAL:   tmp = (char *) "DIAL";    break;  
                    case REL_WHEEL:  tmp = (char *)"WHEEL";   break;  
                    case REL_MISC:   tmp = (char *)"MISC";    break;  
                    default:         tmp = (char *)"UNKNOWN"; break;  
                    }  
                    printf("Relative %s %d", tmp, event->value);  
                    break;  
                case EV_ABS:  
                switch (event->code) 
                {  
                    case ABS_X:        tmp = (char *)"X";        break;  
                    case ABS_Y:        tmp = (char *)"Y";        break;  
                    case ABS_Z:        tmp = (char *)"Z";        break;  
                    case ABS_RX:       tmp = (char *)"RX";       break;  
                    case ABS_RY:       tmp = (char *)"RY";       break;  
                    case ABS_RZ:       tmp = (char *)"RZ";       break;  
                    case ABS_THROTTLE: tmp = (char *)"THROTTLE"; break;  
                    case ABS_RUDDER:   tmp = (char *)"RUDDER";   break;  
                    case ABS_WHEEL:    tmp = (char *)"WHEEL";    break;  
                    case ABS_GAS:      tmp = (char *)"GAS";      break;  
                    case ABS_BRAKE:    tmp = (char *)"BRAKE";    break;  
                    case ABS_HAT0X:    tmp = (char *)"HAT0X";    break;  
                    case ABS_HAT0Y:    tmp = (char *)"HAT0Y";    break;  
                    case ABS_HAT1X:    tmp = (char *)"HAT1X";    break;  
                    case ABS_HAT1Y:    tmp = (char *)"HAT1Y";    break;  
                    case ABS_HAT2X:    tmp = (char *)"HAT2X";    break;  
                    case ABS_HAT2Y:    tmp = (char *)"HAT2Y";    break;  
                    case ABS_HAT3X:    tmp = (char *)"HAT3X";    break;  
                    case ABS_HAT3Y:    tmp = (char *)"HAT3Y";    break;  
                    case ABS_PRESSURE: tmp = (char *)"PRESSURE"; break;  
                    case ABS_DISTANCE: tmp = (char *)"DISTANCE"; break;  
                    case ABS_TILT_X:   tmp = (char *)"TILT_X";   break;  
                    case ABS_TILT_Y:   tmp = (char *)"TILT_Y";   break;  
                    case ABS_MISC:     tmp = (char *)"MISC";     break;  
                    default:           tmp = (char *)"UNKNOWN";  break;  
                }  
                    printf("Absolute %s %d", tmp, event->value);  
                    break;  
                case EV_MSC: printf("Misc"); break;  
                case EV_LED: printf("Led");  break;  
                case EV_SND: printf("Snd");  break;  
                case EV_REP: printf("Rep");  break;  
                case EV_FF:  printf("FF");   break;  
                    break;  
            }  
            printf("\n");  
        } 
    }
}



void InputSubSystem::exit()
{
    for (list<int>::iterator it = fd_list.begin(); it != fd_list.end(); it++)
        close(*it);

    fd_list.clear();

    return ;
}
