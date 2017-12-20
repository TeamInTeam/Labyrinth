#pragma once

#include <vector>
#include <string>
#include <odb/core.hxx>
#include <boost/tr1/memory.hpp>

/**
 * @author Andrey Smirnov
 */

namespace model {
    using std::vector;
    using std::string;

    /**
     * MapProvider class is responsible for importing and exporting game fields
     */
    class MapProvider {
    public:
        MapProvider();

        /**
         * Get map
         * @param name unique map name
         * @return pointer to Map
         */
        std::tr1::shared_ptr <Map> get_map_by_name(const std::string &name);

        /**
         * Imports map to the database
         * @param map map to import
         * @return true if the operation succeeded; otherwise - false
         */
        bool import_map(std::tr1::shared_ptr <Map> map);

    private:
        std::tr1::shared_ptr <odb::core::database> db;
    };
}