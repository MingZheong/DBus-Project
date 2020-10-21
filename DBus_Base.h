#ifndef _DBUS_BASE_H
#define _DBUS_BASE_H

#include "dbus/dbus.h"
#include <iostream>
#include <cstdlib>
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

struct Data_Type
{
    std::string type;
    int dbus_type;
    std::string fullType;
};

enum MessageType{Signal, Method};
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
    int deal(std::vector<DBusMessageIter*> &iter, const Types arg){
        
        int ret = 0;
        // std::cout << arg << std::endl;
        // if(strcmp(typeid(arg).name(), "Pc") || strcmp(typeid(arg).name(),"PKc"))
        //     ret = appendByType(iter, typeid(arg).name(), arg);
        // else
            ret = appendByType(iter, typeid(arg).name(), &arg);

        return ret;
    }

    // 
    template<typename... Types>
    void send(std::string dest_name, std::string msg_name, MessageType messageType, const Types... rest){
        
        // prepare
        std::string busName = "com."+dest_name+".Application";
        std::string objPath = "/com/"+dest_name+"/Object";
        std::string interface = "com."+dest_name+".Interface";

        dbus_uint32_t serial;
        DBusMessage *sendMsg, *replyMsg;
        DBusPendingCall *pending;

        std::vector<DBusMessageIter*> iterVec;
        DBusMessageIter iter;
        
        if(messageType == Signal){
            sendMsg = dbus_message_new_signal(objPath.c_str(), interface.c_str(), msg_name.c_str());
            if(sendMsg == NULL){
                dbus_error_is_set(mDbusErr);
                std::cout << mDbusErr->message << std::endl;
                dbus_error_free(mDbusErr);
                return;
            }
        }
        else if(messageType == Method){
            sendMsg = dbus_message_new_method_call(busName.c_str(), objPath.c_str(), interface.c_str(), msg_name.c_str());
            if(sendMsg == NULL){
                dbus_error_is_set(mDbusErr);
                std::cout << mDbusErr->message << std::endl;
                dbus_error_free(mDbusErr);
                return ;
            }
        }
        else{
            std::cout << "neither signal nor method!" << std::endl;
            dbus_message_unref(sendMsg);
            return ;
        }

        dbus_message_iter_init_append(sendMsg, &iter);
        if(dbus_error_is_set(mDbusErr))
            std::cout << "error: " << mDbusErr->message << std::endl;
        iterVec.push_back(&iter);
        
        int arr[] = {deal(iterVec, rest)...};

        for(std::vector<DBusMessageIter*>::iterator it=iterVec.begin()+1; it!=iterVec.end()&&iterVec.size()>1; it++){
            std::cout << "close container" <<    std::endl;
            if(!dbus_message_iter_close_container(&iter, *it.base())){
                if(dbus_error_is_set(mDbusErr)){
                    std::cout << mDbusErr->message << std::endl;

                }
            }
            delete *it.base();
        }

        if(!dbus_connection_send(mConnection, sendMsg, &serial)){
            dbus_error_is_set(mDbusErr);
            std::cout << mDbusErr->message << std::endl;
            dbus_message_unref(sendMsg);
            return ;
        }

        dbus_connection_flush(mConnection);

        dbus_message_unref(sendMsg);

    }

    void run();
    void stop();

private:
    int appendByType(std::vector<DBusMessageIter*>&, std::string type, const void* data);

protected:
    virtual int Message(std::string msg_name, MessageType message_type, void *data, int cnt);
    static int toMatchDataType(DataType data_type);
    void init_base_type();

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

};

}





#endif