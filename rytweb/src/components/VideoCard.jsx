import { useState, useEffect } from "react";
import { BsPlayFill, BsChevronUp } from 'react-icons/bs';
import { Link } from "react-router-dom";


export default function VideoCard({Title, id, Plot, vote_average, poster, Images }) {

    const [style, setStyle] = useState({});
    const [backHeight, setBackHeight] = useState();
    const [backWidth, setBackWidth] = useState();

    const getMeta = (url, cb) => {
        const img = new Image();
        img.onload = () => cb(null, img);
        img.onerror = (err) => cb(err);
        img.src = url;
      };
    
    useEffect(() => {
        getMeta(`${Images[0]}`, (err, img) =>
        {
        console.log("Updating image ratio");
        setBackWidth(img.naturalWidth);
        setBackHeight(img.naturalHeight);
        })
    });
    
    useEffect(() =>
    {
        setStyle({
            '--backImg': `url(${Images[0]})`,
            '--backWidth': `${backWidth}`,
            '--backHeight': `${backHeight}`,
        });
    }, [backHeight, backWidth])

    return (
        <div className="video-card" style={style}>
            <div className="video-details">
                <h4 className="video-title">{Title}</h4>
                <Link to={`/play/${id}`}>
                    <BsPlayFill className="play-btn"/>
                </Link>
            </div>
        </div>)
}