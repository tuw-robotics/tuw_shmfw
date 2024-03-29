/***************************************************************************
 *   Software License Agreement (BSD License)                              *
 *   Copyright (C) 2012 by Markus Bader <markus.bader@tuwien.ac.at>        *
 *                                                                         *
 *   Redistribution and use in source and binary forms, with or without    *
 *   modification, are permitted provided that the following conditions    *
 *   are met:                                                              *
 *                                                                         *
 *   1. Redistributions of source code must retain the above copyright     *
 *      notice, this list of conditions and the following disclaimer.      *
 *   2. Redistributions in binary form must reproduce the above copyright  *
 *      notice, this list of conditions and the following disclaimer in    *
 *      the documentation and/or other materials provided with the         *
 *      distribution.                                                      *
 *   3. Neither the name of the copyright holder nor the names of its      *
 *      contributors may be used to endorse or promote products derived    *
 *      from this software without specific prior written permission.      *
 *                                                                         *
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS   *
 *   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT     *
 *   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS     *
 *   FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE        *
 *   COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,  *
 *   INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,  *
 *   BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;      *
 *   LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER      *
 *   CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT    *
 *   LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY *
 *   WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE           *
 *   POSSIBILITY OF SUCH DAMAGE.                                           *
 ***************************************************************************/

#ifndef SHARED_MEM_VARIABLE_HPP
#define SHARED_MEM_VARIABLE_HPP

#include <tuw_shmfw/header.hpp>
#include <tuw_shmfw/handler.hpp>

/**
 * Namespace for the fast and dynamic shared memory framework
 */
namespace ShmFw {

/// Class to manage a simple shared memory variable or array
template<typename T>
class Var : public Header {
    friend class boost::serialization::access;
    T *data_element;
public:


    /** Default constructor
     * @post Var::construct
     **/
    Var() : data_element(NULL) {
    }
    /** Constructor
     * @param name name of the variable
     * @param shmHdl pointer to the shared memory segment handler
     * @pre the ShmPtr poitner must be created first
     * @see ShmFw::createSegment
     * @see ShmFw::construct
     **/
    Var ( const std::string &name, HandlerPtr &shmHdl ) : data_element(NULL) {
        if ( construct ( name, shmHdl ) == ERROR ) exit ( 1 );
    }
    /**
     * @param shm_instance_name name of the shared header element in the memory segment
     * @param shmHdl pointer to the shared memory segment handler
     * @param data on NULL the fnc will an anonymous instance otherwise this data will be linked to the shared header
     * @pre the ShmPtr poitner must be created first
     * @see ShmFw::createSegment
     * @see ShmFw::ptrconstruct
     **/
    int construct ( const std::string &shm_instance_name, HandlerPtr &shmHdl, boost::interprocess::offset_ptr<T> data = NULL) {
        size_t type_hash_code = typeid ( Var<T> ).hash_code();
        const char *type_name = typeid ( Var<T> ).name();
        if ( constructHeader ( shm_instance_name, shmHdl, type_name, type_hash_code ) == ERROR ) return ERROR;
        if ( header_local.creator ) {
            /// constructing shared data
            try {
                ScopedLock myLock ( header_shared->mutex );
                header_shared->container = ShmFw::Header::CONTAINER_VARIABLE;
		if(data){
		  header_shared->data = data;
		} else {
		  header_shared->data = header_local.shm_handler->getShm()->construct< T > ( bi::anonymous_instance ) ();
		}
            } catch ( ... ) {
                std::cerr << "Error when constructing shared data" << std::endl;
                return ERROR;
            }
        }
        data_element = (T*) header_shared->data.get();
        return OK;
    }
    /** UNSAVE!! (user have to lock and to update timestamp)
     * returns a pointer to the shared object
     * @return ref to shared data
     **/
    T *get() {
        return data_element;
    }
    /** UNSAVE!! (user have to lock and to update timestamp)
     * returns a pointer to the shared object
     * @return ref to shared data
     **/
    const T *get() const {
        return data_element;
    }
    /** UNSAVE!! (user have to lock and to update timestamp)
     * returns a pointer to the shared object
     * @return ref to shared data
     **/
    T *operator->() {
        return get();
    }
    /** UNSAVE!! (user have to lock and to update timestamp)
     * returns a pointer to the shared object
     * @return ref to shared data
     **/
    const T *operator->() const {
        return get();
    }
    /** UNSAVE!! (user have to lock and to update timestamp)
     * returns a reference to the shared object
     * @return ref to shared data
     **/
    T &operator*() {
        return *get();
    }
    /** UNSAVE!! (user have to lock and to update timestamp)
     * returns a reference to the shared object
     * @return ref to shared data
     **/
    const T &operator*() const {
        return *get();
    }
    /** UNSAVE!! (user have to lock and to update timestamp)
     * operator to set the shared memory
     * the timestamp will not be updated
     * @param source value
     * @return ref to shared data
     **/
    T &operator = ( const T &source ) {
        *get() = source;
        return *get();
    }
    /** UNSAVE!! (user have to lock and to update timestamp)
     * operator to set the shared memory
     * the timestamp will not be updated
     * @param v value
     * @return ref to shared data
     **/
    T &operator = ( const Var<T> &v ) {
        *get() = *v.get();
        return *get();
    }
    /** SAVE ACCESS :-) (the function will to the lock and the timstamp stuff)
     * copies data to the shared variable and updated the timestamps and locks the variable while accessing
     * @param source
     **/
    void set ( const T &source ) {
        lock();
        *get() = source;
        unlock();
        itHasChanged();
    }
    /** SAVE ACCESS :-) (the function will to the lock and the timstamp stuff)
     *  copies data form the shared variable into a local varaiable and sets local timestamps and locks the variable while accessing
     * @param destination
     **/
    void get ( T &destination ) {
        lock();
        destination = *get();
        unlock();
        dataProcessed();
    }
    /** SAVE ACCESS :-) (the function will to the lock and the timstamp stuff)
     *  copies data form the shared variable into a local varaiable and sets local timestamps and locks the variable while accessing
     * @param destination
     * @param ms to wait befor the function returns it the mutex is locked
     * @return If the thread acquires ownership of the mutex, returns true
     * @see timed_lock()
     **/
    bool get ( T &destination, int ms ) {
        if ( timed_lock ( ms ) ) {
            get ( destination );
            unlock();
            updateTimestampLocal();
            return true;
        } else {
            return false;
        }
    }
    /** UNSAVE!! (user have to lock and to update timestamp)
     * destroies the shared memory
     **/
    virtual void destroy() const {
        header_local.shm_handler->getShm()->destroy_ptr ( get() );
        Header::destroy();
    };

    /**
     * overloads the << and calls the varalible overloades operator
     **/
    friend std::ostream &operator << ( std::ostream &os, const Var<T> &o ) {
        return os << *o.get();
    };
    /** UNSAVE!! (user have to lock and to update timestamp)
     *  @param o vector for comparison
     **/
    template<typename T1>
    bool operator == (const T1 &o ) const {
	return (*get() == o);
    }
    /** UNSAVE!! (user have to lock and to update timestamp)
     *  @param o vector for comparison
     **/
    template<typename T1>
    bool operator != (const T1 &o ) const {
	return (*get() != o);
    }
};
};
#endif //SHARED_MEM_VARIABLE_HPP






