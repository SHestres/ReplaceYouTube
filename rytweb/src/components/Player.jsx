//import {MediaPlayer, initialize} from 'dashjs'
import React from 'react';
import { useEffect } from 'react';
import { useState } from 'react';

import { BsFullscreen } from 'react-icons/bs'

//import ScriptTag from 'react-script-tag'

import dashjs from 'dashjs'
import ControlBar from '../../node_modules/dashjs/contrib/akamai/controlbar/ControlBar'
import "../../node_modules/dashjs/contrib/akamai/controlbar/ControlBar.css"

export default function Player({videoUrl}){

    // A hosted sample video for testing
    //console.log("https://dash.akamaized.net/akamai/bbb_30fps/bbb_30fps.mpd")

    //[player, setPlayer] = useState();

    //const player = dashjs.MediaPlayer().create();
    var view;

    /*
    useEffect( ()=>{
        const player = dashjs.MediaPlayer().create()
        view = document.querySelector("#videoPlayer")
        console.log(view);
        player.initialize(view, "https://dash.akamaized.net/akamai/bbb_30fps/bbb_30fps.mpd", true);

        const controlBar = new ControlBar(player);
        controlBar.initialize();
    }, []);    
*/

    useEffect(() => {
        const script = document.createElement('script');
        script.src = `http://${window.location.host.split(":", 1)[0]}:5100/ControlBar.js`;
        //script.async = true;
        document.body.appendChild(script);
      return () => {
          document.body.removeChild(script);
        }
      }, []);

      useEffect(() => {
        const script = document.createElement('script');
        script.textContent = "\
            function init() {\
            var url = 'https://dash.akamaized.net/akamai/bbb_30fps/bbb_30fps.mpd';\
            var videoElement = document.querySelector('.videoContainer video');\
            var player = dashjs.MediaPlayer().create();\
            player.initialize(videoElement, url, true);\
            var controlbar = new ControlBar(player);\
            controlbar.initialize();}"
        //script.async = true;
        document.body.appendChild(script);
      return () => {
          document.body.removeChild(script);
        }
      }, []);

    return(
        <div>
            
            <h1>Video Player</h1>
            <video id='videoPlayer' />
            <div id="videoController" className="video-controller unselectable">
                <div id="playPauseBtn" className="btn-play-pause" title="Play/Pause">
                    <span id="iconPlayPause" className="icon-play"></span>
                </div>
                <span id="videoTime" className="time-display">00:00:00</span>
                <div id="fullscreenBtn" className="btn-fullscreen control-icon-layout" title="Fullscreen">
                    <BsFullscreen className="icon-fullscreen-enter" />
                </div>
                <div id="bitrateListBtn" className="control-icon-layout" title="Bitrate List">
                    <span className="icon-bitrate"></span>
                </div>
                <input type="range" id="volumebar" className="volumebar" value="1" min="0" max="1" step=".01"/>
                <div id="muteBtn" className="btn-mute control-icon-layout" title="Mute">
                    <span id="iconMute" className="icon-mute-off"></span>
                </div>
                <div id="trackSwitchBtn" className="control-icon-layout" title="A/V Tracks">
                    <span className="icon-tracks"></span>
                </div>
                <div id="captionBtn" className="btn-caption control-icon-layout" title="Closed Caption">
                    <span className="icon-caption"></span>
                </div>
                <span id="videoDuration" className="duration-display">00:00:00</span>
                <div className="seekContainer">
                    <input type="range" id="seekbar" value="0" className="seekbar" min="0" step="0.01"/>
                </div>
            </div>
        </div>
    )
}


/*
import videojs from 'video.js';
import 'video.js/dist/video-js.css';
import { useRef, useEffect } from 'react';
//import "videojs-resolution-switcher";

export default function Player({videoUrl}){

    const videoRef = useRef(null);
    const playerRef = useRef(null);
    //const {options, onReady} = props;

    const options = {
        autoplay: true,
        controls: true,
        responsive: true,
        fluid: true,
        plugins: {
            videoJsResolutionSwitcher: {
              default: 'high',
              dynamicLabel: true
            }
        },
        sources: [{
          src: 'http://localhost:5100/success.mp4',
          type: 'video/mp4',
          label: 'sd',
          res: 480,
        },
        {
            src: 'http://localhost:5100/success.mp4',
            type: 'video/mp4',
            label: 'hd',
            res: 480,
        }]
      };

      const onReady = (player) => {
        playerRef.current = player;
      }

    useEffect(() => {

        // Make sure Video.js player is only initialized once
        if (!playerRef.current) {
        // The Video.js player needs to be _inside_ the component el for React 18 Strict Mode. 
        const videoElement = document.createElement("video-js");

        videoElement.classList.add('vjs-big-play-centered');
        videoRef.current.appendChild(videoElement);

        const player = playerRef.current = videojs(videoElement, options, () => {
            videojs.log('player is ready');
            onReady && onReady(player);
        });

        // You could update an existing player in the `else` block here
        // on prop change, for example:
        } else {
        const player = playerRef.current;

        player.autoplay(options.autoplay);
        player.src(options.sources);
        }
    }, [options, videoRef]);


    useEffect(() => {
        const player = playerRef.current;
    
        return () => {
          if (player && !player.isDisposed()) {
            player.dispose();
            playerRef.current = null;
          }
        };
    }, [playerRef]);
    
    return (
        <div data-vjs-player>
          <div ref={videoRef} />
        </div>
    );

}
*/

/*
import VideoPlayer from "./VideoPlayerFunction";
import 

export default function Player(){
    const videoJsOptions = {
        autoplay: false,
        playbackRates: [0.5, 1, 1.5, 2],
        width: 640,
        height: 360,
        controls: true,
        plugins: {
          videoJsResolutionSwitcher: {
            default: "height",
            dynamicLabel: true
          }
        },
        sources: [
          {
            src: "https://vjs.zencdn.net/v/oceans.mp4?hd",
            type: "video/mp4",
            label: "720",
            res: 720
          },
          {
            src: "https://vjs.zencdn.net/v/oceans.mp4?sd",
            type: "video/mp4",
            label: "480",
            res: 480
          }
        ]
    };
      
    return (
        <div>
            <VideoPlayer {...videoJsOptions} />
        </div>
    )
}


*/