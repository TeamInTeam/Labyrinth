#pragma once

#include <string>
#include <cstddef>
#include <iostream>
#include <odb/core.hxx>
#include "AuthenticationManager.h"

/**
 * @author Andrey Smirnov
 */

using std::string;

/**
 * Database maps
 */
#pragma db object table("maps")

class Map {
public:
    /**
     * @throw Not a rectangle error
     * @param name map unique name
     * @param map field
     */
    Map(const string &name, const std::vector <std::vector<int> > &map)
            : name(name),
              len(map.size()) {
        unsigned long inner_len = map[0].size();
        this->map.resize(len * inner_len);

        for (int j = 0; j < len; j++) {
            if (map[j].size() != inner_len) {
                throw std::length_error("not a rectangle");
            }
            for (int i = 0; i < inner_len; i++) {
                this->map[i + j * (len + 1)] = map[j][i];
            }
        }
        this->inner_len = inner_len;
    }

    // inline for ORM
    std::vector <std::vector<int> > get_map() {
        std::vector <std::vector<int> > res;
        res.resize(len);
        for (int j = 0; j < len; j++) {
            res[j].resize(inner_len);
            for (int i = 0; i < inner_len; i++) {
                res[j][i] = this->map[i + j * (len + 1)];
            }
        }

        return res;
    }

    string get_name() { return name; }

private:
    friend class odb::access;

    Map() {}  // [ORM]

#pragma db id auto  // PRIMARY_KEY
    unsigned long id;

#pragma db unique
    string name; // text
    unsigned long len; // bigint
    unsigned long inner_len; // bigint
    std::vector<int> map; // maps_map
};