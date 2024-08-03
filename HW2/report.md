## Environment
https://medium.com/@jackercleaninglab/dockerfile-%E5%BB%BA%E7%AB%8B%E8%87%AA%E8%A8%82%E6%98%A0%E5%83%8F%E6%AA%94-%E6%9E%B6%E8%B5%B7%E7%B6%B2%E7%AB%99%E5%BF%AB%E9%80%9F%E5%8F%88%E7%B0%A1%E5%96%AE-%E4%B8%80-22b2743f97b9

```shell
# build the image
docker build -t Nana2929/aut-ap-hw2 .
# run the container
docker run -d -p 3000:3000 -v ${PWD}:/usr/src/app Nana2929/aut-ap-hw2
# check container id
docker ps
# enter the container
docker exec -it {id} bash
```

## Questions along the way
- How to validate a const member? [StackOverflow](https://stackoverflow.com/questions/53498536/how-to-validate-the-initialization-of-a-const-member-variable-before-assigning-i)
  - `const` member needs to be initialized in the constructor initializer list
  - What if this `const` member has certain validation rules?
```cpp
    class A {
    public:
        A(int x) : x(x) {
            if (!isValid(x)) {
                throw std::invalid_argument("Invalid x");
            }
        }
        bool isValid(int x) {
            return x > 0;
        }
    private:
        const int x;
    };
```