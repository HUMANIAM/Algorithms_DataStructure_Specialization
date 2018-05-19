#include <iostream>
#include <stack>
#include <string>

struct Bracket {
    Bracket(char type, int position):
        type(type),
        position(position)
    {}

    bool Matchc(char c) {
        if (type == '[' && c == ']')
            return true;
        if (type == '{' && c == '}')
            return true;
        if (type == '(' && c == ')')
            return true;
        return false;
    }

    char type;
    int position;
};

int main() {
	int errorposition = 0;
    std::string text;
    getline(std::cin, text);

    std::stack <Bracket> opening_brackets_stack;
    for (int position = 0; position < text.length(); ++position) {
        char next = text[position];

        if (next == '(' || next == '[' || next == '{') {
            // Process opening bracket, write your code here
            opening_brackets_stack.push(Bracket(next, position));

        }

        if (next == ')' || next == ']' || next == '}') {
            // Process closing bracket, write your code here
            if(!opening_brackets_stack.empty() && (opening_brackets_stack.top()).Matchc(next)){
                opening_brackets_stack.pop();
            }
            else{
            	errorposition = position + 1;
                break;
            }
        }
    }

    // Printing answer, write your code here
    if(errorposition == 0){
    if(opening_brackets_stack.empty()){
        std::cout<<"Success";
    }
    else{
            while(opening_brackets_stack.size()>1){
                opening_brackets_stack.pop();
            }
            std::cout<<(opening_brackets_stack.top()).position + 1<<std::endl;
    }
    }
    else{
    	std::cout<<errorposition<<std::endl;
    }

    return 0;
}
