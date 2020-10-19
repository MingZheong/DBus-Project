#include "DBus_Base.h"


namespace DBUS{

DBus_Base::DBus_Base(std::string name)
{
    mBusName = "com."+name+".Application";
    mObjPath = "com."+name+".Object";
    mInterface = "com."+name+".Interface";

    mRule = "type='signal', interface='"+mInterface+"'";

    dbus_error_init(mDbusErr);

    mConnection = dbus_bus_get(DBUS_BUS_SESSION, mDbusErr);
    if(!mConnection)
        if(dbus_error_is_set(mDbusErr))
            std::cout << mDbusErr->message << std::endl;

    // replace_existing proterties make sure that they do not persistently associate it with the connection-name pair
    int ret = dbus_bus_request_name(mConnection, mBusName.c_str(), DBUS_NAME_FLAG_REPLACE_EXISTING, mDbusErr);
    if(dbus_error_is_set(mDbusErr))
        std::cout << mDbusErr->message << std::endl;

    if(ret != DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER)
        if(dbus_error_is_set(mDbusErr))
            std::cout << mDbusErr->message << std::endl;
        
    
    std::cout << "init OK" << std::endl;

}

DBus_Base::~DBus_Base()
{

}

int DBus_Base::send(std::string dest_name, std::string msg_name, MessageType messageType, 
                    DataType dataType, void *data, int cnt)
{
    std::string busName = "com."+dest_name+".Application";
    std::string objPath = "com."+dest_name+".Object";
    std::string interface = "com."+dest_name+".Interface";

    dbus_uint32_t serial;
    DBusMessage *sendMsg, *replyMsg;
    DBusPendingCall *pending;

    if(messageType == Signal){
        // dbus_message_iter_init()
    }
    else if(messageType == Method){

    }
    else{

    }
}

void DBus_Base::run()
{

}
void DBus_Base::stop()
{

}

int DBus_Base::Message(std::string msg_name, MessageType message_type, void *data, int cnt)
{

}


int DBus_Base::toMatchDataType(DataType data_type)
{
    switch (data_type)
    {
    case DataType::Int: return DBUS_TYPE_INT32;
    case DataType::Double: return DBUS_TYPE_DOUBLE;
    case DataType::String: return DBUS_TYPE_STRING;
    case DataType::Boolean: return DBUS_TYPE_BOOLEAN;
    case DataType::Array: return DBUS_TYPE_ARRAY;
    case DataType::Struct: return DBUS_TYPE_STRUCT;
    }
    return -1;
}

}