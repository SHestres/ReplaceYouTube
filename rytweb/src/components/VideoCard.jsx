import { useState, useEffect } from "react";

export default function VideoCard({Title, id, vote_average, poster, Images }) {

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
    
    
    /*
    getMeta(`${Images[0]}`, (err, img) =>
    {
    console.log("Updating image ratio");
    backWidth = img.naturalWidth;
    backHeight = img.naturalHeight;
    })*/

    
    useEffect(() =>
    {


        /*
        setStyle({
            
            backgroundImage: `url(${Images[0]})`,//`url(/images${imgSrc})`;
            backgroundSize: 'auto 100%',
            backgroundRepeat: 'no-repeat',
            height: '100%',
            aspectRatio: `${backWidth} / ${backHeight}`,
            backgroundPosition: 'center',
            
            flexShrink: 0,
            //position: 'relative',
            
            //width: 'auto',
            //display: 'flex',
            //alignItems: 'flex-end',
            border: '1px solid red',



        })*/

        setStyle({
            '--backImg': `url(${Images[0]})`,
            '--backWidth': `${backWidth}`,
            '--backHeight': `${backHeight}`,
        });

            //"--backImg: `url(${Images[0]})`"

    }, [backHeight, backWidth])

    return (
        <div className="video-card" style={style}>
            <div className="video-details">
                <h4 className="video-title">{Title}</h4>
                <div>A brief description</div>
            </div>
        </div>)
}