#include "assertion.h"

#include <iomanip>

std::chrono::nanoseconds TimeFromStart() {
    static auto start = std::chrono::system_clock::now();
    return std::chrono::system_clock::now() - start;
}

std::ostream& PrintTime(std::ostream& os, std::chrono::nanoseconds ns)
{
    using namespace std;
    using namespace std::chrono;
    typedef duration<int, ratio<86400>> days;
    char fill = os.fill();
    os.fill('0');
    auto h = duration_cast<hours>(ns);
    ns -= h;
    auto m = duration_cast<minutes>(ns);
    ns -= m;
    auto s = duration_cast<seconds>(ns);
    ns -= s;
    auto ms = duration_cast<milliseconds>(ns);
    os << setw(2) << h.count() << "h:"
        << setw(2) << m.count() << "m:"
        << setw(2) << s.count() << "s:"
        << setw(4) << ms.count() << "ms";
    os.fill(fill);
    return os;
};