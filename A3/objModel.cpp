// CPSC 453: Introduction to Computer Graphics
//
// Implementation file for a class to handle obj file data
//
// Created by Troy Alderson
// Department of Computer Science
// University of Calgary
//
// Used with permission for CPSC 453 HW3

#include "objModel.h"
#include <fstream>
#include <string>
#include <iostream>
#include <cstring>
#include <QFileDialog>

using namespace std;
using namespace glm;

// Initialize the 21 model animations, for the provided models
anim_t ObjModel::animlist[ 21 ] =
{
        // first, last, fps

        {   0,  39,  9 },       // STAND
        {  40,  45, 10 },       // RUN
        {  46,  53, 10 },       // ATTACK
        {  54,  57,  7 },       // PAIN_A
        {  58,  61,  7 },       // PAIN_B
        {  62,  65,  7 },       // PAIN_C
        {  66,  71,  7 },       // JUMP
        {  72,  83,  7 },       // FLIP
        {  84,  94,  7 },       // SALUTE
        {  95, 111, 10 },       // FALLBACK
        { 112, 122,  7 },       // WAVE
        { 123, 134,  6 },       // POINT
        { 135, 153, 10 },       // CROUCH_STAND
        { 154, 159,  7 },       // CROUCH_WALK
        { 160, 168, 10 },       // CROUCH_ATTACK
        { 169, 172,  7 },       // CROUCH_PAIN
        { 173, 177,  5 },       // CROUCH_DEATH
        { 178, 183,  7 },       // DEATH_FALLBACK
        { 184, 189,  7 },       // DEATH_FALLFORWARD
        { 190, 197,  7 },       // DEATH_FALLBACKSLOW
        { 198, 198,  5 },       // BOOM
};

// default constructor
ObjModel::ObjModel () {

}

// file-loading constructor
ObjModel::ObjModel (const char *filename) {
    LoadModel(filename);
    calculateVertexNormals();
    colorTheObject(1.f, 1.f, 1.f);
    setupObject();
    printf("%d %d %d %d %d", vertDraw.size(), colors.size(), faceNormals.size(), vertexNormals.size(), uvDraw.size());
}

// loads the model, populating the data
bool ObjModel::LoadModel(const char *filename) {

    // clear prior data
    num_xyz = num_tris = skin_width = skin_height = 0;
    m_vertices.clear(); texs.clear(); tris.clear(); faceNormals.clear();

    // From http://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/

    printf("Loading OBJ file %s...\n", filename);

    // try to open the file
    FILE * file = fopen(filename, "r");
    if( file == NULL ){
        printf("Impossible to open the file! Do you have the correct filename?\n");
        getchar();
        return false;
    }

    // load values from the file
    while( true ){

        char lineHeader[128];
        // read the first word of the line
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF)
            break; // EOF = End Of File. Quit the loop.

        // parse lineHeader
        if ( strcmp( lineHeader, "v" ) == 0 ){
            // x, y, z: vertex coordinates
             vec3 vertex;
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
            m_vertices.push_back(vertex);
            num_xyz++;

        }else if ( strcmp( lineHeader, "vt" ) == 0 ){
            // u, v: texture coordinates
             vec2 uv;
            fscanf(file, "%f %f\n", &uv.x, &uv.y );
            uv.y = -uv.y; // Invert V coordinate since we will only use DDS texture, which are inverted. Remove if you want to use TGA or BMP loaders.
            texs.push_back(uv);
        }else if ( strcmp( lineHeader, "vn" ) == 0 ){
            // normal coordinates (ignored)
             vec3 normal;
            fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
            //normals.push_back(normal);

        }else if ( strcmp( lineHeader, "f" ) == 0 ){
            // face indices
            tri t;
            int matches = fscanf(file, "%d/%d %d/%d %d/%d\n", &(t.index_xyz[0]), &(t.index_uv[0]), &(t.index_xyz[1]), &(t.index_uv[1]), &(t.index_xyz[2]), &(t.index_uv[2]) );
            if (matches != 6){
                printf("File can't be read by our simple parser :-( Try exporting with other options\n");
                return false;
            }

            tris.push_back(t);
            calculateFaceNormal(t);
            addToVerticesArray(t);
            addToUVArray(t);
            num_tris++;

        }else{
            // Probably a comment, eat up the rest of the line
            char stupidBuffer[1000];
            fgets(stupidBuffer, 1000, file);
        }

    }

    return true;

}

void ObjModel::calculateVertexNormals() {
    struct vertexInfo{
        vec3 vector;
        int numOfAdjacentFaces;
    };
    vertexInfo *info = new vertexInfo[m_vertices.size()];
    vertexInfo vInfo;
    for (int i=0; i<tris.size(); i++) {
        for (int j=0; j<3; j++) {
            vInfo = vertexInfo();
            vInfo.vector.x += faceNormals[3*(3*i+j)];
            vInfo.vector.y += faceNormals[3*(3*i+j)+1];
            vInfo.vector.z += faceNormals[3*(3*i+j)+2];
            vInfo.numOfAdjacentFaces++;
            info[tris.at(i).index_xyz[j]-1] = vInfo;
        }
    }

    for (int i=0; i<m_vertices.size(); i++) {
        info[i].vector.x /= info[i].numOfAdjacentFaces;
        info[i].vector.y /= info[i].numOfAdjacentFaces;
        info[i].vector.z /= info[i].numOfAdjacentFaces;
        info[i].vector =  normalize(info[i].vector);
    }

    for (int i=0; i<tris.size(); i++) {
        for (int j=0; j<3; j++) {
            vertexNormals.push_back(info[tris.at(i).index_xyz[j]-1].vector.x);
            vertexNormals.push_back(info[tris.at(i).index_xyz[j]-1].vector.y);
            vertexNormals.push_back(info[tris.at(i).index_xyz[j]-1].vector.z);
        }
    }
}

void ObjModel::setAttributes(GLuint m_posAttr, GLuint m_colAttr, GLuint m_norAttr) {
    this->m_posAttr = m_posAttr;
    this->m_colAttr = m_colAttr;
    this->m_norAttr = m_norAttr;
}

void ObjModel::setAttributesUV(GLuint m_posAttr, GLuint m_colAttr, GLuint m_norAttr, GLuint m_uvAttr, GLuint m_TextureUniform) {
    this->m_posAttr = m_posAttr;
    this->m_colAttr = m_colAttr;
    this->m_norAttr = m_norAttr;
    this->m_uvAttr = m_uvAttr;
    this->m_TextureUniform = m_TextureUniform;
}

void ObjModel::calculateFaceNormal(tri t) {
     vec3 normal, a, b, c;
    a = m_vertices.at(t.index_xyz[0]-1);
    b = m_vertices.at(t.index_xyz[1]-1);
    c = m_vertices.at(t.index_xyz[2]-1);
    b = b-a;
    c = c-a;
    normal =  normalize( cross(b, c));
    for (int i=0; i<3; i++) {
        faceNormals.push_back(normal.x);
        faceNormals.push_back(normal.y);
        faceNormals.push_back(normal.z);
    }
}

//add in the white color
void ObjModel::colorTheObject(float r, float g, float b) {
    for (int i=0; i<vertDraw.size()/3; i++) {
        colors.push_back(r);
        colors.push_back(g);
        colors.push_back(b);
    }
}

void ObjModel::addToVerticesArray(tri t) {
    for (int i=0; i<3; i++) {
        vec3 point = m_vertices.at(t.index_xyz[i]-1);
        vertDraw.push_back(point.x);
        vertDraw.push_back(point.y);
        vertDraw.push_back(point.z);
    }
}

void ObjModel::addToUVArray(tri t) {

    for(int i = 0; i < 3; i++)
    {
        vec2 texture = texs.at(t.index_uv[i] - 1);


        uvDraw.push_back(texture.x);
        uvDraw.push_back(texture.y);
    }

}

void ObjModel::draw2() {
    long cBufferSize = colors.size() * sizeof(float),
        vBufferSize = vertDraw.size() * sizeof(float),
        nBufferSize = vertexNormals.size() * sizeof(float);

    // Bind to the correct context
    glBindBuffer(GL_ARRAY_BUFFER, this->m_vbo);

    // Enable the attribute arrays
    glEnableVertexAttribArray(this->m_posAttr);
    glEnableVertexAttribArray(this->m_colAttr);
    glEnableVertexAttribArray(this->m_norAttr);

    // Specifiy where these are in the VBO
    glVertexAttribPointer(this->m_posAttr, 3, GL_FLOAT, 0, GL_FALSE,
        (const GLvoid*)0);

    glVertexAttribPointer(this->m_colAttr, 3, GL_FLOAT, 0, GL_FALSE,
        (const GLvoid*)(vBufferSize));

    glVertexAttribPointer(this->m_norAttr, 3, GL_FLOAT, 0, GL_FALSE,
        (const GLvoid*)(vBufferSize + cBufferSize));

    // Draw the triangles
    glDrawArrays(GL_TRIANGLES, 0, vertDraw.size()); // 36 vertices

    glDisableVertexAttribArray(m_norAttr);
    glDisableVertexAttribArray(m_colAttr);
    glDisableVertexAttribArray(m_posAttr);
}
//for face normals
void ObjModel::draw1() {
    long cBufferSize = colors.size() * sizeof(float),
        vBufferSize = vertDraw.size() * sizeof(float),
        nBufferSize = faceNormals.size() * sizeof(float);

    // Bind to the correct context
    glBindBuffer(GL_ARRAY_BUFFER, this->m_vbo);

    // Enable the attribute arrays
    glEnableVertexAttribArray(this->m_posAttr);
    glEnableVertexAttribArray(this->m_colAttr);
    glEnableVertexAttribArray(this->m_norAttr);

    // Specifiy where these are in the VBO
    glVertexAttribPointer(this->m_posAttr, 3, GL_FLOAT, 0, GL_FALSE,
        (const GLvoid*)0);

    glVertexAttribPointer(this->m_colAttr, 3, GL_FLOAT, 0, GL_FALSE,
        (const GLvoid*)(vBufferSize));

    glVertexAttribPointer(this->m_norAttr, 3, GL_FLOAT, 0, GL_FALSE,
        (const GLvoid*)(vBufferSize + cBufferSize));

    // Draw the triangles
    glDrawArrays(GL_TRIANGLES, 0, vertDraw.size()); // 36 vertices

    glDisableVertexAttribArray(m_norAttr);
    glDisableVertexAttribArray(m_colAttr);
    glDisableVertexAttribArray(m_posAttr);
}

void ObjModel::draw0() {
    long cBufferSize = colors.size() * sizeof(float),
        vBufferSize = vertDraw.size() * sizeof(float);


    // Bind to the correct context
    glBindBuffer(GL_ARRAY_BUFFER, this->m_vbo);

    // Enable the attribute arrays
    glEnableVertexAttribArray(this->m_posAttr);
    glEnableVertexAttribArray(this->m_colAttr);
    //glEnableVertexAttribArray(this->m_norAttr);

    // Specifiy where these are in the VBO
    glVertexAttribPointer(this->m_posAttr, 3, GL_FLOAT, 0, GL_FALSE,
        (const GLvoid*)0);

    glVertexAttribPointer(this->m_colAttr, 3, GL_FLOAT, 0, GL_FALSE,
        (const GLvoid*)(vBufferSize + cBufferSize));

    //glVertexAttribPointer(this->m_norAttr, 3, GL_FLOAT, 0, GL_FALSE,
        //(const GLvoid*)(vBufferSize + cBufferSize + nBufferSize));

    // Draw the triangles
    glDrawArrays(GL_TRIANGLES, 0, vertDraw.size()); // 36 vertices

    //glDisableVertexAttribArray(m_norAttr);
    glDisableVertexAttribArray(m_colAttr);
    glDisableVertexAttribArray(m_posAttr);
}

//face normals with UV
void ObjModel::drawWithUV() {
    long cBufferSize = colors.size() * sizeof(float),
        vBufferSize = vertDraw.size() * sizeof(float),
        nBufferSize = faceNormals.size() * sizeof(float);

    // Bind to the correct context
    glBindBuffer(GL_ARRAY_BUFFER, this->m_vbo);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_testTexture);
    glUniform1i(m_TextureUniform, 0); // Give it the 0'th texture unit


    // Enable the attribute arrays
    glEnableVertexAttribArray(this->m_posAttr);
    glEnableVertexAttribArray(this->m_colAttr);
    glEnableVertexAttribArray(this->m_norAttr);
    glEnableVertexAttribArray(this->m_uvAttr);


    // Specifiy where these are in the VBO
    glVertexAttribPointer(this->m_posAttr, 3, GL_FLOAT, 0, GL_FALSE,
        (const GLvoid*)0);

    glVertexAttribPointer(this->m_colAttr, 3, GL_FLOAT, 0, GL_FALSE,
        (const GLvoid*)(vBufferSize));

    glVertexAttribPointer(this->m_norAttr, 3, GL_FLOAT, 0, GL_FALSE,
        (const GLvoid*)(vBufferSize + cBufferSize));


    glVertexAttribPointer(this->m_uvAttr, 2, GL_FLOAT, 0, GL_FALSE,
        (const GLvoid*)(vBufferSize + cBufferSize + nBufferSize));



    // Draw the triangles
    glDrawArrays(GL_TRIANGLES, 0, vertDraw.size()); // 36 vertices

    glDisableVertexAttribArray(this->m_uvAttr);
    glDisableVertexAttribArray(this->m_norAttr);
    glDisableVertexAttribArray(this->m_colAttr);
    glDisableVertexAttribArray(this->m_posAttr);
}
//for vertex normals
void ObjModel::drawWithUV2() {
    long cBufferSize = colors.size() * sizeof(float),
        vBufferSize = vertDraw.size() * sizeof(float),
        nBufferSize = vertexNormals.size() * sizeof(float);

    // Bind to the correct context
    glBindBuffer(GL_ARRAY_BUFFER, this->m_vbo);


    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_testTexture);
    glUniform1i(m_TextureUniform, 0); // Give it the 0'th texture unit


    // Enable the attribute arrays
    glEnableVertexAttribArray(this->m_posAttr);
    glEnableVertexAttribArray(this->m_colAttr);
    glEnableVertexAttribArray(this->m_norAttr);
    glEnableVertexAttribArray(this->m_uvAttr);


    // Specifiy where these are in the VBO
    glVertexAttribPointer(this->m_posAttr, 3, GL_FLOAT, 0, GL_FALSE,
        (const GLvoid*)0);

    glVertexAttribPointer(this->m_colAttr, 3, GL_FLOAT, 0, GL_FALSE,
        (const GLvoid*)(vBufferSize));

    glVertexAttribPointer(this->m_norAttr, 3, GL_FLOAT, 0, GL_FALSE,
        (const GLvoid*)(vBufferSize + cBufferSize));


    glVertexAttribPointer(this->m_uvAttr, 2, GL_FLOAT, 0, GL_FALSE,
        (const GLvoid*)(vBufferSize + cBufferSize + nBufferSize));



    // Draw the triangles
    glDrawArrays(GL_TRIANGLES, 0, vertDraw.size()); // 36 vertices

    glDisableVertexAttribArray(this->m_uvAttr);
    glDisableVertexAttribArray(this->m_norAttr);
    glDisableVertexAttribArray(this->m_colAttr);
    glDisableVertexAttribArray(this->m_posAttr);
}

//for face Normals
void ObjModel::setupObject()
{
    initializeOpenGLFunctions();
    long cBufferSize = colors.size() * sizeof(float),
        vBufferSize = vertDraw.size() * sizeof(float),
        nBufferSize = faceNormals.size() * sizeof(float);

    //create and bind the VBO
    glGenBuffers(1, &this->m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, this->m_vbo);

    // Allocate buffer
    glBufferData(GL_ARRAY_BUFFER, vBufferSize + cBufferSize + nBufferSize, NULL,
        GL_STATIC_DRAW);

    // Upload the data to the GPU
    glBufferSubData(GL_ARRAY_BUFFER, 0, vBufferSize, &vertDraw[0]);
    glBufferSubData(GL_ARRAY_BUFFER, vBufferSize, cBufferSize, &colors[0]);
    glBufferSubData(GL_ARRAY_BUFFER, vBufferSize + cBufferSize, nBufferSize,&faceNormals[0]);
}

//for vertex normal
void ObjModel::setupObject2()
{
    initializeOpenGLFunctions();
    long cBufferSize = colors.size() * sizeof(float),
        vBufferSize = vertDraw.size() * sizeof(float),
        nBufferSize = vertexNormals.size() * sizeof(float);
    //create and bind the VBO
    glGenBuffers(1, &this->m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, this->m_vbo);

    // Allocate buffer
    glBufferData(GL_ARRAY_BUFFER, vBufferSize + cBufferSize + nBufferSize, NULL, GL_STATIC_DRAW);

    // Upload the data to the GPU
    glBufferSubData(GL_ARRAY_BUFFER, 0, vBufferSize, &vertDraw[0]);
    glBufferSubData(GL_ARRAY_BUFFER, vBufferSize, cBufferSize, &colors[0]);
    glBufferSubData(GL_ARRAY_BUFFER, vBufferSize + cBufferSize, nBufferSize,&vertexNormals[0]);
}
//for face normals
void ObjModel::setupObjectUV(QString texture)
{
    initializeOpenGLFunctions();
    long cBufferSize = colors.size() * sizeof(float),
        vBufferSize = vertDraw.size() * sizeof(float),
        nBufferSize = faceNormals.size() * sizeof(float),
        uvBufferSize = uvDraw.size() * sizeof(float);

    QImage image; // Load the image

    image.load(texture);
    image = image.convertToFormat(QImage::Format_RGBA8888); // Convert it to a usable format

    glGenTextures(1, &m_testTexture); // Generate a texture handle
     glActiveTexture(GL_TEXTURE0); // Make sure we're using texture unit 0
     glBindTexture(GL_TEXTURE_2D, m_testTexture); // bind the texture handle

    // Write it to the GPU
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGBA,
                 image.width(),
                 image.height(),
                 0,
                 GL_RGBA,
                 GL_UNSIGNED_INT_8_8_8_8_REV,
                 (const GLvoid *)image.bits());
    // Set the filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


    //create and bind the VBO
    glGenBuffers(1, &this->m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, this->m_vbo);

    // Allocate buffer
    glBufferData(GL_ARRAY_BUFFER, vBufferSize + cBufferSize + nBufferSize + uvBufferSize , NULL, GL_STATIC_DRAW);

    // Upload the data to the GPU
    glBufferSubData(GL_ARRAY_BUFFER, 0, vBufferSize, &vertDraw[0]);
    glBufferSubData(GL_ARRAY_BUFFER, vBufferSize, cBufferSize, &colors[0]);
    glBufferSubData(GL_ARRAY_BUFFER, vBufferSize + cBufferSize, nBufferSize,&faceNormals[0]);
    glBufferSubData(GL_ARRAY_BUFFER, vBufferSize + cBufferSize + nBufferSize, uvBufferSize,&uvDraw[0]);
}

//for vertex normals
void ObjModel::setupObjectUV2(QString texture)
{
    initializeOpenGLFunctions();
    long cBufferSize = colors.size() * sizeof(float),
        vBufferSize = vertDraw.size() * sizeof(float),
        nBufferSize = vertexNormals.size() * sizeof(float),
        uvBufferSize = uvDraw.size() * sizeof(float);

    QImage image; // Load the image


    image.load(texture);
    image = image.convertToFormat(QImage::Format_RGBA8888); // Convert it to a usable format

    glGenTextures(1, &m_testTexture); // Generate a texture handle
     glActiveTexture(GL_TEXTURE0); // Make sure we're using texture unit 0
     glBindTexture(GL_TEXTURE_2D, m_testTexture); // bind the texture handle

    // Write it to the GPU
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGBA,
                 image.width(),
                 image.height(),
                 0,
                 GL_RGBA,
                 GL_UNSIGNED_INT_8_8_8_8_REV,
                 (const GLvoid *)image.bits());
    // Set the filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


    //create and bind the VBO
    glGenBuffers(1, &this->m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, this->m_vbo);

    // Allocate buffer
    glBufferData(GL_ARRAY_BUFFER, vBufferSize + cBufferSize + nBufferSize + uvBufferSize , NULL, GL_STATIC_DRAW);

    // Upload the data to the GPU
    glBufferSubData(GL_ARRAY_BUFFER, 0, vBufferSize, &vertDraw[0]);
    glBufferSubData(GL_ARRAY_BUFFER, vBufferSize, cBufferSize, &colors[0]);
    glBufferSubData(GL_ARRAY_BUFFER, vBufferSize + cBufferSize, nBufferSize,&vertexNormals[0]);
    glBufferSubData(GL_ARRAY_BUFFER, vBufferSize + cBufferSize + nBufferSize, uvBufferSize,&uvDraw[0]);
}

