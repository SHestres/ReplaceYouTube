import React, { useEffect, useRef } from "react";
import videojs from 'video.js'

export default function VideoPlayer(props) {
  const videoRef = useRef(null);

  useEffect(() => {
    const { sources, ...options } = props;
    videojs(videoRef.current, options, function () {
      this.player = this;
      this.updateSrc(sources);
    });
    return () => {
      this.player && this.player.dispose();
    };
  });

  return (
    <div style={{ display: "flex", justifyContent: "center" }}>
      <div style={{ borderRadius: 10, overflow: "hidden" }}>
        <video ref={videoRef} className="video-js vjs-default-skin"></video>
      </div>
    </div>
  );
}
