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

#ifndef SHARED_MEM_VECTOR_H
#define SHARED_MEM_VECTOR_H

#include <tuw_shmfw/header.hpp>
#include <boost/interprocess/containers/vector.hpp>

namespace ShmFw {

/// Class to manage a shared vectors
template<typename T>
class Vector : public Header {
    friend class boost::serialization::access;
    typedef bi::vector<T, Allocator<T> > VectorShm;
    typedef typename Allocator<T>::size_type size_type;
    VectorShm *data_element;

public:

    /** Default constructor
     * @post Vector::construct
     **/
    Vector() : data_element ( NULL ) {
    }

    /** Constructor
     * @param name name of the variable
     * @param shmHdl pointer to the shared memory segment
     * @pre the ShmPtr poitner must be created first
     * @see ShmFw::createSegment
     **/
    Vector ( const std::string &name, HandlerPtr &shmHdl ) : data_element ( NULL ) {
        if ( construct ( name, shmHdl ) == ERROR ) exit ( 1 );
    }

    /**
     * @param shm_instance_name name of the shared header element in the memory segment
     * @param shmHdl pointer to the shared memory segment handler
     * @param data on NULL the fnc will an anonymous instance otherwise this data will be linked to the shared header
     * @pre the ShmPtr poitner must be created first
     * @see ShmFw::createSegment
     **/
    int construct ( const std::string &shm_instance_name, HandlerPtr &shmHdl, boost::interprocess::offset_ptr<T> data = NULL ) {
        size_t type_hash_code = typeid ( Vector<T> ).hash_code();
        const char *type_name = typeid ( Vector<T> ).name();
        if ( constructHeader ( shm_instance_name, shmHdl, type_name, type_hash_code ) == ERROR ) return ERROR;
        if ( header_local.creator ) {
            /// constructing shared data
            try {
                ScopedLock myLock ( header_shared->mutex );
                header_shared->container = ShmFw::Header::CONTAINER_VECTOR;
                Allocator<T> a ( header_local.shm_handler->getShm()->get_segment_manager() );
                if ( data ) {
                    header_shared->data = data;
                } else {
                    header_shared->data = header_local.shm_handler->getShm()->construct< VectorShm > ( bi::anonymous_instance ) ( a );
                }
            } catch ( ... ) {
                std::cerr << "Error when constructing shared data" << std::endl;
                return ERROR;
            }
        }
        data_element = ( VectorShm* ) header_shared->data.get();
        return OK;
    }
    /** UNSAVE!! (user have to lock and to update timestamp)
     * returns a pointer to the shared object
     * @return ref to shared data
     **/
    VectorShm *get() {
        return data_element;
    }
    /** UNSAVE!! (user have to lock and to update timestamp)
     * returns a pointer to the shared object
     * @return ref to shared data
     **/
    const VectorShm *get() const {
        return data_element;
    }
    /** UNSAVE!! (user have to lock and to update timestamp)
     * returns a pointer to the shared object
     * @return ref to shared data
     **/
    VectorShm *operator->() {
        return get();
    }
    /** UNSAVE!! (user have to lock and to update timestamp)
     * returns a pointer to the shared object
     * @return ref to shared data
     **/
    const VectorShm *operator->() const {
        return get();
    }
    /** UNSAVE!! (user have to lock and to update timestamp)
     * returns a reference to the shared object
     * @return ref to shared data
     **/
    VectorShm &operator*() {
        return *get();
    }
    /** UNSAVE!! (user have to lock and to update timestamp)
     * returns a reference to the shared object
     * @return ref to shared data
     **/
    const VectorShm &operator*() const {
        return *get();
    }
    /** UNSAVE!! (user have to lock and to update timestamp)
     * returns a reference to the shared vector object by index
     * @return ref to shared data
     **/
    const T &operator [] ( size_type n ) const {
        return ( *get() ) [n];
    }
    /** UNSAVE!! (user have to lock and to update timestamp)
     * returns a reference to the shared vector object by index
     * @return ref to shared data
     **/
    T &operator [] ( size_type n ) {
        return ( *get() ) [n];
    }
    /** SAVE ACCESS :-) (the function will to the lock and the timstamp stuff)
     * copies data to the shared variable and updated the timestamps and locks the variable while accessing
     * @param src source vector
     **/
    void set ( const std::vector<T> &src ) {
        lock();
        get()->resize ( src.size() );
        for ( size_t i = 0; i < src.size(); i++ ) {
            ( *get() ) [i] = src[i];
        }
        unlock();
        itHasChanged();
    }
    /** SAVE ACCESS :-) (the function will to the lock and the timstamp stuff)
     * copies data to the shared vector index and updated the timestamps and locks the variable while accessing
     * @param src source vector
     * @param n index
     **/
    void set ( const T &src, size_type n ) {
        lock();
        ( *get() ) [n] = src;
        unlock();
        itHasChanged();
    }
    /** SAVE ACCESS :-) (the function will to the lock and the timstamp stuff)
     *  copies data form the shared variable into a local varaiable and sets local timestamps and locks the variable while accessing
     * @param des
     * @return copy of the shared timestamp
     **/
    SystemClock::time_point get ( std::vector<T> &des ) {
        lock();
        SystemClock::time_point t = timestampShm();;
        des.resize ( get()->size() );
        for ( size_t i = 0; i < des.size(); i++ ) {
            des[i] = ( *get() ) [i];
        }
        unlock();
        updateTimestampLocal();
        return t;
    }
    /** SAVE ACCESS :-) (the function will to the lock and the timstamp stuff)
     *  copies data form the shared vetor index into a local varaiable and sets local timestamps and locks the variable while accessing
     * @param des
     * @return copy of the shared timestamp
     **/
    SystemClock::time_point get ( T &des, size_type n ) {
        lock();
        SystemClock::time_point t = timestampShm();;
        des = ( *get() ) [n];
        unlock();
        updateTimestampLocal();
        return t;
    }
    /** UNSAVE!! (user have to lock and to update timestamp)
     * @return vetor size
     **/
    size_t size() const {
        return get()->size();
    }
    /** UNSAVE!! (user have to lock and to update timestamp)
     * @return Returns true if the vector contains no elements
     **/
    bool empty() const {
        return get()->empty();
    }
    /** UNSAVE!! (user have to lock and to update timestamp)
     * Inserts or erases elements at the end such that the size becomes n. New elements are default constructed.
     **/
    void resize ( size_t n ) {
        get()->resize ( n );
    }
    /** UNSAVE!! (user have to lock and to update timestamp)
     * Inserts or erases elements at the end such that the size becomes n. New elements are default constructed.
     **/
    void resize ( size_t n, const T& v ) {
        get()->resize ( n, v );
    }
    /** UNSAVE!! (user have to lock and to update timestamp)
     * If n is less than or equal to capacity(), this call has no effect.
     * Otherwise, it is a request for allocation of additional memory.
     **/
    void reserve ( size_t n ) {
        get()->reserve ( n );
    }
    /** UNSAVE!! (user have to lock and to update timestamp)
     * Removes all elements from the vector (which are destroyed), leaving the container with a size of 0.
     **/
    void clear () {
        get()->clear ( );
    }
    /** UNSAVE!! (user have to lock and to update timestamp)
     * destroies the shared memory
     * @ToDo
     **/
    virtual void destroy() const {
        // header_local.shm_handler->getShm()->destroy_ptr(data_local.ptr);
        // Header::destroy();
        std::cerr << "vector::destroy() -> kown to have problem!" << std::endl;
    };
    /** UNSAVE!! (user have to lock and to update timestamp)
     *  @param o vector for comparison
     **/
    template<typename T1>
    bool operator == ( const T1 &o ) const {
        if ( size() != o.size() ) return false;
        return ( memcmp ( & ( *get() ) [0], &o[0], size() ) == 0 );
        //for ( size_t i = 0; i < size(); i++ ) if ( at ( i ) != o[i] ) return false;
        //return true;
    }
    /** UNSAVE!! (user have to lock and to update timestamp)
     *  @param o vector for comparison
     **/
    template<typename T1>
    bool operator != ( const T1 &o ) const {
        if ( size() != o.size() ) return true;
        return ( memcmp ( & ( *get() ) [0], &o[0], size() ) != 0 );
    }
    /**
     * overloads the << and calls the varalible overloades operator
     **/
    friend std::ostream &operator << ( std::ostream &os, const Vector<T> &o ) {
        for ( size_t i = 0; i < o.size(); i++ ) {
            os << ( i==0?"[":" " ) << o[i] << ( ( i<o.size()-1 ) ?", ":"]" );
        }
        return os;
    }
};

};
#endif //SHARED_MEM_VECTOR_H


