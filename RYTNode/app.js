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
    let data = require("./assets/movies/movies.json")
    let filtered = data.filter((video) => {
        return video.Genre.includes(req.params.genre)
    })
    console.log(filtered)
    res.json(filtered)
})

app.get('/api/genres', (req, res)=>{
    //console.log("Getting genres internal");
    const getFile = fs.readFileSync('./assets/genres.json')
    //console.log(JSON.parse(getFile));
    res.json(JSON.parse(getFile))
})

app.get('/api/video/:id', (req, res) =>{
    res.json({videoUrl : "https://dash.akamaized.net/akamai/bbb_30fps/bbb_30fps.mpd"});
})

app.get('/api/dash/min', (req, res) => {
    res.sendFile("../../assets/dash/dash.all.min.js")
})

/*
app.get('/api/success', (req, res) =>{
    res.sendFile("../../public/output.h264")
})*/

const usersRouter = require("./routes/users.js");

app.use("/users", usersRouter);


app.listen("5100");
