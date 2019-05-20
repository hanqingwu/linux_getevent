/*************************************************************************
	> File Name: getevent.h
	> Author: 
	> Mail: 
	> Created Time: 2019年05月18日 星期六 11时56分02秒
 ************************************************************************/

#ifndef _GETEVENT_H
#define _GETEVENT_H


#include <linux/input.h>  

class InputSubSystem
{
public:
    InputSubSystem();
    ~InputSubSystem();

    //初始化打开input event 设备句柄 
    //event_num 为不同event 设备
    int init(int event_num);

    //读取事件
    int get_input_event(struct input_event *event);

    //释放input event 设备句柄 
    void exit();

private:

    int event_fd;

};


#endif
