// Created by Vladislav Melnikov on 05.12.17.

#ifndef LABTECH_MAINGUIW_HPP
#define LABTECH_MAINGUIW_HPP

#include "MENU.hpp"

class MainGUI {
private:
    // Все меню, которые используются вне игры
    MENU main_menu_, settings_;
    // Меню, в котором в данный момент
    // находится пользователь
    MENU *cur_window_;

public:
    MainGUI();
    virtual ~MainGUI();

    // Управление контроллером для
    // навигации по меню
    void go();
};


#endif //LABTECH_MAINGUIW_HPP
