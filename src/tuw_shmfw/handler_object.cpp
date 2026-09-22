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

#include <tuw_shmfw/shmfw.hpp>

#include <tuw_shmfw/variable.hpp>
#include <tuw_shmfw/vector.hpp>
#include <tuw_shmfw/header.hpp>

#include <tuw_shmfw/handler_object.hpp>
#include <tuw_shmfw/handler_variable.hpp>
#include <tuw_shmfw/handler_vector.hpp>

using namespace ShmFw;


HandlerObjectPtr HandlerObject::open ( const std::string &name, HandlerPtr &shmHdl ) {

    if ( shmHdl->findName ( name ) == NULL ) {
        std::cerr << "no shared variable with name: " << name << std::endl;
        throw 0;
    }
    ShmFw::Header shmHeader ( shmHdl, name );

#define RETURN_IF_TYPE_VAR( TYPE ) \
    if(shmHeader.isType<ShmFw::Var< TYPE > >()) {\
      return HandlerObjectPtr( new ShmFw::HandlerVar< TYPE >(name, shmHdl));\
    };
    RETURN_IF_TYPE_VAR ( float );
    RETURN_IF_TYPE_VAR ( double );
    RETURN_IF_TYPE_VAR ( bool );
    RETURN_IF_TYPE_VAR ( char );
    RETURN_IF_TYPE_VAR ( short );
    RETURN_IF_TYPE_VAR ( int );
    RETURN_IF_TYPE_VAR ( long );
    RETURN_IF_TYPE_VAR ( long long );
    RETURN_IF_TYPE_VAR ( unsigned char );
    RETURN_IF_TYPE_VAR ( unsigned short );
    RETURN_IF_TYPE_VAR ( unsigned int );
    RETURN_IF_TYPE_VAR ( unsigned long );
    RETURN_IF_TYPE_VAR ( unsigned long long );
    RETURN_IF_TYPE_VAR ( int8_t );
    RETURN_IF_TYPE_VAR ( int16_t );
    RETURN_IF_TYPE_VAR ( int32_t );
    RETURN_IF_TYPE_VAR ( int64_t );
    RETURN_IF_TYPE_VAR ( uint8_t );
    RETURN_IF_TYPE_VAR ( uint16_t );
    RETURN_IF_TYPE_VAR ( uint32_t );
    RETURN_IF_TYPE_VAR ( uint64_t );
    RETURN_IF_TYPE_VAR ( std::chrono::system_clock::time_point );


#define RETURN_IF_TYPE_VECTOR( TYPE ) if(shmHeader.isType<ShmFw::Vector< TYPE > >()) return HandlerObjectPtr( new ShmFw::HandlerVector< TYPE >( name, shmHdl));
    RETURN_IF_TYPE_VECTOR ( float );
    RETURN_IF_TYPE_VECTOR ( double );
    RETURN_IF_TYPE_VECTOR ( bool );
    RETURN_IF_TYPE_VECTOR ( char );
    RETURN_IF_TYPE_VECTOR ( short );
    RETURN_IF_TYPE_VECTOR ( int );
    RETURN_IF_TYPE_VECTOR ( long );
    RETURN_IF_TYPE_VECTOR ( long long );
    RETURN_IF_TYPE_VECTOR ( unsigned char );
    RETURN_IF_TYPE_VECTOR ( unsigned short );
    RETURN_IF_TYPE_VECTOR ( unsigned int );
    RETURN_IF_TYPE_VECTOR ( unsigned long );
    RETURN_IF_TYPE_VECTOR ( unsigned long long );
    RETURN_IF_TYPE_VECTOR ( std::chrono::system_clock::time_point );

    return HandlerObjectPtr();
}

#define CREATE_TYPE_VAR( TYPE ) if(boost::iequals(type, "TYPE")) return HandlerObjectPtr( new HandlerVar< TYPE >(name, shmHdl));

HandlerObjectPtr HandlerObject::create ( const std::string &name, HandlerPtr &shmHdl, const std::string &type ) {

    if ( shmHdl->findName ( name ) != NULL ) {
        std::cerr << "variable exists allready!" << name << std::endl;
        return open ( name, shmHdl );
    }

    CREATE_TYPE_VAR ( float );
    CREATE_TYPE_VAR ( double );
    CREATE_TYPE_VAR ( bool );
    CREATE_TYPE_VAR ( char );
    CREATE_TYPE_VAR ( short );
    CREATE_TYPE_VAR ( int );
    CREATE_TYPE_VAR ( long );
    CREATE_TYPE_VAR ( long long );
    CREATE_TYPE_VAR ( unsigned char );
    CREATE_TYPE_VAR ( unsigned short );
    CREATE_TYPE_VAR ( unsigned int );
    CREATE_TYPE_VAR ( unsigned long );
    CREATE_TYPE_VAR ( unsigned long long );
    return HandlerObjectPtr();
}
