#include <utility/assertion.h>

#include <iomanip>

std::chrono::nanoseconds TimeFromStart() {
    static auto start = std::chrono::system_clock::now();
    return std::chrono::system_clock::now() - start;
}

std::ostream& PrintTime(std::ostream& os, std::chrono::nanoseconds ns)
{
    using namespace std;
    using namespace std::chrono;

    char fill = os.fill();
    os.fill('0');
    auto h = duration_cast<hours>(ns);
    ns -= h;
    auto m = duration_cast<minutes>(ns);
    ns -= m;
    auto s = duration_cast<seconds>(ns);
    ns -= s;
    auto ms = duration_cast<milliseconds>(ns);

    bool ph = h.count();
    
    if (ph)
        os << setw(2) << h.count() << "h:";
    
    os << setw(2) << m.count() << "m:";
    os << setw(2) << s.count() << "s:";
    os << setw(4) << ms.count() << "ms";

    os.fill(fill);
    return os;
};