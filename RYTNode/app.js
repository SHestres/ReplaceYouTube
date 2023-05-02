const express = require("express");
const cors = require("cors");
const fs = require("fs")

const app = express();


app.set("view engine", "ejs");

app.use(express.static("public"));
app.use(express.json());

app.use(cors());

app.get("/", (req, res) => {
    res.send("Base api page");
});

app.post('/api/login', (req, res) =>{
    let users = require("./assets/users.json")
    //console.log(users)
    let userByName = users[req.body.username];
    //console.log(userByName);
    let user;
    if(userByName){
        if(userByName.password == req.body.password){
            user = userByName.name;
        }
    }
    //console.log(user);
    res.send({user});
})

app.get('/api/list/:genre', (req, res) => {
    console.log(`Getting list of ${req.params.genre} movies`);
    let data = require("./assets/videos/data/movies.json")
    let filtered = data.filter((video) => {
        return video.Genre.includes(req.params.genre)
    })
    //console.log(filtered)
    res.json(filtered)
})

app.get('/api/video/:id', (req, res) =>{
    let movies = require('./assets/videos/data/movies.json');
    let ytvids;
    let other;
    let video;
    if(!(video = movies.find((vid) => vid.id == req.params.id)))
    {
        ytvids = require('./assets/videos/data/youtube.json');
        if(!(video = ytvids.find((vid) => vid.id == req.params.id))){
            other = require('./assets/videos/data/other.json');
            if(!(video = ytvids.find((vid) => vid.id == req.params.id))){
                res.status(400).send("Unable to find video");
            }
        }
    }
    video.videoUrl = "https://dash.akamaized.net/akamai/bbb_30fps/bbb_30fps.mpd";
    res.json(video);
})

app.get('/api/dash/min', (req, res) => {
    res.sendFile("../../assets/dash/dash.all.min.js")
})

/*
app.get('/api/success', (req, res) =>{
    res.sendFile("../../public/output.h264")
})*/

const usersRouter = require("./routes/users.js");
const videoRouter = require("./routes/videos.js");

app.use("/users", usersRouter);
app.use("/api/videos", videoRouter);


app.listen("5100");
