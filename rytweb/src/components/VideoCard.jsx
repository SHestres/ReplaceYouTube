import { useState, useEffect } from "react";
import { BsPlayFill, BsChevronUp } from 'react-icons/bs';
import { Link } from "react-router-dom";
import FavIcon from "./FavIcon";

export default function VideoCard({Title, id, Images, ...rest}) {

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
                <div className="video-card-row">
                <FavIcon className='video-card-btn' id={id} {...rest}/>
                <Link to={`/play/${id}`}>
                    <BsPlayFill className="video-card-btn"/>
                </Link>
                </div>
            </div>
        </div>)
}