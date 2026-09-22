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

#ifndef SHARED_MEM_HANDLER_VECTOR
#define SHARED_MEM_HANDLER_VECTOR

#include <tuw_shmfw/handler_object.hpp>
#include <tuw_shmfw/vector.hpp>

namespace ShmFw {

template <class T>
class HandlerVector : public HandlerObject {
public:
private:
    ShmFw::Vector<T> v;
public:
    HandlerVector(const std::string &name, HandlerPtr &shmHdl) {
        v.construct (name, shmHdl);
    }
    std::string type_name () {
        return v.type_name();
    }
    virtual void it_has_changed() {
      v.itHasChanged();
    }
    virtual void lock() {
      v.try_lock();
    }
    virtual void unlock() {
      v.unlock();
    }
    virtual bool locked() {
      return v.locked();
    }
    virtual std::string timestamp() {
        return to_simple_string(v.timestampShm()) ;
    }
    std::string name () {
        return v.name();
    }
    std::string value() const {
      std::stringstream ss;
      ss << "[ ";
      for(size_t i = 0; i < v.size(); i++){
        if( i != 0) ss << "; ";
	      ss << v[i];
      }
      ss << " ]";
      return ss.str();
    }
    virtual std::string value(uint32_t i) const {
	    if( i > size()) return "Out of bound";
      return to_string(v[i]);
    }
    virtual uint32_t size() const {
        return v.size();
    }
    /**
     * @todo not jet finished
     **/
    void value(const std::string &str) {
      size_t start = str.find("[");
      if((start == std::string::npos)) return;
      size_t end = str.find_last_of("]");
      if((end <= start) || (end == std::string::npos)) return;
      std::string data = str.substr(start+1, end-1); 
      bool read_entries = true;
      start = 0;
      v.clear();
      T entry;
      while(read_entries){
        end = data.find(",", start);
        if( end == std::string::npos){
          v->push_back(from_string(data, entry) );
          read_entries = false;
        } else {
          std::string s = data.substr(start, end);
          v->push_back(from_string(s, entry) );
          data = data.substr(end+1);
        }
      }
    }
    int construct ( const std::string &name, Handler &shmHdl ){
      return v.construct(name, shmHdl);
    }
};


};



#endif //SHARED_MEM_HANDLER_VAR




