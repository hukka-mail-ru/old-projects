#ifndef __IPCONFIG_HPP__
#define __IPCONFIG_HPP__

#include <vector>
#include <string>
#include <iostream>

#include <boost/shared_ptr.hpp>

struct Interface
{
    std::string mName;
    std::string mAddress;
    std::string mMask;
    std::string mMacAddress;
};



class NetInterfaces
{
    public:

	NetInterfaces(): mCurrent(0) {}

	int query();

	Interface* getFirst()
	{
	    mCurrent = 0;
	    return mInterfaces[mCurrent].get();
	}
	Interface* getNext()
	{
	    mCurrent++;
	    return mInterfaces[mCurrent].get();
	}
	
    private:

	std::vector<boost::shared_ptr<Interface> > mInterfaces;
        int mCurrent; // counter
};

#endif
