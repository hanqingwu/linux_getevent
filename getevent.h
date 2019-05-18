/*************************************************************************
	> File Name: getevent.h
	> Author: 
	> Mail: 
	> Created Time: 2019年05月18日 星期六 11时56分02秒
 ************************************************************************/

#ifndef _GETEVENT_H
#define _GETEVENT_H


#include <linux/input.h>  
#include <list>


#define MAX_INPUT_NUM   32


class InputSubSystem
{
public:
    InputSubSystem();
    ~InputSubSystem();

    int init();
    int get_input_event(struct input_event *event);

    void exit();

private:

    std::list<int>  fd_list;
    int maxfd;

};


#endif
