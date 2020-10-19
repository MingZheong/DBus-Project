#ifndef _DBUS_BASE_H
#define _DBUS_BASE_H

#include "dbus/dbus.h"
#include <iostream>
#include <cstdlib>
#include <thread>
#include <mutex>
#include <condition_variable>

namespace DBUS{

enum MessageType{Signal, Method};
enum DataType{Int, Double, Boolean, String, Struct, Array};

class DBus_Base
{
    
public:
    DBus_Base(std::string name);
    ~DBus_Base();

    int send(std::string dest_name, std::string msg_name, 
            MessageType messageType, DataType dataType, void *data, int cnt);

    void run();
    void stop();

private:

protected:
    virtual int Message(std::string msg_name, MessageType message_type, void *data, int cnt);
    static int toMatchDataType(DataType data_type);

private:
    std::thread *mRecThread;
    std::thread *mTaskThread;
    std::mutex mTaskMutex;
    std::condition_variable mTaskCond;

    DBusConnection *mConnection;
    DBusError *mDbusErr;

    std::string mBusName;
    std::string mObjPath;
    std::string mInterface;
    std::string mRule;

};

}





#endif