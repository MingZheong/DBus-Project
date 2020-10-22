#include "DBus_Base.h"

// #define  std::cout << __LINE__ << std::endl;

namespace DBUS{

/**
 * 
 */
void register_type(std::string name, std::string fullName, int dbusType)
{
    Data_Type dt;
    dt.dbus_type = dbusType;
    dt.fullType = fullName;
    dt.type = name;
    typeVec.push_back(dt);
}


/**
 * 
 */
DBus_Base::DBus_Base(std::string name)
{
    init_base_type();
    
    mBusName = "com."+name+".Application";
    mObjPath = "com."+name+".Object";
    mInterface = "com."+name+".Interface";

    mRule = "type='signal', interface='"+mInterface+"'";

    mDbusErr = new DBusError;
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

/**
 * 
 */
DBus_Base::~DBus_Base()
{

}

/**
 * 
 */
int DBus_Base::send2(std::string dest_name, std::string msg_name, MessageType messageType, 
                    DataType dataType, void *data, int cnt)
{
    std::string busName = "com."+dest_name+".Application";
    std::string objPath = "/com/"+dest_name+"/Object";
    std::string interface = "com."+dest_name+".Interface";

    dbus_uint32_t serial;
    DBusMessage *sendMsg, *replyMsg;
    DBusPendingCall *pending;

    DBusMessageIter msgIter, subStructIter, subArrayIter;

    if(messageType == Signal){
        sendMsg = dbus_message_new_signal(objPath.c_str(), interface.c_str(), msg_name.c_str());
        if(!sendMsg){
            if(dbus_error_is_set(mDbusErr)){
                std::cout << "new signal error:" << mDbusErr->message << std::endl;
                dbus_error_free(mDbusErr);
            }
        }

        dbus_message_iter_init_append(sendMsg, &msgIter);
        if(dbus_error_is_set(mDbusErr)){
            std::cout << "init iter append error:" << mDbusErr->message << std::endl;
                dbus_error_free(mDbusErr);
        }

        if(!dbus_message_iter_open_container(&msgIter, DBUS_TYPE_STRUCT, NULL, &subStructIter)){
            if(dbus_error_is_set(mDbusErr)){
                std::cout << "init open container error:" << mDbusErr->message << std::endl;
                    dbus_error_free(mDbusErr);
            }
        }

        const char* str = msg_name.c_str();

        if(!dbus_message_iter_append_basic(&subStructIter, DBUS_TYPE_STRING, &str)){
            if(dbus_error_is_set(mDbusErr)){
                std::cout << "init append basic error:" << mDbusErr->message << std::endl;
                    dbus_error_free(mDbusErr);
            }
        }

        if(!dbus_message_iter_open_container(&subStructIter, DBUS_TYPE_ARRAY, DBUS_TYPE_BYTE_AS_STRING, &subArrayIter)){
            if(dbus_error_is_set(mDbusErr)){
                std::cout << "init open container error:" << mDbusErr->message << std::endl;
                    dbus_error_free(mDbusErr);
            }
        }

        if(!dbus_message_iter_append_fixed_array(&subArrayIter, DBUS_TYPE_BYTE, &data, cnt)){
            if(dbus_error_is_set(mDbusErr)){
                std::cout << "init append fix array error:" << mDbusErr->message << std::endl;
                    dbus_error_free(mDbusErr);
            }
        }

        if(dbus_message_iter_close_container(&subStructIter, &subArrayIter)){
            if(dbus_error_is_set(mDbusErr)){
                std::cout << "init closed container error:" << mDbusErr->message << std::endl;
                    dbus_error_free(mDbusErr);
            }
        }

        if(dbus_message_iter_close_container(&msgIter, &subStructIter)){
            if(dbus_error_is_set(mDbusErr)){
                std::cout << "init closed container error:" << mDbusErr->message << std::endl;
                    dbus_error_free(mDbusErr);
            }
        }

        if(!dbus_connection_send(mConnection, sendMsg, &serial)){

        }

        dbus_connection_flush(mConnection);

        dbus_message_unref(sendMsg);
    }
    else if(messageType == Method){

    }
    else{

    }
}


/**
 * 
 */
void DBus_Base::run()
{

}

/**
 * 
 */
void DBus_Base::stop()
{

}

/**
 * 
 */
int DBus_Base::Message(std::string msg_name, MessageType message_type, void *data, int cnt)
{

}


/**
 * 
 */
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

/**
 * 
 */
void DBus_Base::init_base_type()
{
    register_type("i","int",DBUS_TYPE_INT32);
    register_type("s","short",DBUS_TYPE_INT16);
    register_type("l","long",DBUS_TYPE_INT64);
    register_type("Pc","char*",DBUS_TYPE_STRING);
    register_type("PKc","const char*",DBUS_TYPE_STRING);
    register_type("NSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE","string",DBUS_TYPE_STRING);
    register_type("d","double",DBUS_TYPE_DOUBLE);
    
    register_type("f","float",0);
    register_type("c","char",0);
    register_type("x","long long",0);
}


/**
 * 
 */
void DBus_Base::sendPrepare(std::string dest_name, std::string msg_name, MessageType messageType)
{
    // prepare
    msgBusName = "org."+dest_name+".DBus";
    msgObjPath = "/org/"+dest_name+"/DBus";
    msgIface = "org."+dest_name+".DBus";

    // if(msgIter != NULL)
        // delete msgIter;
    msgIter = new DBusMessageIter;
    
    if(messageType == Signal){
        msgSendMsg = dbus_message_new_signal(msgObjPath.c_str(), msgIface.c_str(), msg_name.c_str());
        if(msgSendMsg == NULL){
            dbus_error_is_set(mDbusErr);
            std::cout << mDbusErr->message << std::endl;
            dbus_error_free(mDbusErr);
            return;
        }
    }
    else if(messageType == Method){
        msgSendMsg = dbus_message_new_method_call(msgBusName.c_str(), msgObjPath.c_str(), msgIface.c_str(), msg_name.c_str());
        if(msgSendMsg == NULL){
            dbus_error_is_set(mDbusErr);
            std::cout << mDbusErr->message << std::endl;
            dbus_error_free(mDbusErr);
            return ;
        }
    }
    else{
        std::cout << "neither signal nor method!" << std::endl;
        dbus_message_unref(msgSendMsg);
        return ;
    }

    dbus_message_iter_init_append(msgSendMsg, msgIter);
    if(dbus_error_is_set(mDbusErr))
        std::cout << "error: " << mDbusErr->message << std::endl;
    msgIterVec.push_back(msgIter);
}

/**
 * 
 */
void DBus_Base::sendRelease(MessageType messageType)
{
    for(std::vector<DBusMessageIter*>::iterator it=msgIterVec.begin()+1; it!=msgIterVec.end()&&msgIterVec.size()>1; it++){
        std::cout << "close container" <<    std::endl;
        if(!dbus_message_iter_close_container(msgIter, *it.base())){
            if(dbus_error_is_set(mDbusErr)){
                std::cout << mDbusErr->message << std::endl;
                dbus_error_free(mDbusErr);
            }
        }
        delete *it.base();
    }

    delete msgIterVec.front();
    msgIterVec.clear();

    if(messageType == Signal){
        if(!dbus_connection_send(mConnection, msgSendMsg, &msgSerial)){
                if(dbus_error_is_set(mDbusErr)){
                std::cout << mDbusErr->message << std::endl;
                dbus_message_unref(msgSendMsg);
                return ;
            }
        }
    }
    else if(messageType == Method){
        if(!dbus_connection_send_with_reply(mConnection, msgSendMsg, &msgPending, -1)){
            if(dbus_error_is_set(mDbusErr)){
                std::cout << mDbusErr->message << std::endl;
                dbus_error_free(mDbusErr);
            }
        }
        if(msgPending != NULL){

        }
    }
    else{
        
    }
    
    dbus_connection_flush(mConnection);

    dbus_message_unref(msgSendMsg);

    if(messageType == Method){
        getReplyArgs();
    }
}

/**
 * 
 */
void DBus_Base::getReplyArgs()
{
    dbus_pending_call_block(msgPending);

    msgReplyMsg = dbus_pending_call_steal_reply(msgPending);
    if(msgReplyMsg == NULL){
        if(dbus_error_is_set(mDbusErr)){
            std::cout << mDbusErr->message << std::endl;
            dbus_message_unref(msgReplyMsg);
            dbus_error_free(mDbusErr);
        }
    }
    dbus_pending_call_unref(msgPending);
    msgIter = new DBusMessageIter;
    dbus_message_iter_init(msgReplyMsg, msgIter);

    pasedArgs(msgIter);

    dbus_message_unref(msgReplyMsg);
    delete msgIter;
}

/**
 * 
 */
void DBus_Base::pasedArgs(DBusMessageIter* iter)
{
    do
    {
        int type = dbus_message_iter_get_arg_type(iter);
        if(type == DBUS_TYPE_INT16){
            short data;
            dbus_message_iter_get_basic(iter, &data);
            std::cout << data << std::endl;
        }
        else if(type == DBUS_TYPE_INT32){
            int data;
            dbus_message_iter_get_basic(iter, &data);
            std::cout << data << std::endl;
        }
        else if(type == DBUS_TYPE_INT64){
            long data;
            dbus_message_iter_get_basic(iter, &data);
            std::cout << data << std::endl;
        }
        else if(type == DBUS_TYPE_STRING){
            char* data;
            dbus_message_iter_get_basic(iter, &data);
            std::cout << data << std::endl;
        }
        else if(type == DBUS_TYPE_DOUBLE){
            double data;
            dbus_message_iter_get_basic(iter, &data);
            std::cout << data << std::endl;
        }
        else if(type == DBUS_TYPE_ARRAY){
            DBusMessageIter subIter;
            int elementType = dbus_message_iter_get_element_type(iter);
            dbus_message_iter_recurse(iter, &subIter);
            if(elementType == DBUS_TYPE_BYTE){
                char *data = NULL;
                dbus_message_iter_get_basic(&subIter, &data);
                std::cout << data << std::endl;
            }else
                pasedArgs(&subIter);
        }
        else if(type == DBUS_TYPE_STRUCT){
            DBusMessageIter subIter;
            dbus_message_iter_recurse(iter, &subIter);
            pasedArgs(&subIter);
        }
        dbus_message_iter_next(iter);
    }while (dbus_message_iter_has_next(iter));
    std::cout << "has  no next arg" << std::endl;
}

/**
 * 
 */
int DBus_Base::appendByType(std::vector<DBusMessageIter*>& iter, std::string type, const void* data)
{
    Data_Type dt;
    for(auto& i:typeVec)
        if(i.type == type){
            dt = i;
            std::cout << i.type << std::endl;
            break;
        }
     
    std::cout << dt.type << std::endl;    

    if(dt.type.compare("i") == 0 && tmpData == NULL){
        if(!dbus_message_iter_append_basic(iter.front(), dt.dbus_type, data))
            if(dbus_error_is_set(mDbusErr))
                std::cout << "error: " << mDbusErr->message << std::endl;
    }
    else if(dt.type.compare("s") == 0){
        if(!dbus_message_iter_append_basic(iter.front(), dt.dbus_type, data))
            std::cout << "error: " << mDbusErr->message << std::endl;
    }
    else if(dt.type.compare("l") == 0){
        if(!dbus_message_iter_append_basic(iter.front(), dt.dbus_type, data)) 
            std::cout << "error: " << mDbusErr->message << std::endl;
    }
    else if(dt.type.compare("d") == 0){
        if(!dbus_message_iter_append_basic(iter.front(), dt.dbus_type, data)) 
            std::cout << "error: " << mDbusErr->message << std::endl;
    }
    else if(dt.type.compare("Pc") == 0 || dt.type.compare("PKc") == 0){
        const char *tmp = *(const char **)data;
        if(!dbus_message_iter_append_basic(iter.front(), dt.dbus_type, &tmp))
            std::cout << "error: " << mDbusErr->message << std::endl;
    }
    else if(dt.fullType.compare("string") == 0){
        std::string *str = (std::string*)data;
        const char* tmp = str->c_str();
        if(!dbus_message_iter_append_basic(iter.front(), dt.dbus_type, &tmp))
            std::cout << "error: " << mDbusErr->message << std::endl;
    }else if(dt.type.compare("c") == 0 || dt.type.compare("x") == 0 || dt.type.compare("f") == 0){
        return -1;
    }
    else if(tmpData == NULL){
        tmpData = data;
    }
    else{
        DBusMessageIter *subIter = new DBusMessageIter;
        if(!dbus_message_iter_open_container(iter.front(), DBUS_TYPE_ARRAY, DBUS_TYPE_BYTE_AS_STRING, subIter)){
            dbus_error_is_set(mDbusErr);
            std::cout << mDbusErr->message << std::endl;
            return -2;
        }
        
        ::Message *msg = (::Message*)tmpData;

        std::cout << msg->buf << std::endl;
        std::cout << *(int*)data << std::endl;
        if(!dbus_message_iter_append_fixed_array(subIter, DBUS_TYPE_BYTE, &tmpData, *(int*)data))
            std::cout << "error: " << mDbusErr->message << std::endl;
        iter.push_back(subIter);

        tmpData = NULL;
    }

    return 0;
}

}

