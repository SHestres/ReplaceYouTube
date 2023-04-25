const express = require("express");

const app = express();

app.set("view engine", "ejs");

app.use(express.static("public"));

app.get("/", (req, res) => {
    res.send("Hi. Welcome to the base page");
});

app.get("/render", (req, res) => {
    res.render("../index", {text: "World"});
});

app.get("/render/gpt", (req, res) => {
    res.render("../GPT_vid", {
        videoTitle: 'My Video Title',
        videoPath: 'path/to/video.mp4',
        videoDescription: 'Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed at tincidunt nulla, nec molestie arcu. Phasellus consequat mauris in posuere tristique. Aenean posuere magna at diam vehicula, id egestas velit placerat.'
    });
});

const usersRouter = require("./routes/users.js");

app.use("/users", usersRouter);


app.listen("3000");
