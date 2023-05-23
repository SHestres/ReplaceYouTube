#include "Window.h"
//#include "VideoImporter.h"
#include "fstream"

//For image loading
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using str = std::string;
using namespace nlohmann::literals;

float default_font_size_var;

#define tstr std::to_string

void space(int num) {
    for (int i = 0; i < num; i++)
        ImGui::Spacing();
}

bool loadJsonFile(json* newJson, std::string fileName) {

    try {
        std::ifstream fIn(fileName);
        if (fIn) {
            *newJson = json::parse(fIn);
            return true;
        }
    }
    catch (std::exception err) {
        std::cerr << "Error loading/parsing json file: " << err.what();
        return false;
    }
    return false;
}

bool checkLibraries(json* categories, json* videoLibrary, std::string* message) {
    int broken = 0;
    std::string brokenString = "";

    for (auto& item : categories->items()) {
        bool valid = false;
        try {
            //Check that the current category has at least one valid video entry in the video library
            valid = ((*videoLibrary)[str(item.key())].at(0).contains("Title") && (*videoLibrary)[str(item.key())].at(0).contains("Genre")); }
        catch (std::exception e) {
            valid = false;
        }
        if (!valid) {
            if (broken >= 1) {
                brokenString += ", ";
            }
            brokenString += str(categories->at(item.key()));
            broken++;
        }
    }
    (*message) = broken > 1 ? "!!!WARNING!!! Libraries " : "!!!WARNING!!! Library ";
    (*message) += brokenString;
    (*message) += broken > 1 ? " are empty or invalid or DNE" : " is empty or invalid or DNE";

    return broken == 0;
}

Window::Window(std::string libraryFilename)
{
    strcpy_s(m_libraryFolderpath, libraryFilename.c_str());
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

bool Window::findLibraryFolderPage(char* folderPath, int folderPathMaxLength, json* categoriesObj, json* libObj, bool* persistentBool)
{
    bool didLoad = false;

    Title("Enter path to Library File");

    space(3);

    ImGui::InputTextWithHint("##LibraryPathInput", "Path to Library File", folderPath, 256);
    ImGui::SameLine();
    ImGui::PushTabStop(false);  //False skips the elements
    if (ImGui::Button("Browse"))
    {
        std::string folderNameStr, folderPathStr;
        if (openFolderWithExplorer(&folderNameStr, &folderPathStr))
        {
            strcpy_s(folderPath, folderPathMaxLength, folderPathStr.c_str());
        }
        else
        {
            strcpy_s(folderPath, folderPathMaxLength, "Error. Couldn't get filename");
        }
        *persistentBool = false;
    }
    ImGui::PopTabStop();

    if (ImGui::Button("Submit")) {
        didLoad = loadLibraryFiles(categoriesObj, libObj);
        *persistentBool = true;
    }
    ImGui::SameLine();
    ImGui::Text("Window will freeze while loading. May take up to 60s for large libraries");

    if (*persistentBool) {
        if (!didLoad)
            ImGui::Text("Unable to load library... (check that there is a valid json for each category, and all are located in the data folder)");
        else
            return true;
    }

    return false;
}

bool Window::loadLibraryFiles(json* categories, json* library)
{
    std::string filePath;
    
    try {
        //Load categories file
        filePath = str(m_libraryFolderpath) + CATEGORIES_FILEPATH_EXT;
        if (!loadJsonFile(categories, filePath))
            return false;

        //Validate library folder by loading metedata
        for (auto& item : categories->items())
        {
            std::cout << "Loading " << str(item.key()) + ".json" << std::endl;
            filePath = str(m_libraryFolderpath) + DATA_FILEPATH_EXT + str(item.key()) + ".json";
            json lib;
            if (!loadJsonFile(&lib, filePath)) {
                return false;
            }
            (*library)[str(item.key())] = lib;
        }

    }
    catch (std::exception e) { return false; }

    //Load info from library, but override lib path
    try {
        std::string path = str(m_libraryFolderpath) + IMP_INFO_FILEPATH_EXT;
        loadJsonFile(m_pImpInfo, path);
    }
    catch (std::exception e) {}
    (* m_pImpInfo)["libFolderPath"] = m_libraryFolderpath;

    return true;
    
}

void freeTempPosterImages(json list) {
    for (int i = 0; i < list.size(); i++) {
        try {
            GLuint id = list.at(i)["GLuint"];
            glDeleteTextures(1, &id);
        }
        catch (std::exception e) {}
    }
}

void Window::chooseFromDb(std::string vidTitle, json* resp, json* choice, bool* choosing, bool* madeSelection, bool* postersLoaded) {
    Title("Movie Metadata Selector");

    //Load Posters
    if (!*postersLoaded) {
        try {
            if (str((*resp)["Response"]).find("True") != -1) { //Response found movies
                for (int i = 0; i < (*resp)["Search"].size(); i++) {
                    if (str((*resp)["Search"].at(i)["Poster"]).find("N/A") == -1) { //Has poster
                        GLuint imgRef = 0;
                        float ratio = 1;
                        std::string id = (*resp)["Search"].at(i)["imdbID"];
                        getMoviePosterAsImage(&imgRef, id, &ratio, m_apiKey, m_pPosterIDs);
                        (*resp)["Search"].at(i)["ImgGLuint"] = (int)imgRef;
                        (*resp)["Search"].at(i)["ImgRatio"] = ratio;
                    }
                }
                *postersLoaded = true;
            }
            
        }
        catch (std::exception e) {  }
        
    }

    json selected;

    bool foundMovies = false;
    
    if (ImGui::Button("Return")) {
        *choosing = false;
        *choice = -1;
        freeTempPosterImages((*resp)["Search"]);
        return;
    }

    try {
        if (str((*resp)["Response"]).find("null") != -1) {
            Title("Loading...");
            return;
        }
        foundMovies = str((*resp)["Response"]).find("True") != -1;
    }
    catch (std::exception e) {
        Title("Loading...");
        return;
    }

    if (foundMovies) {

        json vids = (*resp)["Search"];

        for (int i = 0; i < vids.size(); i++) {
            try {
                space(5);
                bool hasPoster = vids.at(i)["ImgGLuint"] > 0;                
                std::string columnKey = "##MovieSelecting" + i;
                ImGui::Columns(2, columnKey.c_str());
                if (hasPoster) { //Display poster
                    GLuint img = vids.at(i)["ImgGLuint"];
                    float ratio = vids.at(i)["ImgRatio"];
                    int height = 100 * DEFAULT_FONT_SIZE;
                    ImGui::SetColumnWidth(0, height * ratio + 20);
                    ImGui::Image((void*)(intptr_t)img, ImVec2(height * ratio, height));
                    ImGui::NextColumn();
                    //ImGui::Text(str(vids.at(i)["ImgGLuint"]).c_str());
                }
                else { //Display no poster message
                    ImGui::SetColumnWidth(0, 300);
                    space(10);
                    Title("No Poster Found", true);
                    space(10);
                    ImGui::NextColumn();
                }
                Title(str(vids.at(i)["Title"]).c_str());
                ImGui::Text(str(vids.at(i)["Year"]).c_str());
                std::string buttonName = "Select##" + std::to_string(i);
                if (ImGui::Button(buttonName.c_str())) {
                    *choosing = false;
                    *choice = i;
                    
                    *madeSelection = true;
                    freeTempPosterImages(vids);
                    selected = vids.at(i);
                    (*choice) = selected;
                    
                    return;
                }
                ImGui::Columns();
            }
            catch (std::exception e) {
                Title("Skipped an element");
            }
        }
    }
    else {
        std::string err = str((*resp)["Error"]);
        Title(err.c_str());
    }
    
    return;
}


void searchDb(std::string vidTitle, json* resp, std::string apiKey) {

    std::string url = "http://www.omdbapi.com";
    std::string ext = "/";

    //Format title
    std::regex space("[[:space:]]");
    std::string vidTitleFormatted = std::regex_replace(vidTitle, space, "%20");

    //Set Headers
    httplib::Headers headers = {
        //{"X-RapidAPI-Key", "ed3bbaa9dbmsh30e8daf3fd5321ap1c95b5jsna49beedcf77c"},
        {"Host", "www.omdbapi.com"}
    };

    //Set Params
    ext += "?s=" + vidTitleFormatted;
    ext += "&type=movie";
    ext += "&apikey=" + apiKey;

    //Get
    httplib::Client db(url);
    db.set_connection_timeout(3, 0);
    if (auto res = db.Get(ext, headers)) {
        if (res->status == 200) {
            //std::cout << res->body << std::endl;
            *resp = json::parse(res->body);
        }
        //std::cout << "Got response" << std::endl;
        //std::cout << "Status: " << res->status << std::endl;
    }
    else {
        auto err = res.error();
        std::cout << "HTTP error: " << httplib::to_string(err) << std::endl;
    }

    return;
}

void getMoviePosterAsImage(GLuint* texRef, std::string id, float* ratio, std::string apiKey, std::vector<GLuint>* posterIDs) {
    
    
    httplib::Client db("http://img.omdbapi.com");
    
    std::string ext = "/";
    ext += "?apikey=" + apiKey;
    ext += "&i=" + id;
    auto res = db.Get(ext);
    

    if (res->status == 200) {
        int cLength = stoi(res->headers.find("Content-Length")->second);
        
        std::vector<unsigned char> buffer;
        buffer.reserve(cLength);
        buffer.assign(res->body.begin(), res->body.end());

        GLuint myImg = 0;
        int imgWidth = 1;
        int imgHeight = 1;
        bool loaded = LoadTextureFromMemory(buffer.data(), cLength, &myImg, &imgWidth, &imgHeight);
        *ratio = (float)imgWidth / (float)imgHeight;
        
        if (!loaded) {
            //std::cout << "Couldn't load" << std::endl;
            myImg = 0;
        }
        else {
            //std::cout << "Loaded" << std::endl;
            posterIDs->push_back(myImg);
        }
        *texRef = myImg;
        return;
    }

    *texRef = 0;
    return;
}

json Window::fetchMovieData(json movie) {

    json out;

    std::string url = "http://www.omdbapi.com";
    std::string ext = "/";
    std::string id = "";
    try {
        id = movie["imdbID"];
    }
    catch (std::exception e) {}

    //Set Headers
    httplib::Headers headers = {
        //{"X-RapidAPI-Key", "ed3bbaa9dbmsh30e8daf3fd5321ap1c95b5jsna49beedcf77c"},
        {"Host", "www.omdbapi.com"}
    };

    //Set Params
    ext += "?i=" + id;
    ext += "&type=movie";
    ext += "&apikey=" + m_apiKey;

    //Get
    httplib::Client db(url);
    db.set_connection_timeout(3, 0);
    if (auto res = db.Get(ext, headers)) {
        if (res->status == 200) {
            std::cout << res->body << std::endl;
            out = json::parse(res->body);
        }
        //std::cout << "Got response" << std::endl;
        //std::cout << "Status: " << res->status << std::endl;
    }
    else {
        auto err = res.error();
        std::cout << "HTTP error: " << httplib::to_string(err) << std::endl;
    }

    return out;

}

void Window::Run()
{
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    //Persistent Window Variables
    bool closeable = false;
    bool showInput = false;
    bool textEntered = false;

    //Video input vars
    vidStat videoStats = IDLE;

    bool submitLibraryPath = false;
    char filePath[MAX_FILEPATH_LENGTH];
    ZeroMemory(filePath, MAX_FILEPATH_LENGTH);

    char vidName[MAX_VIDEO_TITLE_LENGTH];
    //char textEntry[MAX_VIDEO_TITLE_LENGTH];
    //ZeroMemory(textEntry, MAX_VIDEO_TITLE_LENGTH);
    ZeroMemory(vidName, MAX_VIDEO_TITLE_LENGTH);

    char genreList[MAX_VIDEO_GENRE_LENGTH];
    char description[MAX_VIDEO_DESCRIPTION_LENGTH];
    ZeroMemory(genreList, MAX_VIDEO_GENRE_LENGTH);
    ZeroMemory(description, MAX_VIDEO_DESCRIPTION_LENGTH);

    bool isFavorite = false;

    //Database loading vars
    bool choosingFromDb = false;
    bool postersLoaded = false;
    bool hasMadeSelection = false;
    json dbResp;
    json selectedMovie;
    std::future<void> dbFuture;
    std::vector<GLuint> posters;
    m_pPosterIDs = &posters;

    int fetchInfoDelay = 0;
    json selectedMovieData;

    //Optional input vars
    float vidRating = 0;
    char actorList[MAX_ACTOR_LIST_LENGTH];
    ZeroMemory(actorList, MAX_ACTOR_LIST_LENGTH);
    int releaseYear = 2000;
    char ageRating[20];
    ZeroMemory(ageRating, 20);

    DashPackager packager;

    //Library tab vars
    int packageStep = 0;
    std::string errorMsg;
    std::future<void> packageFuture;
    bool goToLibrary = false;
    json loadingVids;
    /*loadingVids = json::object({
        {}
        });
        */

    //Try to load previous library info (all but lib path will be overridden by info file in lib)
    json importerInfo;
    m_pImpInfo = &importerInfo;
    if (loadJsonFile(&importerInfo, "importerInfo.json")) {
        try {
            strcpy_s(m_libraryFolderpath, str(importerInfo["libFolderPath"]).c_str());
        }
        catch (std::exception e) {};
    }

    json videoLibrary;
    json categories;
    bool didLoadLibraryFiles = loadLibraryFiles(&categories, &videoLibrary);//loadJsonFile(&videoLibrary, m_libraryFilepath);


    //Testing


    //Set font size
    int displayX, displayY;
    GetMaxMonitorResolution(&displayX, &displayY);
    DEFAULT_FONT_SIZE = (float)displayY / 800;

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
            
            //Window Styling
            ImGui::SetWindowFontScale(DEFAULT_FONT_SIZE);

            //Possible other screens
            {
                if (!didLoadLibraryFiles) {
                    didLoadLibraryFiles = findLibraryFolderPage(m_libraryFolderpath, MAX_FILEPATH_LENGTH, &categories, &videoLibrary, &submitLibraryPath);
                    goto endWindow;
                }

                if (choosingFromDb) {
                    chooseFromDb(vidName, &dbResp, &selectedMovie, &choosingFromDb, &hasMadeSelection, &postersLoaded);
                    goto endWindow;
                }

                if (fetchInfoDelay > 0) {
                    if (fetchInfoDelay == 1) {
                        selectedMovieData = fetchMovieData(selectedMovie);
                        if (!selectedMovieData.is_null()) {
                            try {
                                strcpy_s(vidName, str(selectedMovieData["Title"]).c_str());
                                strcpy_s(description, str(selectedMovieData["Plot"]).c_str());
                                strcpy_s(genreList, str(selectedMovieData["Genre"]).c_str());
                                strcpy_s(actorList, str(selectedMovieData["Actors"]).c_str());
                                releaseYear = stoi(str(selectedMovieData["Year"]));
                                vidRating = stoi(str(selectedMovieData["imdbRating"]));
                            }
                            catch (std::exception e) {}
                        }
                    }
                    else{
                        space(20);
                        Title("Loading Movie Data...");
                        space(20);
                    }
                    fetchInfoDelay--;
                    goto endWindow;
                }
            }


            
            //Main Program
            ImGui::BeginTabBar("TabBar");

            if(ImGui::BeginTabItem("Import Videos"))
            {

                //Check that library files are valid
                bool libraryIsValid = false;
                std::string warningMessage = "";
                try {
                    libraryIsValid = checkLibraries(&categories, &videoLibrary, &warningMessage);
                }
                catch (std::exception e) { 
                    libraryIsValid = false; 
                    warningMessage = "!!!WARNING!!! Unable to check library file validity. Likely invalid";
                }

                if (!libraryIsValid) {
                    space(10);
                    ImGui::TextColored(ImVec4(1, 0, 0, 1), warningMessage.c_str());
                    space(10);
                }
                
                //Rest of GUI
                ImGui::Text("Video File to import and encode");

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

                space(5);

                ImGui::Text("Video Title");
                if (ImGui::InputTextWithHint("##VideoTitleInput", "Video Title", vidName, MAX_VIDEO_TITLE_LENGTH, ImGuiInputTextFlags_EnterReturnsTrue))
                {
                    //textEntered = true;
                    //strcpy_s(vidName, textEntry);
                }
                //if (textEntered)
                    //ImGui::Text("Video will take the name \"%s\"", vidName);

                space(3);

                if (ImGui::Button("Search for Metadata")) {
                    choosingFromDb = true;
                    postersLoaded = false;
                    hasMadeSelection = false;
                    //std::cout << "VidName: " << vidName << std::endl;
                    dbResp["Response"] = NULL;
                    dbFuture = std::async(std::launch::async, searchDb, vidName, &dbResp, m_apiKey);
                }
                else if (hasMadeSelection) {

                    //TODO: Load and save data for selected movie
                    fetchInfoDelay = 2;
                    hasMadeSelection = false;
                }


                space(5);
                ImGui::Checkbox("Favorite?", &isFavorite);



                space(5);

                ImGui::Text("Genre List (Format matters)");
                ImGui::InputTextWithHint("##GenreList", "Action, Drama, Romance", genreList, MAX_VIDEO_GENRE_LENGTH);

                space(10);

                Title("Optional Parameters");

                space(5);

                ImGui::Text("Video Rating");
                ImGui::SetNextItemWidth(200);
                ImGui::SliderFloat("(0.0 saves as unrated)", &vidRating, 0, 10, "%.1f");

                space(5);

                ImGui::Text("Actor List");
                ImGui::InputTextWithHint("##ActorList", "Gal Gadot, Ryan Reynolds, Dwayne Johnson", actorList, MAX_ACTOR_LIST_LENGTH);

                space(5);

                ImGui::Text("Release Year");

                ImGui::SetNextItemWidth(200);
                ImGui::InputInt("##ReleaseYear", &releaseYear, 1, 10);
                ImGui::SameLine();
                ImGui::Text("Ctrl click to step by 10");

                space(5);

                ImGui::Text("Description / Plot");
                ImGui::InputText("##VideoDescription", description, MAX_VIDEO_DESCRIPTION_LENGTH);
                space(3);
                ImGui::Text("(Readable Version)");
                space(2);
                ImGui::TextWrapped(description);

                space(3);

                ImGui::SetWindowFontScale(TITLE_FONT_SIZE);
                if (ImGui::Button("Import")) {
                    std::cout << "Testing: " << tstr(1234) << std::endl;
                    packager.Init(filePath, m_libraryFolderpath, tstr(1234));
                    packageFuture = packager.Run(&videoStats, &packageStep, &errorMsg);
                    goToLibrary = true;
                }
                ImGui::SetWindowFontScale(DEFAULT_FONT_SIZE);

                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Server Info"))
            {
                Title("Nothing to see here yet! :)");
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Library", 0, goToLibrary ? ImGuiTabItemFlags_SetSelected : 0))
            {
                goToLibrary = false;
                Title("Access your Library here");
                if (ImGui::Button("Change library file")) {
                    submitLibraryPath = false;
                    didLoadLibraryFiles = false;
                }
                space(10);
                if (ImGui::Button("Next Step")) {
                    packageStep += 1;
                }
                ImGui::Text("Step %d", packageStep);
                space(10);
                std::string text = "Video Status: " + str(EnumStrings[videoStats]);
                ImGui::Text(text.c_str());
                std::string err = "Error Message: " + errorMsg;
                ImGui::Text(err.c_str());
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Testing")) {                

                /* 
                //Testing loading json files with nlohmann
                json test;

                if(loadJsonFile(&test, "test.json"))
                {
                    ImGui::Text(str(test.at(0)["name"]).c_str());
                    ImGui::Text(str(test.at(1)["name"]).c_str());
                    ImGui::Text(str(test.at(2)["name"]).c_str());

                    //When creating a json::object, the outer set of brackets creates the object. Each inner pair of brackets becomes a key:value pair.
                    //{ { "name", "fourth"} } is equivalent to {"name: "fourth"} in js.
                    json obj = json::object({ { "name", "fourth"} });
                    
                    test.push_back(obj);

                    ImGui::Text(str(test.at(3)["name"]).c_str());
                }
                */

                /*
                json test = json::object({
                    {"first", "number 1"},
                    {"second", "number 2"},
                    {"third", "number 3"}
                    });

                //Allows for iterating over items with key and value
                for (auto& item : test.items()) {
                    ImGui::Text(str(test[item.key()]).c_str());
                }

                //Allows for iterating over just the values
                for (auto item : test) {
                    ImGui::Text(str(item).c_str());
                }
                */

                /*
                json test;
                std::string testString = "First Test";
                test["Test"] = testString;
                ImGui::Text(str(test["Test"]).c_str());
                testString = "Second Test";
                ImGui::Text(str(test["Test"]).c_str());
                test["Test"] = testString;
                ImGui::Text(str(test["Test"]).c_str());
                //Should produce First Test, First Test, Second Test
                */

                //Image loading test
                //ImGui::Text("size = %d x %d", my_image_width, my_image_height);
                //ImGui::Image((void*)(intptr_t)my_image_texture, ImVec2(200, 200));


                ImGui::EndTabItem();
            }


            ImGui::EndTabBar();
            
endWindow:            
            ImGui::End();
        }

render:
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

    std::ofstream infoSave("importerInfo.json");
    infoSave << importerInfo;
    infoSave.flush();
    infoSave.close();

    std::string infoPath = str(m_libraryFolderpath) + IMP_INFO_FILEPATH_EXT;
    std::ofstream libInfoBackup(infoPath);
    libInfoBackup << importerInfo;
    libInfoBackup.flush();
    infoSave.close();
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

void GetMaxMonitorResolution(int* w, int* h)
{
    int count;
    const GLFWvidmode* modes = glfwGetVideoModes(glfwGetPrimaryMonitor(), &count);

    int maxWidth = 0;
    int maxHeight = 0;
    for (int i = 0; i < count; i++)
    {
        if (modes[i].width > maxWidth)
            maxWidth = modes[i].width;
        if (modes[i].height > maxHeight)
            maxHeight = modes[i].height;
    }

    *w = maxWidth;
    *h = maxHeight;
}

void Title(const char* title, bool wrap, float multiplier)
{
    if (multiplier > 0) ImGui::SetWindowFontScale(DEFAULT_FONT_SIZE * multiplier);
    else ImGui::SetWindowFontScale(TITLE_FONT_SIZE);
    if (wrap)
        ImGui::TextWrapped(title);
    else
        ImGui::Text(title);
    ImGui::SetWindowFontScale(DEFAULT_FONT_SIZE);
}

// Simple helper function to load an image into a OpenGL texture with common settings
bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height)
{
    // Load from file
    int image_width = 0;
    int image_height = 0;
    unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
    if (image_data == NULL)
        return false;

    // Create a OpenGL texture identifier
    GLuint image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

    // Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    stbi_image_free(image_data);

    *out_texture = image_texture;
    *out_width = image_width;
    *out_height = image_height;

    return true;
}

bool LoadTextureFromMemory(stbi_uc *buffer, int bufLen, GLuint* out_texture, int* out_width, int* out_height)
{
    // Load from file
    int image_width = 0;
    int image_height = 0;
    unsigned char* image_data = stbi_load_from_memory(buffer,bufLen, &image_width, &image_height, NULL, 4);
    if (image_data == NULL)
        return false;

    // Create a OpenGL texture identifier
    GLuint image_texture = 0;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

    // Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    stbi_image_free(image_data);

    *out_texture = image_texture;
    *out_width = image_width;
    *out_height = image_height;

    return true;
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

bool openFolderWithExplorer(std::string* SelectedFolder, std::string* FolderPath)
{
    // CREATE FileOpenDialog OBJECT
    HRESULT f_SysHr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    if (FAILED(f_SysHr))
        return FALSE;

    IFileOpenDialog* f_FileSystem;
    f_SysHr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&f_FileSystem));
    if (FAILED(f_SysHr)) {
        CoUninitialize();
        return FALSE;
    }

    // SET OPTIONS TO ALLOW FOLDERS ONLY
    DWORD dwOptions;
    f_FileSystem->GetOptions(&dwOptions);
    f_FileSystem->SetOptions(dwOptions | FOS_PICKFOLDERS);

    // SHOW THE DIALOG
    f_SysHr = f_FileSystem->Show(NULL);
    if (FAILED(f_SysHr)) {
        f_FileSystem->Release();
        CoUninitialize();
        return FALSE;
    }

    // GET THE SELECTED FOLDER
    IShellItem* f_Folder;
    f_SysHr = f_FileSystem->GetResult(&f_Folder);
    if (FAILED(f_SysHr)) {
        f_FileSystem->Release();
        CoUninitialize();
        return FALSE;
    }

    // RETRIEVE THE FOLDER PATH
    PWSTR f_Path;
    f_SysHr = f_Folder->GetDisplayName(SIGDN_FILESYSPATH, &f_Path);
    if (FAILED(f_SysHr)) {
        f_Folder->Release();
        f_FileSystem->Release();
        CoUninitialize();
        return FALSE;
    }

    // STORE THE FOLDER PATH
    std::wstring path(f_Path);
    std::string c(path.begin(), path.end());
    *FolderPath = c;

    // FORMAT STRING FOR FOLDER NAME
    const size_t slash = FolderPath->find_last_of("/\\");
    *SelectedFolder = FolderPath->substr(slash + 1);

    // SUCCESS, CLEAN UP
    CoTaskMemFree(f_Path);
    f_Folder->Release();
    f_FileSystem->Release();
    CoUninitialize();
    return TRUE;
}

