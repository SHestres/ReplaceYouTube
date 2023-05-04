#include "Window.h"
//#include "VideoImporter.h"
#include "json.hpp"
#include "fstream"

using str = std::string;
using json = nlohmann::json;
using namespace nlohmann::literals;

float default_font_size_var;

#define tstr to_string

void space(int num) {
    for (int i = 0; i < num; i++)
        ImGui::Spacing();
}

void loadJsonFile(json* newJson, std::string fileName) {
    *newJson = R"({"isValidJSO": false})"_json;

    try {
        std::ifstream fIn(fileName);
        if (fIn) {
            *newJson = json::parse(fIn);
            (*newJson)["isValidJSO"] = true;
        }
    }
    catch (std::exception err) {
        std::cerr << "Error: " << err.what();
    }
}

Window::Window()
{

}

Window::~Window()
{
    Cleanup();
}

int Window::OpenWindow(const char* WindowTitle)
{
    ChooseGLFWVersionForPlatform();
    if (!glfwInit()) return -1;
    if (InitImGui(WindowTitle) != 0)
    {
        std::cerr << "Error initializing imgui" << std::endl;
        return -1;
    }

}

void Window::Run()
{
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    //Persistent Window Variables
    bool closeable = false;
    bool showInput = false;
    bool textEntered = false;

    char vidName[MAX_VIDEO_TITLE_LENGTH];
    char textEntry[MAX_VIDEO_TITLE_LENGTH];
    ZeroMemory(textEntry, MAX_VIDEO_TITLE_LENGTH);

    char genreList[MAX_VIDEO_GENRE_LENGTH];
    char description[MAX_VIDEO_DESCRIPTION_LENGTH];
    ZeroMemory(genreList, MAX_VIDEO_GENRE_LENGTH);
    ZeroMemory(description, MAX_VIDEO_DESCRIPTION_LENGTH);

    //Video Metadata vars
    int numVidsDataShown = 1;


    char filePath[256];
    ZeroMemory(filePath, 256);

    std::cout << "Display x: " << m_pio->DisplaySize.x << " y: " << m_pio->DisplaySize.y << std::endl;
    default_font_size_var = 2;// (float)m_pio->DisplaySize.y / 1080;

    while (!glfwWindowShouldClose(m_window))
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        //Load in json files
        std::ifstream f("test.json");
        json j = json::parse(f);

        //DemoWindows(); continue;
        
        //My Window
        {
            //Set to fullscreen
            ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->Pos);
            ImGui::SetNextWindowSize(viewport->Size);

            //Begin
            ImGui::Begin("Importer", &closeable, flags);
            ImGui::BeginTabBar("TabBar");

            if(ImGui::BeginTabItem("Import Videos"))
            {
                //Window Styling
                ImGui::SetWindowFontScale(DEFAULT_FONT_SIZE);

                //Window Body
                Title("Video File to import and encode");

                ImGui::InputTextWithHint("##FilePathInput", "Path to Video File", filePath, 256);
                ImGui::SameLine();
                ImGui::PushTabStop(false);  //False skips the elements
                if (ImGui::Button("Browse"))
                {
                    std::string fileNameStr, filePathStr;
                    if (openFileWithExplorer(&fileNameStr, &filePathStr))
                    {
                        strcpy_s(filePath, filePathStr.c_str());
                    }
                    else
                    {
                        strcpy_s(filePath, "Error. Couldn't get filename");
                    }
                }
                ImGui::PopTabStop();

                space(3);

                Title("Video Title");
                if (ImGui::InputTextWithHint("##VideoTitleInput", "Video Title", textEntry, MAX_VIDEO_TITLE_LENGTH, ImGuiInputTextFlags_EnterReturnsTrue))
                {
                    textEntered = true;
                    strcpy_s(vidName, textEntry);
                }
                if (textEntered)
                    ImGui::Text("Video will take the name \"%s\"", vidName);

                space(3);

                Title("Genre List (Format matters)");
                ImGui::InputTextWithHint("##GenreList", "Action, Drama, Romance", genreList, MAX_VIDEO_GENRE_LENGTH);
                
                space(3);
                
                Title("Description");
                ImGui::InputTextMultiline("##VideoDescription", description, MAX_VIDEO_DESCRIPTION_LENGTH);



                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Server Info"))
            {
                Title("Nothing to see here yet! :)");
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Library"))
            {
                

                Title("Access your Library here");

                
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Testing")) {                

                json test;

                loadJsonFile(&test, "test.json");

                if(test["isValidJSO"] == true){
                    ImGui::Text(str(test["name"]).c_str());
                }
                
                ImGui::EndTabItem();
            }


            ImGui::EndTabBar();
            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(m_window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(m_window);

    }
}

void Window::DemoWindows()
{
    //State for demo
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
    {
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

        ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
        ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
        ImGui::Checkbox("Another Window", &show_another_window);

        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

        if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / m_pio->Framerate, m_pio->Framerate);
        ImGui::End();
    }

    // 3. Show another simple window.
    if (show_another_window)
    {
        ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        ImGui::Text("Hello from another window!");
        if (ImGui::Button("Close Me"))
            show_another_window = false;
        ImGui::End();
    }

    // Rendering
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(m_window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(m_window);
}

void Window::Cleanup()
{
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(m_window);
    glfwTerminate();
}

int Window::InitImGui(const char* WindowTitle)
{
    // Create window with graphics context
    m_window = glfwCreateWindow(1280, 720, WindowTitle, NULL, NULL);
    if (m_window == NULL)
        return -1;
    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    m_pio = &io;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    return 0;

}

void Window::ChooseGLFWVersionForPlatform()
{

    // [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
    // To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
    // Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
    #if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
    #pragma comment(lib, "legacy_stdio_definitions")
    #endif

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.3 + GLSL 130
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif
}

void Title(const char* title, float multiplier)
{
    if (multiplier > 0) ImGui::SetWindowFontScale(default_font_size_var * multiplier);
    else ImGui::SetWindowFontScale(TITLE_FONT_SIZE);
    ImGui::Text(title);
    ImGui::SetWindowFontScale(DEFAULT_FONT_SIZE);
}

//Get File with Explorer
//std::string sSelectedFile;
//std::string sFilePath;
bool openFileWithExplorer(std::string* SelectedFile, std::string* FilePath)
{
    //  CREATE FILE OBJECT INSTANCE
    HRESULT f_SysHr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    if (FAILED(f_SysHr))
        return FALSE;

    // CREATE FileOpenDialog OBJECT
    IFileOpenDialog* f_FileSystem;
    f_SysHr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&f_FileSystem));
    if (FAILED(f_SysHr)) {
        CoUninitialize();
        return FALSE;
    }

    //  SHOW OPEN FILE DIALOG WINDOW
    f_SysHr = f_FileSystem->Show(NULL);
    if (FAILED(f_SysHr)) {
        f_FileSystem->Release();
        CoUninitialize();
        return FALSE;
    }

    //  RETRIEVE FILE NAME FROM THE SELECTED ITEM
    IShellItem* f_Files;
    f_SysHr = f_FileSystem->GetResult(&f_Files);
    if (FAILED(f_SysHr)) {
        f_FileSystem->Release();
        CoUninitialize();
        return FALSE;
    }

    //  STORE AND CONVERT THE FILE NAME
    PWSTR f_Path;
    f_SysHr = f_Files->GetDisplayName(SIGDN_FILESYSPATH, &f_Path);
    if (FAILED(f_SysHr)) {
        f_Files->Release();
        f_FileSystem->Release();
        CoUninitialize();
        return FALSE;
    }

    //  FORMAT AND STORE THE FILE PATH
    std::wstring path(f_Path);
    std::string c(path.begin(), path.end());
    *FilePath = c;

    //  FORMAT STRING FOR EXECUTABLE NAME
    const size_t slash = FilePath->find_last_of("/\\");
    *SelectedFile = FilePath->substr(slash + 1);

    //  SUCCESS, CLEAN UP
    CoTaskMemFree(f_Path);
    f_Files->Release();
    f_FileSystem->Release();
    CoUninitialize();
    return TRUE;
}
