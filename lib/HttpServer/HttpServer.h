#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
// #include "memory.h"
#include <MemoryHandler.h>

void runHttpServer(MemoryHandler& memoryHandler);

#endif // HTTPSERVER_H