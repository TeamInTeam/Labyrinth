// Created by Vladislav Melnikov on 05.12.17.

#ifndef LABTECH_MAINGUIW_HPP
#define LABTECH_MAINGUIW_HPP

#include "MENU.hpp"

class MainGUIw {
private:
    MENU main_menu_, settings_;
    MENU *cur_window_;

public:
    MainGUIw();
    virtual ~MainGUIw();

    void go();
};


#endif //LABTECH_MAINGUIW_HPP
