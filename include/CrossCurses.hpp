#include "CrossCursesBase.hpp"

#ifdef _WIN32
#include "WindowsImpl.hpp"
#else
#include "LinuxImpl.hpp"
#endif // _WIN32
