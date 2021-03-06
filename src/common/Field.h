#ifndef DEEPNF_FIELD_H
#define DEEPNF_FIELD_H

#include <string>

enum Field {
    SIP = 0,
    DIP,
    SPORT,
    DPORT,
    PAYLOAD,
};

namespace field
{
    /**
     * @param nf    The Field to convert
     * @return A string representation of the input Field
     */
    std::string field_to_string(Field field);
}

#endif