#include "functions.h"

/*std::vector<std::string> explode(const std::string &delimiter, const std::string &str)
{
    std::vector<std::string> arr;

    int strleng = str.length();
    int delleng = delimiter.length();
    if (delleng==0)
        return arr;//no change

    int i=0;
    int k=0;
    while( i<strleng )
    {
        int j=0;
        while (i+j<strleng && j<delleng && str[i+j]==delimiter[j])
            j++;
        if (j==delleng)//found delimiter
        {
            arr.push_back(  str.substr(k, i-k) );
            i+=delleng;
            k=i;
        }
        else
        {
            i++;
        }
    }
    arr.push_back(  str.substr(k, i-k) );
    return arr;
}

template<typename T>
bool contains(std::vector<T> v, T x)
{
      if (v.empty())
           return false;
      if (find(v.begin(), v.end(), x) != v.end())
           return true;
      else
           return false;
}

template<typename T>
bool all_in_vec(std::vector<T> v1, std::vector<T> v2)
{
      if(v1.empty() || v2.empty()) return false;
      for(unsigned int i = 0; i < v1.size(); i++)
      {
          if(!contains(v2, v1[i])) return false;
      }
      return true;
}*/
