//
// Created by Victoria on 2018-04-04.
//

#include "ActionTable.h"

/**
 * Initialize hardcoded action table
 */
ActionTable::ActionTable() {
    std::set<Field> dnf_firewall_write_fields = { };
    std::set<Field> proxy_write_fields = { DIP, DPORT };
    std::set<Field> compressor_write_fields = { PAYLOAD };
    std::set<Field> dnf_lb_write_fields = { };
    std::set<Field> pktgen_write_fields = { };

    write_fields_map.insert(std::make_pair(NF::dnf_firewall, dnf_firewall_write_fields));
    write_fields_map.insert(std::make_pair(NF::proxy, proxy_write_fields));
    write_fields_map.insert(std::make_pair(NF::compressor, compressor_write_fields));
    write_fields_map.insert(std::make_pair(NF::dnf_loadbalancer, dnf_lb_write_fields));
    write_fields_map.insert(std::make_pair(NF::pktgen, pktgen_write_fields));
}

/**
 * @param nf    The NF to check
 * @return a list of all packet fields that the NF might write to
 */
std::set<Field> ActionTable::get_write_fields(NF nf) {
    return write_fields_map.at(nf);
}