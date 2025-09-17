cmake_minimum_required(VERSION 3.10) 

# Nom du projet 
project(TerminalClient)

# Spécifier le standard C++ 
set(CMAKE_CXX_STANDARD 11) 
set(CMAKE_CXX_STANDARD_REQUIRED True) 

# Trouver les modules Qt nécessaires
find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

include_directories(ClientConsultationBookerQt)

# Ajouter les fichiers sources automatiquement 
file(GLOB SOURCES "ClientConsultationBookerQt/*.cpp") 

# Ajouter l'exécutable
add_executable(${PROJECT_NAME} ${SOURCES})

# Lier les bibliothèques Qt nécessaires 
target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Gui Qt5::Widgets) 