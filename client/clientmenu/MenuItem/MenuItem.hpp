// Created by Vladislav Melnikov on 07.12.17.

#ifndef LABTECH_MENUITEM_HPP
#define LABTECH_MENUITEM_HPP

#include <iostream>


class MenuItem {
protected:
    std::string name;

public:
    MenuItem(std::string str);
    virtual ~MenuItem() = default;
    std::string to_string() { return name; };
    virtual bool action() = 0;
};


#endif //LABTECH_MENUITEM_HPP
