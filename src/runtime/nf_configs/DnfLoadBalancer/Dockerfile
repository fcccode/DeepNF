FROM ubuntu:latest

MAINTAINER Akash Subramanian <skythelimit.96@gmail.com>

RUN apt-get update -y && \
    apt-get upgrade -y && \
    apt-get install -y g++ libpcap-dev

COPY dnf_loadbalancer.cpp .
COPY packet.h .
COPY packet.cpp .
COPY socket_util.h .
COPY socket_util.cpp .
COPY address_util.h .
COPY address_util.cpp .
COPY address.h .
COPY log_util.h .
COPY log_util.cpp .

RUN g++ -std=c++11 -c address.h address_util.h address_util.cpp && \
	g++ -std=c++11 -c packet.h packet.cpp && \
	g++ -std=c++11 -c packet.cpp log_util.h log_util.cpp && \
	g++ -std=c++11 -c packet.cpp socket_util.h socket_util.cpp && \
	g++ -std=c++11 packet.cpp log_util.cpp address_util.cpp socket_util.cpp dnf_loadbalancer.cpp -o fw

RUN mkdir log
RUN touch log/log.txt