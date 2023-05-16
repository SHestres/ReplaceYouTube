const express = require('express');
const path = require('path');

const app = express();

app.use(express.static("C:\\Users\\sidne\\source\\repos\\SHestres\\ReplaceYouTube\\RYTNode\\assets\\videos\\videos"))

app.get('/', (req, res) =>{
    const options = {
        root: path.join(__dirname)
    };

    res.sendFile("stream.html", options);
})

app.get('/video', (req, res) =>{
    console.log("Getting video");
    res.sendFile("C:/Users/sidne/source/repos/SHestres/ReplaceYouTube/RYTServer/dashProgs/output/test_video.mpd");
})

app.listen(80);