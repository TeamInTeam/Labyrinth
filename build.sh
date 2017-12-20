#!/bin/bash


# Generate database structure

names=('user' 'map')
echo models: [${names[*]}]

for i in ${!names[*]}
do
   odb -d pgsql --generate-query --generate-schema ${names[$i]}.hxx
   g++ -c ${names[$i]}-odb.cxx
   names[$i]=${names[$i]}-odb.o
done

# Requirements:
# odb libssl-dev libboost

# Compile project

g++ -c demo.cxx -std=c++14
g++ -c AuthenticationManager.cpp -std=c++14
g++ -c MapProvider.cpp -std=c++14

echo options: -o demo demo.o AuthenticationManager.o MapProvider.o ${names[*]} -lodb-pgsql -lodb -lboost_system -lcrypto -lssl -std=c++14 -Wall -Wextra
g++ -o demo demo.o AuthenticationManager.o MapProvider.o ${names[*]} -lodb-pgsql -lodb -lboost_system -lcrypto -lssl -std=c++14 -Wall -Wextra
echo Done