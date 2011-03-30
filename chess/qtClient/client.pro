######################################################################
# Automatically generated by qmake (2.01a) ?? ????. 28 15:38:00 2009
######################################################################

QMAKE_CXXFLAGS = -g

TEMPLATE = app
TARGET = client

SRC = src
BASE = $$SRC/Base
NETWORK = $$SRC/Network
DATA = $$SRC/Data
UI = $$SRC/UI
DIALOGS = $$SRC/Dialogs

DEPENDPATH += .
DEPENDPATH += $$NETWORK  $$UI  $$BASE  $$DATA $$DIALOGS

INCLUDEPATH += $$SRC  $$NETWORK  $$UI  $$BASE  $$DATA
INCLUDEPATH += $$(QTPATH)/QtNetwork $$(QTPATH)/QtXml
INCLUDEPATH += $$(WAPSERVERPATH) $$(WAPSERVERPATH)/libs $$(WAPSERVERPATH)/libs/header $$(WAPSERVERPATH)/chessserver $$(WAPSERVERPATH)/chessserver/engine


DESTDIR = bin
OBJECTS_DIR = obj
MOC_DIR = obj
LIBS = -lQtNetwork -lQtXml

HEADERS += $$BASE/Animation.h $$BASE/Board.h $$BASE/CaptureBox.h $$BASE/Chat.h $$BASE/Clock.h $$BASE/Button.h $$BASE/MoveBox.h $$BASE/MyDialog.h $$BASE/MyMessageBox.h $$BASE/Exception.h $$BASE/Defines.h   $$BASE/Cell.h $$BASE/orientation.h $$BASE/orientthread.h
HEADERS += $$DATA/XML.h $$DATA/Pixmaps.h  
HEADERS += $$DIALOGS/AuthorizationDialog.h $$DIALOGS/CreateGameDialog.h $$DIALOGS/WaitJoinDialog.h $$DIALOGS/JoinGameDialog.h $$DIALOGS/FindGameDialog.h $$DIALOGS/WaitAgreeDialog.h $$DIALOGS/GameDialog.h $$DIALOGS/WaitDrawDialog.h  $$DIALOGS/ChatMessageDialog.h  $$DIALOGS/OptionsDialog.h $$DIALOGS/PromotionDialog.h
HEADERS += $$UI/UI.h $$NETWORK/Client.h $$UI/MainWindow.h $$UI/GameScene.h $$UI/MainMenu.h 

SOURCES += $$BASE/Animation.cpp $$BASE/Board.cpp $$BASE/CaptureBox.cpp $$BASE/Chat.cpp $$BASE/Clock.cpp $$BASE/Button.cpp  $$BASE/MoveBox.cpp $$BASE/MyDialog.cpp $$BASE/MyMessageBox.cpp $$BASE/Cell.cpp $$BASE/orientation.cpp $$BASE/orientthread.cpp
SOURCES += $$DATA/Pixmaps.cpp $$DATA/XML.cpp
SOURCES += $$DIALOGS/AuthorizationDialog.cpp $$DIALOGS/CreateGameDialog.cpp $$DIALOGS/WaitJoinDialog.cpp $$DIALOGS/JoinGameDialog.cpp $$DIALOGS/FindGameDialog.cpp $$DIALOGS/WaitAgreeDialog.cpp $$DIALOGS/GameDialog.cpp $$DIALOGS/WaitDrawDialog.cpp $$DIALOGS/ChatMessageDialog.cpp  $$DIALOGS/OptionsDialog.cpp $$DIALOGS/PromotionDialog.cpp
SOURCES += $$SRC/main.cpp $$UI/UI.cpp $$NETWORK/Client.cpp $$UI/MainWindow.cpp $$UI/GameScene.cpp $$UI/MainMenu.cpp  

RESOURCES = resources.qrc

TRANSLATIONS = translate/strings_ru.ts



QT       += core gui
CONFIG += meegotouch
TEMPLATE = app
target.path=/usr/local/bin
INSTALLS=target

