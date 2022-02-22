#include "helloworld.grpc.pb.h"

#include <memory>
#include <grpcpp/grpcpp.h>


using helloworld::Greeter;
using helloworld::HelloRequest;
using helloworld::HelloReply;

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

int main(){

    std::string server_address = "localhost:50051";

    std::shared_ptr<Channel> channel (CreateChannel(server_address, grpc::InsecureChannelCredentials()));
    std::unique_ptr<Greeter::Stub> stub_(Greeter::NewStub(channel));

    // Request 
    {
    HelloRequest request;
    request.set_name("User");

    HelloReply reply;

    ClientContext context;

    
    Status status = stub_->SayHello(&context,request,&reply);

    if (status.ok()) {
        std::cout << "Server sent: " << reply.message() << std::endl;
    } else {
        std::cout << status.error_code() << ": " << status.error_message() << std::endl;
    }
    } 


    return 0;
}