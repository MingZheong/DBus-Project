#ifndef _DBUS_BASE_H
#define _DBUS_BASE_H

#include "dbus/dbus.h"
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <string.h>

#define  LINE std::cout << __LINE__ << std::endl;

struct Message
{
    char buf[100*1024];
};

namespace DBUS{

struct Recv_Data
{
    void* data;
    int type;
    uint size;
};


struct Data_Type
{
    std::string type;
    int dbus_type;
    std::string fullType;
};

enum MessageType{Signal, Method, Method_Return};
enum DataType{Int, Double, Boolean, String, Struct, Array};

static std::vector<Data_Type>typeVec;

static void register_type(std::string name, std::string fullName, int dbusType);

template<typename T>
void register_type(const T data){
    Data_Type dt;
    dt.type = typeid(data).name();
    dt.fullType = dt.type.substr(1);
    dt.dbus_type = DBUS_TYPE_BYTE;
}


class DBus_Base
{
    
public:
    DBus_Base(std::string name);
    ~DBus_Base();

    int send2(std::string dest_name, std::string msg_name, 
            MessageType messageType, DataType dataType, void *data, int cnt);
    
    // deal by type template
    template<typename Types>
    int deal(DBusMessageIter* &iter, const Types arg){
        return appendByType(iter, typeid(arg).name(), &arg);
    }

    // Variadic function template
    template<typename... Types>
    void send(std::string dest_name, std::string msg_name, MessageType messageType, const Types... rest){
        sendPrepare(dest_name, msg_name, messageType);
        int arr[] = {deal(msgIter, rest)...};
        sendRelease(messageType);
    }

    void addMatch(std::string rule);
    void clearMatch(std::string rule);

    void run();
    void stop();
    static void recThread(DBus_Base* obj);
    bool releaseRecvData();

private:
    int appendByType(DBusMessageIter*&, std::string type, const void* data);
    void init_base_type();
    void sendPrepare(std::string dest_name, std::string msg_name, MessageType messageType);
    void sendRelease(MessageType messageType);
    void getReplyArgs();
    void pasedArgs(DBusMessageIter* iter, MessageType type);
    

protected:
    virtual int Message(std::string msg_name, MessageType message_type, std::vector<void*>dataList, int cnt)=0;
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

    const void* tmpData = NULL;

    //--------
    std::string msgBusName;
    std::string msgObjPath;
    std::string msgIface;

    dbus_uint32_t msgSerial;
    DBusMessage *msgSendMsg, *msgReplyMsg;
    DBusPendingCall *msgPending;

    DBusMessageIter *msgIter;

    std::vector<Recv_Data>replyArgs;
    std::vector<Recv_Data>recArgs;

    DBusMessage* returnMsg;
};

}





#endif