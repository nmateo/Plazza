#include <string>
#include <stack>
#include <bits/stdc++.h>

std::list<std::string> splitStr(std::string str, char del)
{
    std::list<std::string> list;
    std::string sub = "";
    size_t strSize = str.size();

    for (size_t i = 0; i < strSize + 1; i++)
    {
        if (str[i] != del && str[i] != '\0')
            sub += str[i];
        else
        {
            if (sub.size() != 0)
                list.push_back(sub);
            sub = "";
        }
    }
    return list;
}
