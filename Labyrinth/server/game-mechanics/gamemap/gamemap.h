/* Copyright (C) 2017 Mikhail Masyagin */

#ifndef GAMEMAP_H_
#define GAMEMAP_H_

#include <boost/variant.hpp>
#include <vector>
#include <tuple>
#include <utility>
#include <cstdlib>
#include <ctime>
#include "fields/fields.h"

namespace game_map_visitor {
    struct GetType : boost::static_visitor<FieldTypes> {
        template <typename T>
        FieldTypes operator() (const T& field) const {
            return field.type_of_field;
        }
    };

    struct WatchItems : boost::static_visitor<std::tuple<bool, int32_t, int32_t, int32_t>> {
        std::tuple<bool, int32_t, int32_t, int32_t> operator() (Wall&) const {
            return std::make_tuple(false, 0, 0, 0);
        }

        std::tuple<bool, int32_t, int32_t, int32_t> operator() (Concrete&) const {
            return std::make_tuple(false, 0, 0, 0);
        }
        
        template <typename T>
        std::tuple<bool, int32_t, int32_t, int32_t> operator() (T &field) const {
            return std::make_tuple(field.has_treasure, field.bombs, field.ammo, field.aids);
        }
    };

    struct PushItems : boost::static_visitor<> {
        void operator() (Wall&, std::tuple<bool, int32_t, int32_t, int32_t>) const {
            return;
        }

        void operator() (Concrete&, std::tuple<bool, int32_t, int32_t, int32_t>) const {
            return;
        }

        template <typename T>
        void operator() (T &field, std::tuple<bool, int32_t, int32_t, int32_t> tp) const {
            field.has_treasure = std::get<0>(tp);
            field.bombs = std::get<1>(tp);
            field.ammo = std::get<2>(tp);
            field.aids =  std::get<3>(tp);
        }
    };

    struct IsDestroyable : boost::static_visitor<bool> {
        bool operator() (const Wall& field) const {
            return field.is_destroyable;
        }
        
        bool operator() (const Concrete&) const {
            return true;
        }

        template <typename T>
        bool operator() (const T&) const {
            return false;
        }
    };

    struct GetRiverCoordinates : boost::static_visitor<std::vector<std::pair<int32_t, int32_t>>> {
        std::vector<std::pair<int32_t, int32_t>> operator() (const River &field) const {
            return field.coordinates;
        }
        
        template<typename T>
        std::vector<std::pair<int32_t, int32_t>> operator() (const T&) const {
            std::vector<std::pair<int32_t, int32_t>> result;
            return result;
        }
    };

    struct GetTeleportCoordinates : boost::static_visitor<std::pair<int32_t, int32_t>> {
        std::pair<int32_t, int32_t> operator() (const Metro &field) const {
            return std::make_pair(field.teleport_m, field.teleport_n);
        }
        
        template<typename T>
        std::pair<int32_t, int32_t> operator() (const T&) const {
            return std::make_pair(-1, -1);
        }
    };

    struct GetWeapon : boost::static_visitor<std::tuple<bool, bool, bool>> {
        std::tuple<bool, bool, bool> operator() (Armoury &field) const {
            std::srand(unsigned(std::time(0)));
            int32_t rnd = std::rand() % 3;
            if (rnd == 0) {
                if (field.my_bombs > 0) {
                    field.my_bombs--;
                    return std::make_tuple(true, false, false);
                } else if (field.my_ammo > 0) {
                    field.my_ammo--;
                    return std::make_tuple(false, true, false);
                } else if (field.my_concrete > 0) {
                    field.my_concrete--;
                    return std::make_tuple(false, false, true);
                }
            } else if (rnd == 1) {
                if (field.my_ammo > 0) {
                    field.my_ammo--;
                    return std::make_tuple(false, true, false);
                } else if (field.my_bombs > 0) {
                    field.my_bombs--;
                    return std::make_tuple(true, false, false);
                } else if (field.my_concrete > 0) {
                    field.my_concrete--;
                    return std::make_tuple(false, false, true);
                }
            } else {
                if (field.my_concrete > 0) {
                    field.my_concrete--;
                    return std::make_tuple(false, false, true);
                } else if (field.my_bombs > 0) {
                    field.my_bombs--;
                    return std::make_tuple(true, false, false);
                } else if (field.my_ammo > 0) {
                    field.my_ammo--;
                    return std::make_tuple(false, true, false);
                }
            }
            return std::make_tuple(false, false, false);
        }
        
        template<typename T>
        std::tuple<bool, bool, bool> operator() (T&) const {
            return std::make_tuple(false, false, false);
        }
    };
    
    struct GetMedicine : boost::static_visitor<bool> {
        bool operator() (Hospital &hospital) const {
            if (hospital.my_aids > 0) {
                hospital.my_aids--;
                return true;
            } else {
                return false;
            }
        }

        template<typename T>
        bool operator() (T&) const {
            return false;
        }
    };
}

class GameMap {
 public:
    /*
      Правила построения карты:
      0 - Road,
      1 - Wall,
      2 - River,
      x3 - Metro,
      4 - Armoury,
      5 - Hospital,
      6 - Bomb,
      7 - Concrete.
     */
    explicit GameMap(std::vector<std::vector<int32_t> > game_map_scheme);
    GameMap() = default;

    std::pair<int32_t, int32_t> GetSize();

    FieldTypes GetType(int32_t m, int32_t n);

    void ChangeType(int32_t m, int32_t n, FieldTypes type_of_field);

    std::tuple<bool, int32_t, int32_t, int32_t> WatchItems(int32_t m, int32_t n);
    std::tuple<bool, int32_t, int32_t, int32_t> GetItems(int32_t m, int32_t n);
    void PushItems(int32_t m, int32_t n, std::tuple<bool, int32_t, int32_t, int32_t> tp);
    bool IsDestroyable(int32_t m, int32_t n) const;
    std::vector<std::pair<int32_t, int32_t>> GetRiverCoordinates(int32_t n, int32_t m) const;
    std::pair<int32_t, int32_t> GetTeleportCoordinates(int32_t m, int32_t n) const;
    std::tuple<bool, bool, bool> GetWeapon(int32_t m, int32_t n);
    bool GetMedicine(int32_t m, int32_t n);

 private:
    int32_t size_m_, size_n_;
    std::vector<std::vector<boost::variant<Road,
                                           Wall,
                                           River,
                                           Metro,
                                           Armoury,
                                           Hospital,
                                           Bomb,
                                           Concrete> > > game_map_;
};

#endif  // GAMEMAP_H_
