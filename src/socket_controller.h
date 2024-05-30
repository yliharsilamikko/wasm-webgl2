#pragma once
#include <functional>
#include <emscripten.h>
#include <emscripten/html5.h>
#include "core_math.h"
#include "part.h"

#include <stdio.h> // printf debug
#include <map>

class socket_controller;
int next_id = 0;
std::map<int, socket_controller *> socket_controllers;

class socket_controller
{
private:
    int id_;
    part *part_instance_ = nullptr;

public:
    socket_controller(part *part_instance)
    {
        part_instance_ = part_instance;
        id_ = next_id++;
        socket_controllers[id_] = this;

        EM_ASM(
            var id = $0;

            ws = new WebSocket("wss://wasm-webgl-server-915aea422050.herokuapp.com/");
            ws.onopen = function() { console.log("ws.onopen\n"); };

            ws.onmessage = function(event) {
                var data = JSON.parse(event.data);
                _on_message(id, data.x); };
            
            ws.onclose = function(e) { console.log("ws.onclose\n"); };

            ws.onerror = function(err) { console.log("ws.onerror\n"); };
            
            , id_);

    }

    void on_message(float x)
    {
        part_instance_->position[0] = x;
    }

    void send(float delta)
    {
        EM_ASM(
            var delta = $0;

            var data = {
                x : delta
            };

            ws.send(JSON.stringify(data));

            , delta);
    }
};

extern "C" EMSCRIPTEN_KEEPALIVE void on_message(int id, float x)
{
    printf("on_message\n");
    socket_controllers[id]->on_message(x);
}