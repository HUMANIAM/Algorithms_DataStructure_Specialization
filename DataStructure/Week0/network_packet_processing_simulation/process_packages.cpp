#include <iostream>
#include <queue>
#include <vector>
using namespace std;

//really it is very nice design and pure thinking i love it
struct Request {
    Request(int arrival_time, int process_time):
        arrival_time(arrival_time),
        process_time(process_time)
    {}

    int arrival_time;
    int process_time;
};

struct Response {
    Response(bool dropped, int start_time):
        dropped(dropped),
        start_time(start_time)
    {}

    bool dropped;
    int start_time;
};

class Buffer {
public:
    Buffer(int size):
        size_(size), finishTime(0),
        finish_time_()
    {}

    Response Process(const Request &request) {
        Response response(true, -1);
        if(size_ > 0){
            if(finishTime < request.arrival_time){
                response.dropped = false;
                response.start_time = request.arrival_time;
                finishTime = request.arrival_time + request.process_time;
            }
            else{
                response.dropped = false;
                response.start_time = finishTime;
                finishTime += request.process_time;
            }
            finish_time_.push(finishTime);
            size_--;
        }
        else if(finish_time_.front() <= request.arrival_time){
            while(!finish_time_.empty()){
                if(finish_time_.front() < request.arrival_time){
                    finish_time_.pop();
                    size_++;
                }
                else if(finish_time_.front() == request.arrival_time){
                    response.dropped = false;
                    response.start_time = finishTime;
                    finishTime += request.process_time;
                    finish_time_.pop();
                    finish_time_.push(finishTime);
                    break;
                }
                else{
                    response.dropped = false;
                    response.start_time = finishTime;
                    finishTime += request.process_time;
                    finish_time_.push(finishTime);
                    size_--;
                    break;
                }
            }

            if(finish_time_.empty()){
                response.dropped = false;
                response.start_time = request.arrival_time;
                finishTime = request.arrival_time + request.process_time;
                finish_time_.push(finishTime);
            }
        }
        else{
        }
        return response;
    }
private:
    int size_;
    int finishTime;
    std::queue <int> finish_time_;
};

std::vector <Request> ReadRequests() {
    std::vector <Request> requests;
    int count;
    std::cin >> count;
    for (int i = 0; i < count; ++i) {
        int arrival_time, process_time;
        std::cin >> arrival_time >> process_time;
        requests.push_back(Request(arrival_time, process_time));
    }
    return requests;
}

std::vector <Response> ProcessRequests(const std::vector <Request> &requests, Buffer *buffer) {
    std::vector <Response> responses;
    for (size_t i = 0; i < requests.size(); ++i)
        responses.push_back(buffer->Process(requests[i]));
    return responses;
}

void PrintResponses(const std::vector <Response> &responses) {
    for (size_t i = 0; i < responses.size(); ++i)
        std::cout << (responses[i].dropped ? -1 : responses[i].start_time) << std::endl;
}

int main() {
    int size;
    std::cin >> size;
    std::vector <Request> requests = ReadRequests();

    Buffer buffer(size);
    std::vector <Response> responses = ProcessRequests(requests, &buffer);

    PrintResponses(responses);
    return 0;
}
