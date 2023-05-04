import React from "react";

export default class VideoPlayer extends React.Component {
  componentDidMount() {
    const { sources, ...options } = this.props;
    window.videojs(this.videoNode, options, function () {
      window.player = this;
      this.updateSrc(sources);
      // this.on('resolutionchange', function () {
      //   console.info('Source changed to %s', player.src());
      // });
    });
  }
  componentWillUnmount() {
    if (window.player) {
      window.player.dispose();
    }
  }
  render() {
    return (
      <div style={{ display: "flex", justifyContent: "center" }}>
        <div style={{ borderRadius: 5, overflow: "hidden" }}>
          <video
            ref={(node) => (this.videoNode = node)}
            className="video-js vjs-default-skin"
          ></video>
        </div>
      </div>
    );
  }
}
