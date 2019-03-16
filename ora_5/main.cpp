#include "vector2.h"

int main(int, char**) {
    Vector2<double> v1={1.1, 2.1};
    Vector2<double> v2={1.0, 2.0};
    Vector2<double> v3{};
    double s=2.0;

    std::cout << std::fixed;
    std::cout.precision(4);

    std::cout << "\n---Test of '<<' operator---\n";
    std::cout << "v1 is: " << v1 << '\n';
    std::cout << "v2 is: " << v2 << '\n';

    std::cout << "\n---Test of '+=' operator---\n";
    std::cout << "After v1+=v2\n";
    v1+=v2;
    std::cout << "v1 is: " << v1 << '\n';

    std::cout << "\n---Test of '-=' operator---\n";
    std::cout << "After v1-=v2\n";
    v1-=v2;
    std::cout << "v1 is: " << v1 << '\n';

    std::cout << "\n---Test of '*=' operator---\n";
    std::cout << "After v1*=" << s << '\n';
    v1*=s;
    std::cout << "v1 is: " << v1 << '\n';

    std::cout << "\n---Test of '/=' operator---\n";
    std::cout << "After v1/=" << s << '\n';
    v1/=s;
    std::cout << "v1 is: " << v1 << '\n';

    std::cout << "\n---Test of '+' operator---\n";
    std::cout << "v1+v2 is: " << v1+v2 << '\n';

    std::cout << "\n---Test of '-' operator---\n";
    std::cout << "v1-v2 is: " << v1-v2 << '\n';

    std::cout << "\n---Test of '*' operator---\n";
    std::cout << "v1*" << s << " is: " << v1*s << '\n';
    std::cout << s << "*v1 is: " << s*v1 << '\n';

    std::cout << "\n---Test of '/' operator---\n";
    std::cout << "v1/" << s << " is: " << v1/s << '\n';

    std::cout << "\n---Test of 'dot()' function---\n";
    std::cout << "dot(v1, v2) is:" << dot(v1, v2) << '\n';

    std::cout << "\n---Test of 'sqlength()' function---\n";
    std::cout << "sqlength(v2) is:" << sqlength(v2) << '\n';

    std::cout << "\n---Test of 'length()' function---\n";
    std::cout << "length(v2) is:" << length(v2) << '\n';

    std::cout << "\n---Test of 'normalize()' function---\n";
    std::cout << "normalize(v2) is: " << normalize(v2) << '\n';
    std::cout << "normalize(Vector2<int>{0,0}) is: " << normalize(Vector2<int>{0,0}) <<'\n';

    std::cout << "\n---Test of '>>' operator---\n";
    std::cout << "Vector2<double> v3=\n";
    std::cin >> v3.x;
    std::cin >> v3.y;
    std::cout << "v3 is: " << v3;

    return 0;
}
