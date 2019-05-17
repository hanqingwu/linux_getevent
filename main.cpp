/*************************************************************************
	> File Name: main.c
	> Author: 
	> Mail: 
	> Created Time: 2019年05月17日 星期五 14时53分44秒
 ************************************************************************/

#include <stdio.h>  
#include <stdlib.h>  
#include <unistd.h>  
#include <string.h>  
#include <sys/types.h>  
#include <fcntl.h>  
#include <errno.h>  
#include <time.h>  
#include <linux/input.h>  
  
struct input_event event;  

#include <list>
#include <iostream>


using namespace std;


#define MAX_INPUT_NUM   32

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

int get_event()
{
    char          name[64];           /* RATS: Use ok, but could be better */  
    list<int> fd_list;
    int i;
    int fd = 0;
    int maxfd = 0;
    char          *tmp;  

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

    while(1)
    {
        struct timeval tv;
        fd_set rfdset; 
        int rc;

        FD_ZERO (&rfdset);
        for (list<int>::iterator it = fd_list.begin(); it != fd_list.end(); it++)
        {
            FD_SET (*it, &rfdset);
        }

        tv.tv_sec = 0;
        tv.tv_usec = 100000;
        rc = select(maxfd + 1, &rfdset, NULL, NULL, &tv);       //通过select筛选处于就绪状态的fd
        if (rc < 0)
        {
            printf("select error!", __FUNCTION__);
            break;
        }
        else if (rc == 0)
        {
            continue;
        }
        else if (rc > 0)
        {
            for (list<int>::iterator it = fd_list.begin(); it != fd_list.end(); it++)
            {
                if (FD_ISSET (*it, &rfdset))
                {
                    while ((rc = read(*it, &event, sizeof(event))) > 0) {  
//                        printf("%-24.24s.%06lu type 0x%04x; code 0x%04x;"  
//                               " value 0x%08x; ",  
//                               ctime(&event.time.tv_sec),  
//                               event.time.tv_usec,  
//                               event.type, event.code, event.value);  
                        switch (event.type) {  
                        case EV_KEY:  
                            if (event.code > BTN_MISC) {  
                                printf("Button %d %s",  
                                       event.code & 0xff,  
                                       event.value ? "press" : "release");  
                            } else {  
                                printf("Key %d (0x%x) %s",  
                                       event.code & 0xff,  
                                       event.code & 0xff,  
                                       event.value ? "press" : "release");  
                            }  
                            break;  
                        case EV_REL:  
                            switch (event.code) {  
                            case REL_X:      tmp = "X";       break;  
                            case REL_Y:      tmp = "Y";       break;  
                            case REL_HWHEEL: tmp = "HWHEEL";  break;  
                            case REL_DIAL:   tmp = "DIAL";    break;  
                            case REL_WHEEL:  tmp = "WHEEL";   break;  
                            case REL_MISC:   tmp = "MISC";    break;  
                            default:         tmp = "UNKNOWN"; break;  
                            }  
                            printf("Relative %s %d", tmp, event.value);  
                            break;  
                        case EV_ABS:  
                            switch (event.code) {  
                            case ABS_X:        tmp = "X";        break;  
                            case ABS_Y:        tmp = "Y";        break;  
                            case ABS_Z:        tmp = "Z";        break;  
                            case ABS_RX:       tmp = "RX";       break;  
                            case ABS_RY:       tmp = "RY";       break;  
                            case ABS_RZ:       tmp = "RZ";       break;  
                            case ABS_THROTTLE: tmp = "THROTTLE"; break;  
                            case ABS_RUDDER:   tmp = "RUDDER";   break;  
                            case ABS_WHEEL:    tmp = "WHEEL";    break;  
                            case ABS_GAS:      tmp = "GAS";      break;  
                            case ABS_BRAKE:    tmp = "BRAKE";    break;  
                            case ABS_HAT0X:    tmp = "HAT0X";    break;  
                            case ABS_HAT0Y:    tmp = "HAT0Y";    break;  
                            case ABS_HAT1X:    tmp = "HAT1X";    break;  
                            case ABS_HAT1Y:    tmp = "HAT1Y";    break;  
                            case ABS_HAT2X:    tmp = "HAT2X";    break;  
                            case ABS_HAT2Y:    tmp = "HAT2Y";    break;  
                            case ABS_HAT3X:    tmp = "HAT3X";    break;  
                            case ABS_HAT3Y:    tmp = "HAT3Y";    break;  
                            case ABS_PRESSURE: tmp = "PRESSURE"; break;  
                            case ABS_DISTANCE: tmp = "DISTANCE"; break;  
                            case ABS_TILT_X:   tmp = "TILT_X";   break;  
                            case ABS_TILT_Y:   tmp = "TILT_Y";   break;  
                            case ABS_MISC:     tmp = "MISC";     break;  
                            default:           tmp = "UNKNOWN";  break;  
                            }  
                            printf("Absolute %s %d", tmp, event.value);  
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
        }
    }

    for (list<int>::iterator it = fd_list.begin(); it != fd_list.end(); it++)
        close(*it);

    return 0;
}

int main(int argc, char **argv)  
{  
    char          name[64];           /* RATS: Use ok, but could be better */  
    char          buf[256] = { 0, };  /* RATS: Use ok */  
    unsigned char mask[EV_MAX/8 + 1]; /* RATS: Use ok */  
    int           version;  
    int           fd = 0;  
    int           rc;  
    int           i, j;  
    char          *tmp;  
  
#define test_bit(bit) (mask[(bit)/8] & (1 << ((bit)%8)))  
  
    
    for (i = 0; i < 32; i++) {  
        sprintf(name, "/dev/input/event%d", i);  
        if ((fd = open(name, O_RDONLY, 0)) >= 0) {  
            ioctl(fd, EVIOCGVERSION, &version);  
            ioctl(fd, EVIOCGNAME(sizeof(buf)), buf);  
            ioctl(fd, EVIOCGBIT(0, sizeof(mask)), mask);  
            printf("%s\n", name);  
            printf("    evdev version: %d.%d.%d\n",  
                   version >> 16, (version >> 8) & 0xff, version & 0xff);  
            printf("    name: %s\n", buf);  
            printf("    features:");  
            for (j = 0; j < EV_MAX; j++) {  
                if (test_bit(j)) {  
                    const char *type = "unknown";  
                    switch(j) {  
                    case EV_KEY: type = "keys/buttons"; break;  
                    case EV_REL: type = "relative";     break;  
                    case EV_ABS: type = "absolute";     break;  
                    case EV_MSC: type = "reserved";     break;  
                    case EV_LED: type = "leds";         break;  
                    case EV_SND: type = "sound";        break;  
                    case EV_REP: type = "repeat";       break;  
                    case EV_FF:  type = "feedback";     break;  
                    }  
                    printf(" %s", type);  
                }  
            }  
            printf("\n");  
            close(fd);  
        }  
    }  
  
    get_event();

    return 0;

}  


