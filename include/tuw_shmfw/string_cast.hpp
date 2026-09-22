/***************************************************************************
 *   Software License Agreement (BSD License)                              *
 *   Copyright (C) 2024 by Markus Bader <markus.bader@tuwien.ac.at>        *
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

#ifndef SHARED_STRING_CAST_HPP
#define SHARED_STRING_CAST_HPP

#include <chrono>
#include <iostream>
#include <iomanip>
#include <string>

/**
 * Namespace for the fast and dynamic shared memory framework
 */
namespace ShmFw
{
    template <typename T>
    std::string to_string_default(const T &o)
    {
        std::stringstream ss;
        ss << o;
        return ss.str();
    }
    std::string to_string(const float &o);
    std::string to_string(const double &o);
    std::string to_string(const bool &o);
    std::string to_string(const char &o);
    std::string to_string(const signed char &o);
    std::string to_string(const short &o);
    std::string to_string(const int &o);
    std::string to_string(const long &o);
    std::string to_string(const long long &o);
    std::string to_string(const unsigned char &o);
    std::string to_string(const unsigned short &o);
    std::string to_string(const unsigned int &o);
    std::string to_string(const unsigned long &o);
    std::string to_string(const unsigned long long&o);
    std::string to_string(const std::chrono::system_clock::time_point &o);

    float &from_string(const std::string &str, float &des);
    double &from_string(const std::string &str, double &des);
    bool &from_string(const std::string &str, bool &des);
    char &from_string(const std::string &str, char &des);
    signed char &from_string(const std::string &str, signed char &des);
    short &from_string(const std::string &str, short &des);
    int &from_string(const std::string &str, int &des);
    long &from_string(const std::string &str, long &des);
    long long &from_string(const std::string &str, long long &des);
    unsigned char& from_string(const std::string &str, unsigned char& des);
    unsigned short& from_string(const std::string &str, unsigned short& des);
    unsigned int& from_string(const std::string &str, unsigned int& des);
    unsigned long& from_string(const std::string &str, unsigned long& des);
    unsigned long long& from_string(const std::string &str, unsigned long long& des);
    std::chrono::system_clock::time_point &from_string(const std::string &time_string, std::chrono::system_clock::time_point& des);
}
#endif // SHARED_STRING_CAST_HPP
