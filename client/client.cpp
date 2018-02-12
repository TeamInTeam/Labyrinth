// Created by Vladislav Melnikov on 26.11.17.

//#include <csignal>
#include "clientmenu/MainGUI.hpp"


//void sig(int n) {}


int main() {
//    setlocale(LC_ALL, "");
//    signal(SIGINT, sig);

    MainGUI m;
    m.go();
    return 0;
}