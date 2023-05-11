import VideoCard from "./VideoCard"
import { uniqueId } from "lodash"
import { useEffect, useState } from "react";

export default function SearchGrid({cat, input}){

    const [videos, setVideos] = useState({});
    const [isFinished, setIsFinished] = useState(false);

    const updateVideos = async() => {
        fetch(`http://${window.location.host.split(":", 1)[0]}:5100/api/search/${cat}${input ? "/" : ""}${input}`)
        .then((res) => res.json())
        .then((res) =>{
            setVideos(res);
            setIsFinished(true);
        })
        .catch(console.error("Couldn't set videos"));
    }

    useEffect(() =>{
        updateVideos()
        .catch(console.error)
    }, [input, cat]);


    return(
        isFinished && 
        <div className="search-grid">
            {console.log("videos", videos)}
            {videos.map( (video) => <VideoCard key={uniqueId()} {...video} />)}
        </div>
    )

}