add_library(
        imgui

        imconfig.h
        imgui_internal.h
        imstb_rectpack.h
        imstb_textedit.h
        imstb_truetype.h

        imgui.cpp imgui.h

        imgui_demo.cpp
        imgui_draw.cpp
        imgui_tables.cpp
        imgui_widgets.cpp

        imgui_impl_sdl2.cpp imgui_impl_sdl2.h
        imgui_impl_sdlrenderer2.cpp imgui_impl_sdlrenderer2.h
        )