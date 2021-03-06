//
// Created by Victoria on 2018-04-04.
//

#ifndef DEEPNF_MERGEROPERATOR_H
#define DEEPNF_MERGEROPERATOR_H

#include <arpa/inet.h>
#include <errno.h>
#include <map>
#include <set>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <sys/socket.h>
#include <vector>
#include <pthread.h>
#include <fstream>

#include <runtime/packet.h>
#include <runtime/socket_util.h>
#include <runtime/address.h>
#include <common/RuntimeNode.h>
#include <common/Field.h>
#include <runtime/address_util.h>
#include <runtime/log_util.h>

#include "ActionTable.h"
#include "MergerInfo.h"



class MergerOperator {

public:
    typedef struct packet_info
    {
        int node_id;
        packet* pkt;
        /* list of all fields of the packet that should be considered "written to" during the merging
         * process */
        std::set<Field>* written_fields;
    } PACKET_INFO;

    MergerOperator();
    MergerOperator(MergerInfo* mi);

    /**
     * Setup MergerOperator to start listening and merging packets
     */
    void run();

    /**
     * Listens on the given port for packets from the given runtime_id, merging as necessary
     *
     * @param port          The port to listen for packets from runtime_id
     * @param node_id    The id of the runtime node leaf
     */
    void run_node_thread(int port, int node_id);

    static void* run_node_thread_wrapper(void *arg);
    static void* merge_packet_wrapper(void *arg);

private:

    // contains information about virtual network interfaces, conflicting NF pairs, etc.
    MergerInfo* merger_info;
    // the total number of leaf runtime nodes that merger should listen for
    int num_nodes;

    // address to send merged packets to
    address* dest_address;

    // contains information about the NF action table
    ActionTable* action_table;

    /* for each packet id, map each runtime node id to the body of the packet is received with that
     * packet id (if such a packet has been received) */
    std::map<int, std::map<int, packet*>*> packet_map;
    pthread_mutex_t packet_map_mutex;

    // used to log merger activity
    int pkt_count;
    std::ofstream log;
    pthread_mutex_t log_mutex;

    /**
     * Given two packets (one with precedence over the another), return a merged packet that merges all written fields
     * in the two packets, resolving write conflicts appriopriately
     *
     * @param major_p       Info corresponding to the packet with precedence
     * @param minor_p       Info corresponding to the packet without precedence
     * @param conflict      The ConflictItem describing the conflict between the major and minor packets
     * @return  A merged packet containing both major_p and minor_p's writes
     */
    PACKET_INFO* resolve_packet_conflict(PACKET_INFO* major_p, PACKET_INFO* minor_p, ConflictItem* conflict);

    /**
     * Retrieves all packets for the given pkt_id stored in packet_map and outputs a merged packet
     * based on the conflict items in merger_info
     *
     * @param pkt_id    The id of the packet to merge
     * @return Pointer to a packet with all changes merged
     */
    packet* merge_packet(int pkt_id);

    /**
     * Merges all received packets for the given pkt_id, then sends merged packet to receive address
     *
     * @param pkt_id    The id of the packet to merge
     */
    void run_merge_packet(int pkt_id);

    /**
     * Converts a map from node ids to packets to a map from node ids to equivalent packet_infos
     * @return A map of runtime node ids to a packet_info struct equivalent to the node's
     *         packet in packet_map
     */
    std::map<int, PACKET_INFO*>* packet_map_to_packet_info_map(std::map<int, packet*>* packet_map);


    /**
     * Encapsulates the given packet into a packet_info struct
     *
     * @param packet    The packet to encapsulate
     * @param node_id   The id of the runtime node that send the input packet
     * @return Packet_info struct encapsulating the input packet instance
     */
    PACKET_INFO* packet_to_packet_info(packet* packet, int node_id);

    /**
     * Helper function that prints the contents of packet_map to stdout
     */
    void print_packet_map();
};



#endif //DEEPNF_MERGEROPERATOR_H
