
// https://itanium-cxx-abi.github.io/cxx-abi/abi.html

extern "C" {

/**
* The __dso_handle is a global pointer to the current address where the next
* {destructor_ptr, object_ptr} pair will be stored. The main function of most
* bare metal applications is not supposed to return and global/static objects
* will not be destructed. In this case it will be enough to implement the
* required function the following way:
*/

// C library standard initialisation is not called, so __dso_handle address can be set to 0.

void* __dso_handle = nullptr;

} // extern C
