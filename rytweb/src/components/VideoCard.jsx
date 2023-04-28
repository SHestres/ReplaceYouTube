import { useState, useEffect } from "react";
import {uniqueId} from 'lodash'

export default function VideoCard({Title, id, vote_average, poster, Images }) {

    const [style, setStyle] = useState({});

    const getMeta = (url, cb) => {
        const img = new Image();
        img.onload = () => cb(null, img);
        img.onerror = (err) => cb(err);
        img.src = url;
      };

    let backHeight = 1;
    let backWidth = 1;

    getMeta(`${Images[0]}`, (err, img) =>
    {
        console.log("Updating image ratio");
        backWidth = img.naturalWidth;
        backHeight = img.naturalHeight;
    })
    
    useEffect(() =>
    {
        setStyle({
            backgroundImage: `url(${Images[0]})`,//`url(/images${imgSrc})`;
            backgroundSize: 'auto 100%',
            backgroundRepeat: 'no-repeat',
            height: '100%',
            aspectRatio: `${backWidth} / ${backHeight}`,
            flexShrink: 0,
            //position: 'relative',
            backgroundPosition: 'center',
            //width: 'auto',
            //display: 'flex',
            //alignItems: 'flex-end',
            border: '1px solid red'
        })
    }, [backHeight, backWidth])

    const tagId = uniqueId();
    console.log(tagId);

    return <div className="video-card" key={tagId} style={style}>
        <div className="video-details">
            <h4>{Title}</h4>
        </div>
    </div>
}