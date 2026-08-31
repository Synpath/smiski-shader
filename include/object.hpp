#ifndef OBJECT_HPP
#define OBJECT_HPP

class Object {

    public:
        float * vertices;
        unsigned int VAO;
        unsigned int VBO;

        Object(float * vData);
};

#endif