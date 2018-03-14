//
// Created by atmelfan on 2018-03-04.
//

#ifndef SRC_JETSON_CORE_H
#define SRC_JETSON_CORE_H

#include <string>

class Core{
public:


    virtual void update() = 0;

    virtual std::string get_name() const = 0;

    virtual bool receive(const message& msg) = 0;

    int get_id(){ return  uuid; }

    template <typename T> struct message{
        message(int sender, bool cb = false): sender(sender), callback_freed(cb){
            payload = new T();
        }

        ~message(){
            delete payload;
        }

        /**
         * Id of core that sent this message
         */
        int sender;

        /**
         *
         * @return
         */
        T* get_payload(){
            return (T*)&payload;
        }

        /**
         * Increment reference count preventing message from being garbage collected.
         */
        void lock(){ ref_count++; }

        /**
         * Decrements reference count. When <=0, message may be garbage collected.
         */
        void free(){ ref_count--; }

        /**
         *
         * @return
         */
        int get_refs(){ return ref_count; }

        /**
         * Allocate a new message with payload T
         * @tparam T
         * @return
         */
    protected:
        bool callback_freed; /*True if sender should be notified when message has been freed*/
        /**/
        int ref_count;
        void* payload;
    };
protected:
    int uuid;
};




#endif //SRC_JETSON_CORE_H
