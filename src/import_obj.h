#include <vector>
#include <string>

#include <stdio.h>
#include <cstring>
#include <sstream>
#include <iostream>
#include <functional>

#include <emscripten/fetch.h>

#include "utility.h"

struct obj_data
{
    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<float> uvs;
};

obj_data convert(std::string data)
{
    obj_data out;
    std::vector<unsigned int> vertex_indicies;
    std::vector<unsigned int> normal_indicies;
    std::vector<unsigned int> uv_indicies;
    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<float> uvs;

    std::istringstream f(data.c_str());
    std::string line;
    while (std::getline(f, line))
    {
        //std::cout << line.c_str() << std::endl;

        if (line.find("v ", 0) != std::string::npos)
        {
            math::vec3 vec{};
            std::sscanf(line.c_str(), "%*s %f %f %f", &vec[0], &vec[1], &vec[2]);
            vertices.push_back(vec[0]);
            vertices.push_back(vec[1]);
            vertices.push_back(vec[2]);
        }
        else if (line.find("vt ", 0) != std::string::npos)
        {
            math::vec3 vec{};
            std::sscanf(line.c_str(), "%*s %f %f", &vec[0], &vec[1]);
            uvs.push_back(vec[0]);
            uvs.push_back(vec[1]);
        }
        else if (line.find("vn ", 0) != std::string::npos)
        {
            math::vec3 vec{};
            std::sscanf(line.c_str(), "%*s %f %f %f", &vec[0], &vec[1], &vec[2]);
            normals.push_back(vec[0]);
            normals.push_back(vec[1]);
            normals.push_back(vec[2]);
        }
        else if (line.find("f ", 0) != std::string::npos)
        {
            unsigned int v_i[3], n_i[3], uv_i[3];
            int n_matches = std::sscanf(line.c_str(), "%*s %d/%d/%d %d/%d/%d %d/%d/%d",
                                        &v_i[0], &uv_i[0], &n_i[0],
                                        &v_i[1], &uv_i[1], &n_i[1],
                                        &v_i[2], &uv_i[2], &n_i[2]);

            vertex_indicies.push_back(v_i[0] - 1);
            vertex_indicies.push_back(v_i[1] - 1);
            vertex_indicies.push_back(v_i[2] - 1);
            uv_indicies.push_back(uv_i[0] - 1);
            uv_indicies.push_back(uv_i[1] - 1);
            uv_indicies.push_back(uv_i[1] - 1);
            normal_indicies.push_back(n_i[0] - 1);
            normal_indicies.push_back(n_i[1] - 1);
            normal_indicies.push_back(n_i[2] - 1);
        }
    }

    std::cout << "n_vertices: " << vertices.size() << std::endl;
    std::cout << "n_normals: " << normals.size() << std::endl;
    std::cout << "n_uvs: " << uvs.size() << std::endl;
    std::cout << "n_v_i: " << vertex_indicies.size() << std::endl;
    std::cout << "n_n_i: " << normal_indicies.size() << std::endl;
    std::cout << "n_uv_i: " << uv_indicies.size() << std::endl;
    //process
    for (auto i : vertex_indicies)
    {
        out.vertices.push_back(vertices[i * 3 + 0]);
        out.vertices.push_back(vertices[i * 3 + 1]);
        out.vertices.push_back(vertices[i * 3 + 2]);
    }
    for (auto i : normal_indicies)
    {
        out.normals.push_back(normals[i * 3 + 0]);
        out.normals.push_back(normals[i * 3 + 1]);
        out.normals.push_back(normals[i * 3 + 2]);
    }
    for (auto i : uv_indicies)
    {
        out.uvs.push_back(uvs[i * 2 + 0]);
        out.uvs.push_back(uvs[i * 2 + 1]);
    }

    std::cout << "n_out_vertices: " << out.vertices.size() << std::endl;
    std::cout << "n_out_normals: " << out.normals.size() << std::endl;
    std::cout << "n_out_uvs: " << out.uvs.size() << std::endl;

    return out;
}

obj_data import_test_cube()
{
    std::string data =
        "v 1.000000 -1.000000 -1.000000\n"
        "v 1.000000 -1.000000 1.000000\n"
        "v -1.000000 -1.000000 1.000000\n"
        "v -1.000000 -1.000000 -1.000000\n"
        "v 1.000000 1.000000 -1.000000\n"
        "v 0.999999 1.000000 1.000001\n"
        "v -1.000000 1.000000 1.000000\n"
        "v -1.000000 1.000000 -1.000000\n"
        "vn 0.000000 0.000000 -1.000000\n"
        "vn -1.000000 -0.000000 -0.000000\n"
        "vn -0.000000 -0.000000 1.000000\n"
        "vn -0.000001 0.000000 1.000000\n"
        "vn 1.000000 -0.000000 0.000000\n"
        "vn 1.000000 0.000000 0.000001\n"
        "vn 0.000000 1.000000 -0.000000\n"
        "vn -0.000000 -1.000000 0.000000\n"
        "vt 0.748573 0.750412\n"
        "vt 0.749279 0.501284\n"
        "vt 0.999110 0.501077\n"
        "vt 0.999455 0.750380\n"
        "vt 0.250471 0.500702\n"
        "vt 0.249682 0.749677\n"
        "vt 0.001085 0.750380\n"
        "vt 0.001517 0.499994\n"
        "vt 0.499422 0.500239\n"
        "vt 0.500149 0.750166\n"
        "vt 0.748355 0.998230\n"
        "vt 0.500193 0.998728\n"
        "vt 0.498993 0.250415\n"
        "vt 0.748953 0.250920\n"
        "f 5/1/1 1/2/1 4/3/1\n"
        "f 5/1/1 4/3/1 8/4/1\n"
        "f 3/5/2 7/6/2 8/7/2\n"
        "f 3/5/2 8/7/2 4/8/2\n"
        "f 2/9/3 6/10/3 3/5/3\n"
        "f 6/10/4 7/6/4 3/5/4\n"
        "f 1/2/5 5/1/5 2/9/5\n"
        "f 5/1/6 6/10/6 2/9/6\n"
        "f 5/1/7 8/11/7 6/10/7\n"
        "f 8/11/7 7/12/7 6/10/7\n"
        "f 1/2/8 2/9/8 3/13/8\n"
        "f 1/2/8 3/13/8 4/14/8\n";

    return convert(data);
}

obj_data import_test_asm()
{
    auto data = load_file("assets/assembly.obj");

    return convert(std::string(data.begin(), data.end()));
}
