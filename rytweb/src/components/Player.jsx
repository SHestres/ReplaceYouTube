//import {MediaPlayer, initialize} from 'dashjs'
import React from 'react';
import dashjs from 'dashjs'


export default function({videoUrl}){

    // A hosted sample video for testing
    //console.log("https://dash.akamaized.net/akamai/bbb_30fps/bbb_30fps.mpd")

    const player = dashjs.MediaPlayer().create();
    player.initialize(document.querySelector("#videoPlayer"), "https://dash.akamaized.net/akamai/bbb_30fps/bbb_30fps.mpd", true);


    return(
        <div>
            <h1>Video Player</h1>
            <video id='videoPlayer' controls/>
        </div>
    )
}
