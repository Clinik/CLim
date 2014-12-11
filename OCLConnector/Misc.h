#ifndef MISC_H
#define MISC_H

#include <vector>

const char* oclErrorString(cl_int error);	//... Hiba2String

class NDRange {
	std::vector<size_t> sizes;
	size_t dims;

public:
	NDRange() {}

	NDRange(size_t size0) : dims(1) {
		sizes.push_back(size0);
	}

	NDRange(size_t size0, size_t size1) : dims(2) {
		sizes.push_back(size0);
		sizes.push_back(size1);
	}

	NDRange(size_t size0, size_t size1, size_t size2) : dims(3) {
		sizes.push_back(size0);
		sizes.push_back(size1);
		sizes.push_back(size2);
	}

	size_t getDims() {
		return dims;
	}

	size_t getSize(int index) const {
		if (index > dims)
			return 1;

		return sizes[index];
	}
};


#endif
