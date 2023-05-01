import {useState, useEffect} from 'react'
import { useParams} from 'react-router-dom'

export default function FetchVideo({children}){
    const [video, setVideo] = useState();
    const {id} = useParams();

    useEffect(() =>{
        fetch(`http://${window.location.host.split(":", 1)[0]}:5100/api/video/${id}`)
        .then((res) => res.json())
        .then((res) => setVideo(res))
        .catch((e) => console.log("Error getting video"));
    }, [id])

    return video && children(video);
}