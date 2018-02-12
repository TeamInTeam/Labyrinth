// Created by Vladislav Melnikov on 05.12.17.

#ifndef LABTECH_MENU_HPP
#define LABTECH_MENU_HPP

#include <iostream>
#include <vector>
#include <panel.h>
#include "MenuItem/MenuItem.hpp"
#include "MenuItem/BUNG/BUNG.hpp"
#include "MenuItem/LINK/LINK.hpp"
#include "MenuItem/START/START.hpp"
#include "MenuItem/QUIT/QUIT.hpp"

class MENU {
private:
    // Окно, используемое для этой менюшки
    WINDOW* w;
    // Панель для нее
    PANEL* p;
    // Вектор с элементами (полями) меню
    std::vector<MenuItem*> fields_;
    // Вывод менюшки на экран
    void print();

    // Положение, в котором находится курсор
    uint32_t cursor_;

public:
    MENU() = default;
    MENU(std::initializer_list<MenuItem*> fields);

    MENU& operator=(MENU&& men) noexcept;
    virtual ~MENU();

    // Контроллер для перемещения курсора по меню.
    // Возвращает 0, если нужно выйти из игры, 1 - иначе.
    bool select();
};


#endif //LABTECH_MENU_HPP
