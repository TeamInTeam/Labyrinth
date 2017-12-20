#include <odb/database.hxx>
#include "PrivateData.h"
#include <odb/pgsql/database.hxx>
#include <boost/algorithm/string/case_conv.hpp>
#include "map.hxx"
#include "map-odb.hxx"
#include "MapProvider.h"

/**
 * @author Andrey Smirnov
 */

using namespace odb::core;
using namespace odb;
using namespace model;

MapProvider::MapProvider() {
    this->db = std::tr1::shared_ptr<database>(new odb::pgsql::database(
            PrivateData::DB_USER,
            PrivateData::USER_PASSWD,
            PrivateData::DB_NAME));
}

/**
 * Returns map by its name
 * @param name map's unique name
 * @return map or 0, in case of failure
 */
std::tr1::shared_ptr<Map> MapProvider::get_map_by_name(const std::string &name) {
    typedef odb::query<Map> Query;
    typedef odb::result<Map> Result;

    transaction t(db->begin());

    std::tr1::shared_ptr<Map> map(db->query_one<Map>(Query::name == boost::algorithm::to_lower_copy(name)));

    return map;
}

/**
 * Imports the map to the database
 * @param map map to import
 * @return true if operation succeeded; otherwise - false
 */
bool MapProvider::import_map(std::tr1::shared_ptr<Map> map) {
    try {
        transaction t(db->begin());
        Map new_map(boost::algorithm::to_lower_copy(map->get_name()), map->get_map());
        db->persist(new_map);
        t.commit();
    } catch (...) {
        return false;
    }

    return true;
}

