#include <iostream>
#include <typeinfo>
#include "DBus_Base.h"
#include <unistd.h>

using namespace std;

// struct Message
// {
//     char buf[10];
// };

int main(){
    DBUS::DBus_Base dbus("dev");

    // while(1){
        
        Message msg;
        DBUS::register_type(msg);
        for(int i=0; i<10240; i++)
            memcpy(msg.buf+(i*10), "1234567890", 10);
        char buf[] = "hello";
        int a = 21;
        short s = 123;
        long l = 12312412354;
        float f = 123.123;
        double d = 321.321;
        string str = "stringsdasda";

        dbus.run();

        // dbus.send("devManager", "letter", DBUS::Signal, a, str, msg, sizeof(Message));
        // dbus.send("devManager", "letter", DBUS::Signal, 1111);
        // dbus.send2("devManager", "letter", DBUS::Signal, DBUS::Struct, &msg, sizeof(Message));
        // dbus.send("freedesktop", "ListNames", DBUS::Method);
        dbus.send("dev", "iamaname", DBUS::Signal, msg, sizeof(Message),a, s, l, f, d, str);
        usleep(500*1000);

        dbus.releaseRecvData();
    // }

    while (1)
    {
        dbus.send("dev", "iamaname", DBUS::Signal, msg, sizeof(Message),a, s, l, f, d, str);
        dbus.releaseRecvData();
        usleep(500*1000);
    }
    
}
