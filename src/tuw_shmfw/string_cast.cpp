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

#include <iostream>
#include <tuw_shmfw/string_cast.hpp>

namespace ShmFw
{

    std::string to_string(const float &o)
    {
        return std::to_string(o);
    }
    std::string to_string(const double &o)
    {
        return std::to_string(o);
    }
    std::string to_string(const bool &o)
    {
        return (o ? "true" : "false");
    }
    std::string to_string(const char &o)
    {
        return std::to_string(o);
    }
    std::string to_string(const signed char &o)
    {
        return std::to_string(o);
    }
    std::string to_string(const short &o)
    {
        return std::to_string(o);
    }
    std::string to_string(const int &o)
    {
        return std::to_string(o);
    }
    std::string to_string(const long &o)
    {
        return std::to_string(o);
    }
    std::string to_string(const long long &o)
    {
        return std::to_string(o);
    }
    std::string to_string(const unsigned char &o)
    {
        return std::to_string(o);
    }
    std::string to_string(const unsigned short &o)
    {
        return std::to_string(o);
    }
    std::string to_string(const unsigned int &o)
    {
        return std::to_string(o);
    }
    std::string to_string(const unsigned long &o)
    {
        return std::to_string(o);
    }
    std::string to_string(const unsigned long long &o)
    {
        return std::to_string(o);
    }

    std::string to_string(const std::chrono::system_clock::time_point &o){
        std::time_t time = std::chrono::system_clock::to_time_t(o);
        char buffer[80];
        std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", std::localtime(&time));
        return std::string(buffer);
    }

    float &from_string(const std::string &str, float &des)
    {
        des = std::stof(str);
        return des;
    }
    double &from_string(const std::string &str, double &des)
    {
        des = std::stod(str);
        return des;
    }
    bool &from_string(const std::string &str, bool &des)
    {
        if (str.compare("true"))
            des = true;
        else if (str.compare("True"))
            des = true;
        else if (str.compare("TRUE"))
            des = true;
        else if (str.compare("1"))
            des = true;
        else if (str.compare("false"))
            des = false;
        else if (str.compare("False"))
            des = false;
        else if (str.compare("FALSE"))
            des = false;
        else if (str.compare("0"))
            des = false;
        return des;
    }
    signed char &from_string(const std::string &str, signed char &des)
    {
        des = std::stoi(str);
        return des;
    }
    char &from_string(const std::string &str, char &des)
    {
        if (str.length() > 0)
            des = str.at(0);
        return des;
    }
    short &from_string(const std::string &str, short &des)
    {
        des = std::stoi(str);
        return des;
    }
    int &from_string(const std::string &str, int &des)
    {
        des = std::stoi(str);
        return des;
    }
    long &from_string(const std::string &str, long &des)
    {
        des = std::stol(str);
        return des;
    }
    long long &from_string(const std::string &str, long long &des)
    {
        des = std::stoll(str);
        return des;
    }
    unsigned char &from_string(const std::string &str, unsigned char &des)
    {
        if (str.length() > 0)
            des = str.at(0);
        return des;
    }
    unsigned short &from_string(const std::string &str, unsigned short &des)
    {
        des = std::stoi(str);
        return des;
    }
    unsigned int &from_string(const std::string &str, unsigned int &des)
    {
        des = std::stoll(str);
        return des;
    }
    unsigned long &from_string(const std::string &str, unsigned long &des)
    {
        des = std::stoll(str);
        return des;
    }
    unsigned long long &from_string(const std::string &str, unsigned long long &des)
    {
        des = std::stoll(str);
        return des;
    }
    std::chrono::system_clock::time_point &from_string(const std::string &time_string, std::chrono::system_clock::time_point &des)
    {
        std::tm tm = {};
        std::istringstream ss(time_string);
        ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
        if (ss.fail())
        {
            throw std::runtime_error("Failed to parse date and time from string");
        }
        std::time_t time = std::mktime(&tm);
        des = std::chrono::system_clock::from_time_t(time);
        return des;
    }

}