
#include <sstream>
#include <iostream>
#include <string>
// g++ -o outs/stream /home/nanaeilish/extra/aut_ap_2024/lecture_demos/stream.cpp


// *string to integer function*
int stringToInt(const std::string& str){
    std::istringstream iss(str); // turning the string into a stream
    int res;
    iss >> res; // extracting the integer from the stream
    if (iss.fail()) throw std::invalid_argument("stringToInt: converting to int failed");
    char remain; // to check if this str looks sth like `234a`, in which the remaining `a` is not a digit
    iss >> remain;
    if (!iss.fail()) throw std::invalid_argument("stringToInt: has trailing characters");
    return res;
};


int getInteger(const std::string& prompt, const std::string& reprompt){
    while (true){
        std::cout << prompt;
        std::string str;
        std::getline(std::cin, str);
        try{
            return stringToInt(str);
        } catch (const std::invalid_argument& e){
            std::cout << e.what() << std::endl;
            std::cout << reprompt;
        }
    }

}


int main(){
    // output string stream
    std::ostringstream oss("Ito En green tea ");
    std::cout << oss.str() << std::endl;

    // writing to osstream
    // WHY:
    // pointer to the string buffer is returned
    // so when we write to the stream, we are writing to the buffer and replacing the original string
    oss << 16.9 << " ounces is the best!";
    std::cout << oss.str() << std::endl;  // 16.9 ounces is the best!
    // note that `16.9` is a `double` and it is converted to a string
    oss << " You can try it at your local grocery store!";
    std::cout << oss.str() << std::endl;  // 16.9 ounces is the best! You can try it at your local grocery store!


    // WHAT IF we want to append to the back of the string?
    // use `ate`: std::ios_base::ate 打開文件後，文件指針移到文件末尾
    std::ostringstream oss2("Ito En green tea ", std::ios_base::ate);
    oss2 << 16.9 << " ounces is the best!";
    // note that
    // " ounces is the best!" is a C-string
    // std::string(" ounces is the best!") is a C++ string
    std::cout << oss2.str() << std::endl;  // Ito En green tea 16.9 ounces is the best!

    // WHAT IF we directly do
    // std::cout << oss2 << std::endl;  // an error, no matching operator<<


    const std::string str = "123";
    std::cout << stringToInt(str) << std::endl;  // 123
    try{
        std::cout <<  stringToInt("123a") << std::endl;  // stringToInt: has trailing characters
    } catch (const std::invalid_argument& e){
        std::cout << e.what() << std::endl;
    }
    try {
        std::cout << stringToInt("a123") << std::endl;  // stringToInt: converting to int failed
    } catch (const std::invalid_argument& e){
        std::cout << e.what() << std::endl;
    }
    return 0;

}