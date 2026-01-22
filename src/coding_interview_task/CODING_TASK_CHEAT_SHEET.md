# Coding Task Cheat Sheet

## Rule of 5
```cpp
class MyClass{
public: 
    MyClass()=default;
    ~MyClass()=default;
    MyClass(const MyClass&other)=default;
    MyClass& operator=(const MyClass& other)=default;
    MyClass(MyClass&& other) noexcept=default;
    MyClass& operator=(MyClass&& other) noexcept=default;
};
```

## Typical syntaxes

accumulate:
```cpp
#include <numeric>
int start_value{};
std::accumulate(vec.begin(),vec.end(),start_value);
```

enable_if (for instance to restrict matrix multiplication implementation)
```cpp 
template <int NROW1, int NCOL1, int NROW2, int NCOL2, std::enable_if_t<NCOL1 == NROW2, int> = 0>
Matrix<NROW1, NCOL2> MatrixMultiplication(const Matrix<NROW1, NCOL1>& mat1, const Matrix<NROW2, NCOL2>& mat2) {
    ....
}
```

find: Find a value for the element after the given iterator to the end of the vector;
```cpp 
#include <algorithm>
std::find(std::next(it, 1), input.end(), value_to_search);
```

for: iterator based
```cpp
for (it=vec.begin();it!=vec.end();it=std::nex(it,1))
```

max_element:
```cpp 
#include <algorithm>
it=std::max_element(vec.begin(),vec.end());
```

numeric_limits:
```cpp 
#include <limits>
std::numeric_limits<int>::max()
```

sort:
```cpp 
#include <algorithm>
std::sort<vec.begin(),vec.end()>;
```

## Types
```cpp
#include<cstdint>
std::uint16_t fooobar{};
```

## Containers

### Map 

```cpp 
#include <map>
std::map<string,int> mymap;
mymap["foobar"]=2; //create in place if not existent.
```

### Bitset
```cpp
#include <bitset>
char c{2U};
std::bitset<1*8U> bits{c}; //
```

### Queue
```cpp
#include <queue>
std::queue<int> myqueue{};
queue.push(1);
queue.emplace(2);
one=queue.front();
queue.pop(); //Remember cant retrieve element with pop.
queue.size() //1
```

### String

```cpp 
#include <string>
string foobar{"foo"};
foobar.append("bar")
foobar.find("b");
if (foobar.find("z") == std::string::npos)
{
    //no z in string;
}
```

### Optional 
```cpp
#include <utility>
std::optional<int> opt_int{};
opt_int.has_value(); //==false;
opt_int=3;
opt_int.value(); //==3
*opt_int; //==3
```

### Set
```cpp
#include <set>
std::set<int> myset;
myset.insert(1);
myset.insert(1);
myset.insert(3);
myset.size(); //==2
myset.count(1); //=1
myset.count(3); //=1
myset.count(2); //=0
```

### Tuple 
```cpp
#include <tuple>
std::tuple<int,float,string> mytuple=std::make_tuple<int,float,string>(1,2.0F,"foo");
std::get<0>(mytuple); //==1
std::get<1>(mytuple); //==2.0F
int x;
std::string s;
std::tie(x,std::ignore,s)=mytuple; //x=1,s="foo"
auto cat=std::tuple_cat(mytuple,mytuple); // std::tuple<int,float,string,int,float,string>
```
