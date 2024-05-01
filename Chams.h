#ifndef ZEFF_CHAMS
#define ZEFF_CHAMS
#include <GLES2/gl2.h>
#include <dlfcn.h>
#include <array>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
using namespace std;
static void *handle;
static const char* shaderName;
static bool enableRainbow;
static float r = 0.0f;
static float g = 0.0f;
static float b = 0.0f;
static int w = 1;

float red = 255.0f;
float gren = 0.0f;
float blue = 0.0f;
float mi = 0.0f;

int chamsint = 0;

//ZEFF: For ImGui ColorEdit or ColorPicker
ImVec4 chamsColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
ImVec4 wallColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
ImVec4 visibleColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

//ZEFF: For ImGui Combo or something else
const char* Chams[] = {"OFF", "Default", "Shading", "Wireframe", "Outline", "Visibility Check Outline", "Visibility Check Shading", "Visibility Check Shading + Outline"};

const char *isDumpLocation = ("/sdcard/Download/Shaders.txt");

//ZEFF: Function with shader name dumping
string isDump(const char *name)
{
    vector<string> isShaders = {name};

    static string isTotalShaders;

    for (const auto &isAddTo: isShaders)
        isTotalShaders += (isAddTo + "\n"); // Adding whatever is added to the string vector to isTotalShaders, with a new line added per shader.

    return isTotalShaders.c_str();
}

void setShader(const char* s) {
    shaderName = s;
}
const char* getShader() {
    return shaderName;
}

void SetR(float set){
    r = set;
}

void SetG(float set){
    g = set;
}

void SetB(float set){
    b = set;
}

/*ZEFF: Uncomment the code if you want a shader dump.
        Don't leave the code uncommented because it causes lag during the game.*/
int (*old_glGetUniformLocation)(GLuint, const GLchar *);
GLint new_glGetUniformLocation(GLuint program, const GLchar *name) {
    //ofstream(isDumpLocation) << isDump(name);
    return old_glGetUniformLocation(program, name);
}

void (*old_glDrawElements)(GLenum mode, GLsizei count, GLenum type, const void *indices);
void new_glDrawElements(GLenum mode, GLsizei count, GLenum type, const void *indices) {
    //ZEFF: Old code
    //if (mode != GL_LINES && count < 1000) return old_glDrawElements(mode, count, type, indices);

    //ZEFF: New code with object index counts
    if (mode != GL_LINES && count != 786 && count != 852) return old_glDrawElements(mode, count, type, indices);

    GLint currProgram;
    glGetIntegerv(GL_CURRENT_PROGRAM, &currProgram);

    GLint id = old_glGetUniformLocation(currProgram, getShader());
    if (id == -1) return old_glDrawElements(mode, count, type, indices);

    if (chamsint == 0) {
        old_glDrawElements(mode, count, type, indices);
    }

    if (chamsint == 1) {
        glDepthRangef(1, 0.5);
        old_glDrawElements(GL_TRIANGLES, count, type, indices);
    }

    if (chamsint == 2) {
        glEnable(GL_BLEND);
        glDepthRangef(1, 0.5);
        glBlendFunc(GL_ONE, GL_CONSTANT_COLOR);
        glBlendColor(GLfloat(r), GLfloat(g), GLfloat(b), 1);
        old_glDrawElements(GL_TRIANGLES, count, type, indices);
    }

    if (chamsint == 3) {
        old_glDrawElements(mode, count, type, indices);
        glEnable(GL_BLEND);
        glBlendFunc(GL_CONSTANT_COLOR, GL_CONSTANT_ALPHA);
        glDepthRangef(1, 0.5);
        glBlendColor(GLfloat(r), GLfloat(g), GLfloat(b), 1);
        glLineWidth(w);
        old_glDrawElements(GL_LINES, count, type, indices);
    }

    if (chamsint == 4) {
        old_glDrawElements(mode, count, type, indices);
        glDepthRangef(1, 0);
        glLineWidth(10);
        glEnable(GL_BLEND);
        glBlendFunc(GL_CONSTANT_COLOR, GL_CONSTANT_ALPHA);
        glBlendColor(0, 0, 0, 1);
        old_glDrawElements(GL_TRIANGLES, count, type, indices);
        glDepthRangef(1, 0.5);
        glBlendColor(GLfloat(r), GLfloat(g), GLfloat(b), 1);
        old_glDrawElements(GL_LINES, count, type, indices);
    }

    if (chamsint == 5) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_CONSTANT_COLOR, GL_CONSTANT_ALPHA);
        glLineWidth(10);
        glBlendColor(GLfloat(r), GLfloat(g), GLfloat(b), 1);
        glDepthRangef(0.5, 1);
        old_glDrawElements(GL_LINES, count, type, indices);
        glBlendFunc(GL_ONE_MINUS_CONSTANT_COLOR, GL_ONE_MINUS_CONSTANT_ALPHA);
        glBlendColor(0, 0, 0, 1);
        glDepthRangef(1, 0.5);
        old_glDrawElements(GL_TRIANGLES, count, type, indices);
    }

    //ZEFF: If you have java source, replace glBlendColor parameters
    if (chamsint == 6) {
        glEnable(GL_BLEND);
        glDisable(GL_DEPTH_TEST);
        glBlendFunc(GL_ONE, GL_CONSTANT_COLOR);
        glBlendColor(wallColor.x, wallColor.y, wallColor.z, 1);
        old_glDrawElements(GL_TRIANGLES, count, type, indices);
        glEnable(GL_DEPTH_TEST);
        glDepthRangef(0.5, 1);
        glBlendColor(visibleColor.x, visibleColor.y, visibleColor.z, 1);
        old_glDrawElements(GL_TRIANGLES, count, type, indices);
    }

    if (chamsint == 7) {
        glEnable(GL_BLEND);
        glDisable(GL_DEPTH_TEST);
        glBlendFunc(GL_CONSTANT_COLOR, GL_CONSTANT_ALPHA);
        glBlendColor(wallColor.x, wallColor.y, wallColor.z, 1);
        old_glDrawElements(GL_TRIANGLES, count, type, indices);
        glEnable(GL_DEPTH_TEST);
        glDepthRangef(0.5, 1);
        glBlendColor(visibleColor.x, visibleColor.y, visibleColor.z, 1);
        glLineWidth(10);
        old_glDrawElements(GL_LINES, count, type, indices);
        glBlendFunc(GL_ONE_MINUS_CONSTANT_COLOR, GL_ONE_MINUS_CONSTANT_ALPHA);
        glDepthRangef(0.49, 1);
        glBlendColor(0, 0, 0, 1);
        old_glDrawElements(GL_TRIANGLES, count, type, indices);
    }

    if (enableRainbow && chamsint != 6 && chamsint != 7) {
        if (red == 255) {
            if (blue == 0) {
                if (gren == 255) {}
                else {
                    gren = gren + 1;
                }
            }
        }
        if (gren == 255) {
            if (red == 0) {}
            else {
                red = red - 1;
            }
        }
        if (gren == 255) {
            if (red == 0) {
                if (blue == 255) {}
                else {
                    blue = blue + 1;
                }
            }
        }
        if (blue == 255) {
            if (gren == 0) {
                mi = 1;
                red = red + 1;
            } else {
                gren = gren - 1;
            }
        }
        if (mi == 1) {
            if (red == 255) {
                if (blue == 0) {}
                else {
                    blue = blue - 1;
                }
            }
        }
        SetR(red / 255);
        SetG(gren / 255);
        SetB(blue / 255);
    }
    //ZEFF: Return to original drawing settings
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendColor(0, 0, 0, 1);
    glLineWidth(1);
    glDepthRangef(0.5, 1);
}

bool mlovinit(){
    handle = NULL;
    handle = dlopen("libGLESv2.so", RTLD_LAZY);
    if(!handle){
        return false;
    }
    return true;
}

void LogShaders(){
    auto p_glGetUniformLocation = (const void*(*)(...))dlsym(handle, "glGetUniformLocation");
    const char *dlsym_error = dlerror();
    if(dlsym_error){
        return;
    }else{
        DobbyHook(reinterpret_cast<void*>(p_glGetUniformLocation), reinterpret_cast<void*>(new_glGetUniformLocation), reinterpret_cast<void**>(&old_glGetUniformLocation));
    }
}

void Wallhack(){
    auto p_glDrawElements = (const void*(*)(...))dlsym(handle, "glDrawElements");
    const char *dlsym_error = dlerror();
    if(dlsym_error){
        return;
    }else{
        DobbyHook(reinterpret_cast<void*>(p_glDrawElements), reinterpret_cast<void*>(new_glDrawElements), reinterpret_cast<void**>(&old_glDrawElements));
    }
}

#endif

