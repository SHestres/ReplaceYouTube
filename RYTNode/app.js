const express = require("express");
const cors = require("cors");
const fs = require("fs")

const app = express();


app.set("view engine", "ejs");

app.use(express.static("public"));
app.use(express.json());

app.use(cors());


let movies;
let moviespath = './assets/videos/data/movies.json';
let ytVids;
let ytVidspath = './assets/videos/data/youtube.json';
let others;
let otherspath = './assets/videos/data/other.json';

function loadMovies(){
    console.log("Loading movies");
    movies = JSON.parse(fs.readFileSync(moviespath));
}

function loadYtVids(){
    console.log("Loading ytVids")
    ytVids = JSON.parse(fs.readFileSync(ytVidspath));
}

function loadOthers(){
    console.log("Loading others");
    others = JSON.parse(fs.readFileSync(otherspath));
}

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
    let video;

    if(!movies) loadMovies();
    if(!ytVids) loadYtVids();
    if(!others) loadOthers();

    try{
        if(!(video = movies.find((vid) => vid.id == req.params.id)))
        {
            if(!(video = ytvids.find((vid) => vid.id == req.params.id))){
                if(!(video = ytvids.find((vid) => vid.id == req.params.id))){
                    console.log("Unable to find video")
                    res.status(400).send("Unable to find video");
                }
            }
        }
    } catch(err) {console.log("Error loading video library"); res.status(400).send("Error loading video library")}
    
    if(video)
        video.videoUrl = "https://dash.akamaized.net/akamai/bbb_30fps/bbb_30fps.mpd";
    res.json(video);
})

app.post('/api/fav/:id', (req, res) =>{
    console.log("Trying to update favorites");
    
    if(!movies) loadMovies();
    if(!ytVids) loadYtVids();
    if(!others) loadOthers();
    let video;
    let set_val = (req.query.fav.toLowerCase() === 'true');

    if((video = movies.findIndex((vid) => vid.id == req.params.id)) != -1)
    {
        console.log("Found id in movies");
        movies[video].Favorite = set_val;
        fs.writeFile(moviespath, JSON.stringify(movies, null, "\t"), (err) =>{
            console.log("Writing callback");
            if(err){
                console.log("Couldn't write favoriting changes to movies")
                res.status(400).send("Failed to change favorite status")
            }
            else{
                res.status(200).send("Successfully changed favorite status")
            }
        })
    }
    else{
        if((video = ytVids.findIndex((vid) => vid.id == req.params.id)) != -1){
            ytVids[video].Favorite = set_val;
            fs.writeFile(moviespath, JSON.stringify(ytVids, null, "\t"), (err) =>{
                if(err){
                    console.log("Couldn't write favoriting changes to youtubeVids")
                    res.status(400).send("Failed to change favorite status")
                }
                else{
                    res.status(200).send("Successfully changed favorite status")
                }
            })
        }
        else{
            if(!(video = ytvids.findIndex((vid) => vid.id == req.params.id)) != -1){
                others[video].Favorite = set_val;
                fs.writeFile(moviespath, JSON.stringify(others, null, "\t"), (err) =>{
                    if(err){
                        console.log("Couldn't write favoriting changes to others")
                        res.status(400).send("Failed to change favorite status")
                    }
                    else{
                        res.status(200).send("Successfully changed favorite status")
                    }
                })
            }
            else{
                res.status(400).send("Successfully changed favorite status")
            }
        }
    }
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
