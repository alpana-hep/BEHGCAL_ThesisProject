#ifndef Emulation_HGCalBufferModel_Connection_h
#define Emulation_HGCalBufferModel_Connection_h

/*
Safe data transfer between entities, where safe means no data can be
overwritten and the order in which the entites at either end are run
doesn't affect the data.

The structure is an input object, and output object and an intermediate
queue. A typical starting point is with the queue holding one object and
no valid data in either the input or output object

  Inp X         Q 1        Out X

The first step is the output entity calls strobeOutput(), which copies the
valid data from the Q to the output object

  Inp X         Q X        Out 1

The output entity is now run, using get() to read from the output object. It
is protected by a valid flag so it cannot be sampled more than once. Further
calls output entity calls to get() and its strobeOutput() have no effect
as there are no other data in the queue.

  Inp X         Q X        Out X

At some point the input entity is run. This uses set() to write to the
input object. 

  Inp 2         Q X        Out X

Again, the input object is protected by a valid flag which prevents it
from being overwritten. When the input entity completes, it calls
strobeInput() which copies the data from the input object into the queue.

  Inp X         Q 2        Out X

This is the same state as for the initial step.

Note, unlike the output entity, if the input entity is rerun it can write
to the input using set() again

  Inp 3         Q 2        Out X

and the subsequent strobeInput() will transfer the data to the queue.

  Inp X         Q 3,2      Out X

At this point, the output entity can get run strobeOutput() and then
call get() twice before it empties the connection.
 */


#include <iostream>
#include <queue>
#include <array>

template<typename T> class Connection {
 public:
 Connection() :
  inputValid_(false), //inputBlock_(false),
    outputValid_(false), //outputBlock_(false),
    maxSize_(0xffffffff) {
  }
  
  virtual ~Connection() {
  }

  void setMaxSizeTo(unsigned m) {
    maxSize_=m;
  }
  
  void initialiseQueue(const T &v, unsigned n=1) {
    for(unsigned i(0);i<n && queue_.size()<=maxSize_;i++) {
      queue_.push(v);
    }
  }
  
  bool set(const T &v) {
    //if(inputBlock_) return false;
    if(inputValid_) return false;
    inputValid_=true;
    inputValue_=v;
    return true;
  }
  
  bool get(T &v) {
    /*
    std::cout << "Connection::get() outputValid = "
	      << (outputValid_?"true":"false") << std::endl;
    */
    
    if(!outputValid_) return false;

    v=outputValue_;
    outputValid_=false; //
    //outputBlock_=false;
    return true;
  }
  
  unsigned strobeInput() {
    //inputBlock_=false;
    if(!inputValid_ || queue_.size()>=maxSize_) return queue_.size();

    queue_.push(inputValue_);
    inputValid_=false;
    return queue_.size();
  }
  
  unsigned strobeOutput() {
    if(outputValid_ || queue_.empty()) return queue_.size();

      //std::cout << "Connection::strobeOutput() outputValid = "
	//      << (outputValid_?"true":"false") << std::endl;

    outputValue_=queue_.front();
    queue_.pop();
    outputValid_=true;
    return queue_.size();
  }
  
  unsigned size() const {
    return queue_.size();
  }
  
  unsigned maxSize() const {
    return maxSize_;
  }
  
  bool empty() const {
    return queue_.empty();
  }
  
  bool full() const {
    return queue_.size()>=maxSize_;
  }
  
 private:
  bool inputValid_;
  T    inputValue_;
  //bool inputBlock_;
    
  bool outputValid_;
  T    outputValue_;
  //bool outputBlock_;
  
  unsigned maxSize_;
  std::queue<T> queue_;
};

typedef uint32_t Elink;
typedef std::array<Elink,2> ElinkPair;

class RamWord;
typedef Connection<RamWord> EventBuffer;

#endif
