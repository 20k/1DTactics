#include <iostream>

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <imgui/misc/freetype/imgui_freetype.h>
#include <imgui/examples/imgui_impl_glfw.h>
#include <imgui/examples/imgui_impl_opengl3.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vec/vec.hpp>
#include <SFML/Graphics.hpp>
#include "renderables.hpp"

//#define GLFW_EXPOSE_NATIVE_WIN32
//#include <glfw/glfw3native.h>

void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

int main(int argc, char* argv[])
{
    bool no_viewports = false;

    if(argc > 1)
    {
        for(int i=1; i < argc; i++)
        {
            std::string sarg = argv[i];

            if(sarg == "-noviewports" || sarg == "-noviewport")
            {
                no_viewports = true;

                printf("Viewports are disabled\n");
            }
        }
    }

    vec2i window_size = {1500, 1000};

    glfwSetErrorCallback(glfw_error_callback);

    if(!glfwInit())
        throw std::runtime_error("Could not init glfw");

    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint( GLFW_DOUBLEBUFFER, GL_FALSE );
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
    //glfwWindowHint(GLFW_SAMPLES, 8);

    glfwWindowHint(GLFW_SRGB_CAPABLE, GLFW_TRUE);

    GLFWwindow* window = glfwCreateWindow(window_size.x(), window_size.y(), "Falling Sand Sim", NULL, NULL);

    if (window == NULL)
        throw std::runtime_error("Nullptr window in glfw");

    glfwMakeContextCurrent(window);

    if(glewInit() != GLEW_OK)
        throw std::runtime_error("Bad Glew");

    ImFontAtlas atlas = {};

    ImGui::CreateContext(&atlas);

    printf("ImGui create context\n");

    ImGuiIO& io = ImGui::GetIO();

    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    if(!no_viewports)
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    //ImGui::SetStyleLinearColor(true);

    ImGui::PushSrgbStyleColor(ImGuiCol_WindowBg, ImVec4(30/255., 30/255., 30/255., 1));

    ImGuiStyle& style = ImGui::GetStyle();

    style.FrameRounding = 0;
    style.WindowRounding = 0;
    style.ChildRounding = 0;
    style.ChildBorderSize = 0;
    style.FrameBorderSize = 0;
    style.WindowBorderSize = 1;

    if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    ImGui::SetStyleLinearColor(true);

    io.Fonts->Clear();
    io.Fonts->AddFontDefault();
    ImGuiFreeType::BuildFontAtlas(&atlas, 0, 1);

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    printf("Init ogl\n");

    /*#ifdef GLFW_EXPOSE_NATIVE_WIN32
    auto native_handle = glfwGetWin32Window(window);
    #endif // GLFW_EXPOSE_NATIVE_WIN32

    #ifdef GLFW_EXPOSE_NATIVE_COCOA
    auto native_handle = glfwGetCocoaWindow(window);
    #endif // GLFW_EXPOSE_NATIVE_COCOA

    #ifdef GLFW_EXPOSE_NATIVE_X11
    auto native_handle = glfwGetX11Window(window);
    #endif // GLFW_EXPOSE_NATIVE_X11

    #ifdef GLFW_EXPOSE_NATIVE_WAYLAND
    static_assert(false);
    #endif // GLFW_EXPOSE_NATIVE_WAYLAND*/

    //glfwMakeContextCurrent(nullptr);

    //sf::ContextSettings sett(24, 8, 0, 3, 0, 0, false);

    /*sf::RenderWindow win(native_handle, sett);
    win.setActive(true);
    win.setVerticalSyncEnabled(true);*/

    render_window win;

    int display_w = 0;
    int display_h = 0;
    int wxpos = 0;
    int wypos = 0;

    glfwGetFramebufferSize(window, &display_w, &display_h);
    glfwGetWindowPos(window, &wxpos, &wypos);

    playspace_manager level;
    level.screen_dimensions = {display_w, display_h};
    level.create_level({1000, 1000}, level_info::GRASS);
    level.camera_pos = {display_w/2, display_h/2};
    level.window_pos = {wxpos, wypos};

    //win.setActive(false);
    //glfwMakeContextCurrent(window);

    bool running = true;

    while(running)
    {
        glfwPollEvents();

        glfwGetFramebufferSize(window, &display_w, &display_h);

        level.screen_dimensions = {display_w, display_h};

        if(glfwWindowShouldClose(window))
            running = false;

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if(ImGui::IsKeyDown(GLFW_KEY_N))
            std::cout << ImGui::GetIO().DeltaTime << std::endl;

        glfwGetWindowPos(window, &wxpos, &wypos);

        level.window_pos = {wxpos, wypos};

        vec2f screen_absolute_pos = {wxpos, wypos};

        /*double mxpos = 0;
        double mypos = 0;
        glfwGetCursorPos(window, &mxpos, &mypos);

        auto mpos = (vec2f){mxpos, mypos};*/

        auto mpos = (vec2f){io.MousePos.x, io.MousePos.y};

        level.tick(mpos, {display_w, display_h}, ImGui::GetIO().DeltaTime);

        {
            /*glfwMakeContextCurrent(nullptr);
            win.setActive(true);
            win.resetGLStates();

            if(display_w != win.getSize().x || display_h != win.getSize().y)
            {
                win.create(native_handle, sett);
                win.setVerticalSyncEnabled(true);
            }

            win.clear();*/

            level.draw(win, mpos);
        }


        ImGui::Begin("Test");

        if(((level.turn % 2) == 0) && ImGui::Button("End Turn"))
        {
            level.next_turn();
        }

        ImGui::End();

        ImGui::Render();

        /*glFinish();
        win.setActive(false);*/
        glfwMakeContextCurrent(window);

        glViewport(0, 0, display_w, display_h);

        glClearColor(0,0,0,1);
        glClear(GL_COLOR_BUFFER_BIT);
        //glDrawBuffer(GL_BACK);
        //glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER, 0);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

        /*glFinish();

        glfwMakeContextCurrent(nullptr);
        win.setActive(true);
        win.resetGLStates();

        win.display();

        win.setActive(false);
        glfwMakeContextCurrent(window);*/
        //glfwMakeContextCurrent(window);
        glfwSwapBuffers(window);
    }

    return 0;
}
